#include "ra.h"
#include <algorithm>

int RATranslator::next_id_ = 1;

std::string RANode::to_linear_string() const {
    std::ostringstream ss;
    switch (type) {
        case RAOpType::RELATION:
            ss << details;
            break;
        case RAOpType::CROSS_PRODUCT:
            if (children.size() >= 2) {
                ss << "(" << children[0]->to_linear_string() << " \u00D7 " << children[1]->to_linear_string() << ")";
            }
            break;
        case RAOpType::THETA_JOIN:
            if (children.size() >= 2) {
                ss << "(" << children[0]->to_linear_string() << " \u22C8";
                if (!join_type.empty() && join_type != "INNER") {
                    ss << "^{" << join_type << "}";
                }
                if (!condition.empty()) {
                    ss << "_{" << condition << "}";
                }
                ss << " " << children[1]->to_linear_string() << ")";
            }
            break;
        case RAOpType::SELECTION:
            ss << "\u03C3_{" << condition << "}";
            if (!children.empty()) {
                ss << "(" << children[0]->to_linear_string() << ")";
            }
            break;
        case RAOpType::PROJECTION:
            ss << "\u03C0_{" << details << "}";
            if (!children.empty()) {
                ss << "(" << children[0]->to_linear_string() << ")";
            }
            break;
        case RAOpType::AGGREGATION:
            ss << "\u03B3_{" << details << "}";
            if (!children.empty()) {
                ss << "(" << children[0]->to_linear_string() << ")";
            }
            break;
        case RAOpType::DISTINCT_OP:
            ss << "\u03B4";
            if (!children.empty()) {
                ss << "(" << children[0]->to_linear_string() << ")";
            }
            break;
        case RAOpType::SORT:
            ss << "\u03C4_{" << details << "}";
            if (!children.empty()) {
                ss << "(" << children[0]->to_linear_string() << ")";
            }
            break;
        case RAOpType::SET_UNION:
            if (children.size() >= 2) {
                ss << "(" << children[0]->to_linear_string() << " \u222A " << children[1]->to_linear_string() << ")";
            }
            break;
        case RAOpType::SET_INTERSECT:
            if (children.size() >= 2) {
                ss << "(" << children[0]->to_linear_string() << " \u2229 " << children[1]->to_linear_string() << ")";
            }
            break;
        case RAOpType::SET_EXCEPT:
            if (children.size() >= 2) {
                ss << "(" << children[0]->to_linear_string() << " \u2212 " << children[1]->to_linear_string() << ")";
            }
            break;
    }
    return ss.str();
}

std::string RANode::to_json() const {
    std::ostringstream ss;
    ss << "{\"id\":" << id << ","
       << "\"op_symbol\":\"" << escape_json(op_symbol) << "\","
       << "\"op_name\":\"" << escape_json(op_name) << "\","
       << "\"details\":\"" << escape_json(details) << "\","
       << "\"condition\":\"" << escape_json(condition) << "\","
       << "\"join_type\":\"" << escape_json(join_type) << "\","
       << "\"linear_str\":\"" << escape_json(to_linear_string()) << "\","
       << "\"attributes\":[";
    for (size_t i = 0; i < attributes.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "\"" << escape_json(attributes[i]) << "\"";
    }
    ss << "],\"children\":[";
    for (size_t i = 0; i < children.size(); ++i) {
        if (i > 0) ss << ",";
        ss << children[i]->to_json();
    }
    ss << "]}";
    return ss.str();
}

void RATranslator::collect_aggregates(const ExprNode* expr, std::vector<std::string>& aggs) {
    if (!expr) return;
    if (auto agg = dynamic_cast<const AggregateExpr*>(expr)) {
        std::string str = agg->to_string();
        if (std::find(aggs.begin(), aggs.end(), str) == aggs.end()) {
            aggs.push_back(str);
        }
    } else if (auto bin = dynamic_cast<const BinaryExpr*>(expr)) {
        collect_aggregates(bin->left, aggs);
        collect_aggregates(bin->right, aggs);
    } else if (auto un = dynamic_cast<const UnaryExpr*>(expr)) {
        collect_aggregates(un->child, aggs);
    }
}

std::shared_ptr<RANode> RATranslator::translate_table_ref(const TableRef* ref) {
    if (!ref) return nullptr;

    if (ref->kind == TableRef::Kind::BASE) {
        auto node = std::make_shared<RANode>();
        node->id = next_id_++;
        node->type = RAOpType::RELATION;
        node->op_symbol = "R";
        node->op_name = "Relation";
        std::string name = ref->table_name;
        if (!ref->alias.empty()) {
            name += " AS " + ref->alias;
        }
        node->details = name;
        node->attributes.push_back(ref->table_name);
        return node;
    } else {
        auto left_node = translate_table_ref(ref->left);
        auto right_node = translate_table_ref(ref->right);

        auto node = std::make_shared<RANode>();
        node->id = next_id_++;
        node->type = RAOpType::THETA_JOIN;
        node->op_symbol = "\u22C8"; // ⋈
        node->op_name = "Theta Join";
        node->join_type = ref->join_type.empty() ? "INNER" : ref->join_type;
        node->condition = ref->condition ? ref->condition->to_string() : "";
        node->details = (node->join_type != "INNER" ? (node->join_type + " JOIN") : "JOIN");
        if (!node->condition.empty()) {
            node->details += " ON " + node->condition;
        }
        node->children.push_back(left_node);
        node->children.push_back(right_node);
        return node;
    }
}

std::shared_ptr<RANode> RATranslator::translate_select(const SelectStmtNode* stmt) {
    if (!stmt) return nullptr;

    // 1. FROM clause
    std::shared_ptr<RANode> current = nullptr;
    if (!stmt->from_clause.empty()) {
        current = translate_table_ref(stmt->from_clause[0]);
        for (size_t i = 1; i < stmt->from_clause.size(); ++i) {
            auto right_table = translate_table_ref(stmt->from_clause[i]);
            auto cross = std::make_shared<RANode>();
            cross->id = next_id_++;
            cross->type = RAOpType::CROSS_PRODUCT;
            cross->op_symbol = "\u00D7"; // ×
            cross->op_name = "Cartesian Product";
            cross->details = "\u00D7";
            cross->children.push_back(current);
            cross->children.push_back(right_table);
            current = cross;
        }
    }

    // 2. WHERE clause -> σ
    if (stmt->where_clause && current) {
        auto sel = std::make_shared<RANode>();
        sel->id = next_id_++;
        sel->type = RAOpType::SELECTION;
        sel->op_symbol = "\u03C3"; // σ
        sel->op_name = "Selection";
        sel->condition = stmt->where_clause->to_string();
        sel->details = sel->condition;
        sel->children.push_back(current);
        current = sel;
    }

    // 3. GROUP BY & Aggregates -> γ
    std::vector<std::string> aggs;
    for (const auto* item : stmt->select_list) {
        if (item && item->expr) collect_aggregates(item->expr, aggs);
    }
    if (stmt->having_clause) {
        collect_aggregates(stmt->having_clause, aggs);
    }

    if (!stmt->group_by_clause.empty() || !aggs.empty()) {
        auto agg_node = std::make_shared<RANode>();
        agg_node->id = next_id_++;
        agg_node->type = RAOpType::AGGREGATION;
        agg_node->op_symbol = "\u03B3"; // γ
        agg_node->op_name = "Aggregation";

        std::string grp_str;
        for (size_t i = 0; i < stmt->group_by_clause.size(); ++i) {
            if (i > 0) grp_str += ", ";
            std::string col = stmt->group_by_clause[i]->to_string();
            grp_str += col;
            agg_node->attributes.push_back(col);
        }

        std::string agg_str;
        for (size_t i = 0; i < aggs.size(); ++i) {
            if (i > 0) agg_str += ", ";
            agg_str += aggs[i];
            agg_node->attributes.push_back(aggs[i]);
        }

        if (!grp_str.empty() && !agg_str.empty()) {
            agg_node->details = grp_str + "; " + agg_str;
        } else if (!grp_str.empty()) {
            agg_node->details = grp_str;
        } else {
            agg_node->details = agg_str;
        }

        if (current) agg_node->children.push_back(current);
        current = agg_node;
    }

    // 4. HAVING clause -> σ
    if (stmt->having_clause && current) {
        auto hav_sel = std::make_shared<RANode>();
        hav_sel->id = next_id_++;
        hav_sel->type = RAOpType::SELECTION;
        hav_sel->op_symbol = "\u03C3"; // σ
        hav_sel->op_name = "Selection (HAVING)";
        hav_sel->condition = stmt->having_clause->to_string();
        hav_sel->details = hav_sel->condition;
        hav_sel->children.push_back(current);
        current = hav_sel;
    }

    // 5. Projection -> π
    std::string proj_details;
    std::vector<std::string> proj_attrs;
    for (size_t i = 0; i < stmt->select_list.size(); ++i) {
        if (i > 0) proj_details += ", ";
        std::string item_str = stmt->select_list[i]->to_string();
        proj_details += item_str;
        proj_attrs.push_back(item_str);
    }

    auto proj = std::make_shared<RANode>();
    proj->id = next_id_++;
    proj->type = RAOpType::PROJECTION;
    proj->op_symbol = "\u03C0"; // π
    proj->op_name = "Projection";
    proj->details = proj_details;
    proj->attributes = proj_attrs;
    if (current) proj->children.push_back(current);
    current = proj;

    // 6. DISTINCT -> δ
    if (stmt->is_distinct && current) {
        auto dist = std::make_shared<RANode>();
        dist->id = next_id_++;
        dist->type = RAOpType::DISTINCT_OP;
        dist->op_symbol = "\u03B4"; // δ
        dist->op_name = "Duplicate Elimination";
        dist->details = "DISTINCT";
        dist->children.push_back(current);
        current = dist;
    }

    // 7. ORDER BY -> τ
    if (!stmt->order_by_clause.empty() && current) {
        std::string sort_str;
        for (size_t i = 0; i < stmt->order_by_clause.size(); ++i) {
            if (i > 0) sort_str += ", ";
            sort_str += stmt->order_by_clause[i]->to_string();
        }
        auto sort_node = std::make_shared<RANode>();
        sort_node->id = next_id_++;
        sort_node->type = RAOpType::SORT;
        sort_node->op_symbol = "\u03C4"; // τ
        sort_node->op_name = "Sort";
        sort_node->details = sort_str;
        sort_node->children.push_back(current);
        current = sort_node;
    }

    return current;
}

std::shared_ptr<RANode> RATranslator::translate_set_op(const SetOpQueryNode* set_op) {
    if (!set_op) return nullptr;

    auto left_ra = translate(set_op->left);
    auto right_ra = translate(set_op->right);

    auto node = std::make_shared<RANode>();
    node->id = next_id_++;

    if (set_op->op_type.find("UNION") != std::string::npos) {
        node->type = RAOpType::SET_UNION;
        node->op_symbol = "\u222A"; // ∪
        node->op_name = "Union";
        node->details = set_op->op_type;
    } else if (set_op->op_type.find("INTERSECT") != std::string::npos) {
        node->type = RAOpType::SET_INTERSECT;
        node->op_symbol = "\u2229"; // ∩
        node->op_name = "Intersect";
        node->details = set_op->op_type;
    } else {
        node->type = RAOpType::SET_EXCEPT;
        node->op_symbol = "\u2212"; // −
        node->op_name = "Except";
        node->details = set_op->op_type;
    }

    node->children.push_back(left_ra);
    node->children.push_back(right_ra);
    return node;
}

std::shared_ptr<RANode> RATranslator::translate(const ASTNode* root) {
    next_id_ = 1;
    if (!root) return nullptr;

    if (auto sel = dynamic_cast<const SelectStmtNode*>(root)) {
        return translate_select(sel);
    } else if (auto set_op = dynamic_cast<const SetOpQueryNode*>(root)) {
        return translate_set_op(set_op);
    }
    return nullptr;
}
