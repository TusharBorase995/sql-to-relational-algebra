#include "optimizer.h"
#include <sstream>
#include <algorithm>
#include <cctype>

int QueryOptimizer::next_opt_id_ = 100;

std::shared_ptr<RANode> QueryOptimizer::clone_node(const std::shared_ptr<RANode>& node) {
    if (!node) return nullptr;
    auto copy = std::make_shared<RANode>();
    copy->id = node->id;
    copy->type = node->type;
    copy->op_symbol = node->op_symbol;
    copy->op_name = node->op_name;
    copy->details = node->details;
    copy->attributes = node->attributes;
    copy->condition = node->condition;
    copy->join_type = node->join_type;
    for (const auto& ch : node->children) {
        copy->children.push_back(clone_node(ch));
    }
    return copy;
}

void QueryOptimizer::renumber_ids(std::shared_ptr<RANode>& node) {
    if (!node) return;
    for (auto& ch : node->children) {
        renumber_ids(ch);
    }
    node->id = next_opt_id_++;
}

static std::string trim_str(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static std::string strip_outer_parens(const std::string& s) {
    std::string str = trim_str(s);
    while (str.size() >= 2 && str.front() == '(' && str.back() == ')') {
        // Check if these outer parens match each other
        int depth = 0;
        bool wraps_all = true;
        for (size_t i = 0; i < str.size() - 1; ++i) {
            if (str[i] == '(') depth++;
            else if (str[i] == ')') depth--;
            if (depth == 0) {
                wraps_all = false;
                break;
            }
        }
        if (wraps_all) {
            str = trim_str(str.substr(1, str.size() - 2));
        } else {
            break;
        }
    }
    return str;
}

std::vector<std::string> QueryOptimizer::split_conjuncts(const std::string& cond) {
    std::vector<std::string> result;
    std::string s = strip_outer_parens(cond);
    if (s.empty()) return result;

    int depth = 0;
    size_t last = 0;
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] == '(') depth++;
        else if (s[i] == ')') depth--;
        else if (depth == 0) {
            // Check for " AND " (case-insensitive)
            if (i + 5 <= s.size()) {
                std::string sub = s.substr(i, 5);
                std::string upper;
                for (char c : sub) upper += (char)toupper(c);
                if (upper == " AND ") {
                    std::string part = trim_str(s.substr(last, i - last));
                    if (!part.empty()) {
                        auto subparts = split_conjuncts(part);
                        result.insert(result.end(), subparts.begin(), subparts.end());
                    }
                    i += 5;
                    last = i;
                    continue;
                }
            }
        }
        i++;
    }

    std::string rem = trim_str(s.substr(last));
    if (!rem.empty()) {
        std::string stripped = strip_outer_parens(rem);
        // If stripped is different from rem and contains AND, recursively split
        if (stripped != rem && stripped.find(" AND ") != std::string::npos) {
            auto subparts = split_conjuncts(stripped);
            result.insert(result.end(), subparts.begin(), subparts.end());
        } else {
            result.push_back(rem);
        }
    }

    return result;
}

std::string QueryOptimizer::join_conjuncts(const std::vector<std::string>& conjuncts) {
    if (conjuncts.empty()) return "";
    if (conjuncts.size() == 1) return conjuncts[0];
    std::ostringstream ss;
    for (size_t i = 0; i < conjuncts.size(); ++i) {
        if (i > 0) ss << " AND ";
        ss << conjuncts[i];
    }
    return ss.str();
}

std::set<std::string> QueryOptimizer::get_table_names_in_subtree(const std::shared_ptr<RANode>& node) {
    std::set<std::string> tables;
    if (!node) return tables;

    if (node->type == RAOpType::RELATION) {
        // details can be "students" or "students AS s"
        std::string det = node->details;
        size_t as_pos = det.find(" AS ");
        if (as_pos != std::string::npos) {
            std::string tbl = trim_str(det.substr(0, as_pos));
            std::string alias = trim_str(det.substr(as_pos + 4));
            tables.insert(tbl);
            tables.insert(alias);
        } else {
            tables.insert(trim_str(det));
        }
        for (const auto& a : node->attributes) {
            tables.insert(a);
        }
    }

    for (const auto& ch : node->children) {
        auto ch_tables = get_table_names_in_subtree(ch);
        tables.insert(ch_tables.begin(), ch_tables.end());
    }
    return tables;
}

std::set<std::string> QueryOptimizer::get_referenced_tables_in_expr(const std::string& expr) {
    std::set<std::string> tables;
    const auto& catalog = SchemaCatalog::instance();

    // Look for identifier.identifier patterns
    for (size_t i = 0; i < expr.size(); ++i) {
        if (expr[i] == '.') {
            // Scan left for identifier
            int left = (int)i - 1;
            while (left >= 0 && (isalnum(expr[left]) || expr[left] == '_')) {
                left--;
            }
            std::string tbl = expr.substr(left + 1, (int)i - left - 1);
            if (!tbl.empty()) {
                tables.insert(tbl);
            }
        }
    }

    // Also scan standalone identifier tokens and check against catalog
    size_t idx = 0;
    while (idx < expr.size()) {
        if (isalpha(expr[idx]) || expr[idx] == '_') {
            size_t start = idx;
            while (idx < expr.size() && (isalnum(expr[idx]) || expr[idx] == '_')) {
                idx++;
            }
            std::string word = expr.substr(start, idx - start);
            // Ignore SQL keywords
            std::string up;
            for (char c : word) up += (char)toupper(c);
            if (up != "AND" && up != "OR" && up != "NOT" && up != "LIKE" &&
                up != "IN" && up != "BETWEEN" && up != "COUNT" && up != "SUM" &&
                up != "AVG" && up != "MIN" && up != "MAX" && up != "TRUE" &&
                up != "FALSE" && up != "NULL") {
                // If this column exists in catalog, associate with its table
                for (const auto& kv : catalog.get_tables()) {
                    if (kv.second.has_column(word)) {
                        tables.insert(kv.first);
                    }
                }
            }
        } else {
            idx++;
        }
    }

    return tables;
}

std::shared_ptr<RANode> QueryOptimizer::merge_selections(std::shared_ptr<RANode> root) {
    if (!root) return nullptr;

    // Recursively optimize children first
    for (size_t i = 0; i < root->children.size(); ++i) {
        root->children[i] = merge_selections(root->children[i]);
    }

    // Merge cascaded selections: σ_{c1}(σ_{c2}(R)) -> σ_{c1 AND c2}(R)
    if (root->type == RAOpType::SELECTION && !root->children.empty()) {
        auto child = root->children[0];
        if (child && child->type == RAOpType::SELECTION) {
            std::string merged_cond = root->condition + " AND " + child->condition;
            root->condition = merged_cond;
            root->details = merged_cond;
            root->children = child->children;
            // Recurse again in case of a 3-way cascade
            return merge_selections(root);
        }
    }

    return root;
}

std::shared_ptr<RANode> QueryOptimizer::merge_projections(std::shared_ptr<RANode> root) {
    if (!root) return nullptr;

    for (size_t i = 0; i < root->children.size(); ++i) {
        root->children[i] = merge_projections(root->children[i]);
    }

    // Merge cascaded projections: π_{L1}(π_{L2}(R)) -> π_{L1}(R)
    if (root->type == RAOpType::PROJECTION && !root->children.empty()) {
        auto child = root->children[0];
        if (child && child->type == RAOpType::PROJECTION) {
            root->children = child->children;
            return merge_projections(root);
        }
    }

    return root;
}

std::shared_ptr<RANode> QueryOptimizer::push_selections(std::shared_ptr<RANode> root) {
    if (!root) return nullptr;

    // First recursively push selections in children
    for (size_t i = 0; i < root->children.size(); ++i) {
        root->children[i] = push_selections(root->children[i]);
    }

    if (root->type != RAOpType::SELECTION || root->children.empty()) {
        return root;
    }

    auto child = root->children[0];
    auto conjuncts = split_conjuncts(root->condition);

    // Case 1: Child is JOIN or CROSS_PRODUCT
    if ((child->type == RAOpType::CROSS_PRODUCT || child->type == RAOpType::THETA_JOIN) && child->children.size() >= 2) {
        auto left_tables = get_table_names_in_subtree(child->children[0]);
        auto right_tables = get_table_names_in_subtree(child->children[1]);

        std::vector<std::string> left_conjuncts;
        std::vector<std::string> right_conjuncts;
        std::vector<std::string> join_conjuncts_list;
        std::vector<std::string> remaining;

        for (const auto& conj : conjuncts) {
            auto ref_tables = get_referenced_tables_in_expr(conj);

            bool in_left = false;
            bool in_right = false;
            for (const auto& t : ref_tables) {
                if (left_tables.count(t)) in_left = true;
                if (right_tables.count(t)) in_right = true;
            }

            if (in_left && !in_right) {
                left_conjuncts.push_back(conj);
            } else if (in_right && !in_left) {
                right_conjuncts.push_back(conj);
            } else if (in_left && in_right) {
                join_conjuncts_list.push_back(conj);
            } else {
                remaining.push_back(conj);
            }
        }

        bool pushed_anything = false;

        // Push to left
        if (!left_conjuncts.empty()) {
            pushed_anything = true;
            auto sel_left = std::make_shared<RANode>();
            sel_left->type = RAOpType::SELECTION;
            sel_left->op_symbol = "\u03C3";
            sel_left->op_name = "Selection";
            sel_left->condition = join_conjuncts(left_conjuncts);
            sel_left->details = sel_left->condition;
            sel_left->children.push_back(child->children[0]);
            child->children[0] = push_selections(sel_left);
        }

        // Push to right
        if (!right_conjuncts.empty()) {
            pushed_anything = true;
            auto sel_right = std::make_shared<RANode>();
            sel_right->type = RAOpType::SELECTION;
            sel_right->op_symbol = "\u03C3";
            sel_right->op_name = "Selection";
            sel_right->condition = join_conjuncts(right_conjuncts);
            sel_right->details = sel_right->condition;
            sel_right->children.push_back(child->children[1]);
            child->children[1] = push_selections(sel_right);
        }

        // Convert Cross Product to Theta Join if join conjuncts exist
        if (!join_conjuncts_list.empty()) {
            pushed_anything = true;
            std::string join_cond = join_conjuncts(join_conjuncts_list);
            if (child->type == RAOpType::CROSS_PRODUCT) {
                child->type = RAOpType::THETA_JOIN;
                child->op_symbol = "\u22C8";
                child->op_name = "Theta Join";
                child->join_type = "INNER";
                child->condition = join_cond;
                child->details = "JOIN ON " + join_cond;
            } else if (child->type == RAOpType::THETA_JOIN) {
                if (!child->condition.empty()) {
                    child->condition = child->condition + " AND " + join_cond;
                } else {
                    child->condition = join_cond;
                }
                child->details = (child->join_type != "INNER" ? (child->join_type + " JOIN") : "JOIN") + " ON " + child->condition;
            }
        }

        if (pushed_anything) {
            if (remaining.empty()) {
                return child;
            } else {
                root->condition = join_conjuncts(remaining);
                root->details = root->condition;
                root->children[0] = child;
                return root;
            }
        }
    }

    // Case 2: Child is PROJECTION: σ_C(π_L(R)) -> π_L(σ_C(R))
    if (child->type == RAOpType::PROJECTION && !child->children.empty()) {
        auto inner_child = child->children[0];
        root->children[0] = inner_child;
        auto new_sel = push_selections(root);
        child->children[0] = new_sel;
        return child;
    }

    return root;
}

std::shared_ptr<RANode> QueryOptimizer::push_projections(std::shared_ptr<RANode> root) {
    if (!root) return nullptr;

    for (size_t i = 0; i < root->children.size(); ++i) {
        root->children[i] = push_projections(root->children[i]);
    }

    // Push projection below joins / cross products if valid
    if (root->type == RAOpType::PROJECTION && !root->children.empty()) {
        auto child = root->children[0];
        if ((child->type == RAOpType::THETA_JOIN || child->type == RAOpType::CROSS_PRODUCT) && child->children.size() >= 2) {
            const auto& catalog = SchemaCatalog::instance();
            // Collect needed columns in root details and join condition
            std::string needed_str = root->details + " " + child->condition;

            auto push_early_proj = [&](std::shared_ptr<RANode>& branch) {
                if (!branch || branch->type != RAOpType::RELATION) return;
                std::string base_tbl = branch->attributes.empty() ? "" : branch->attributes[0];
                const TableMeta* meta = catalog.get_table(base_tbl);
                if (!meta) return;

                std::vector<std::string> needed_cols;
                for (const auto& col : meta->columns) {
                    // Check if col.name appears in needed_str
                    if (needed_str.find(col.name) != std::string::npos) {
                        needed_cols.push_back(col.name);
                    }
                }

                // If needed_cols is a strict subset of all columns
                if (!needed_cols.empty() && needed_cols.size() < meta->columns.size()) {
                    auto early_proj = std::make_shared<RANode>();
                    early_proj->type = RAOpType::PROJECTION;
                    early_proj->op_symbol = "\u03C0";
                    early_proj->op_name = "Projection (Early)";
                    std::string dt;
                    for (size_t k = 0; k < needed_cols.size(); ++k) {
                        if (k > 0) dt += ", ";
                        dt += needed_cols[k];
                        early_proj->attributes.push_back(needed_cols[k]);
                    }
                    early_proj->details = dt;
                    early_proj->children.push_back(branch);
                    branch = early_proj;
                }
            };

            push_early_proj(child->children[0]);
            push_early_proj(child->children[1]);
        }
    }

    return root;
}

std::shared_ptr<RANode> QueryOptimizer::optimize(const std::shared_ptr<RANode>& unoptimized_root) {
    if (!unoptimized_root) return nullptr;

    next_opt_id_ = 100;
    auto opt_tree = clone_node(unoptimized_root);

    // Apply optimization passes:
    // 1. Merge cascaded selections
    opt_tree = merge_selections(opt_tree);

    // 2. Push selections down as close to base relations as possible
    // (also converts Cartesian Product × to Theta Join ⋈ when join conditions are pushed)
    opt_tree = push_selections(opt_tree);

    // 3. Merge cascaded projections
    opt_tree = merge_projections(opt_tree);

    // 4. Push projections down where valid
    opt_tree = push_projections(opt_tree);

    // Renumber IDs for clean visual graph keys
    renumber_ids(opt_tree);

    return opt_tree;
}
