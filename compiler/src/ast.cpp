#include "ast.h"

// --- BinaryExpr ---
std::string BinaryExpr::to_string() const {
    std::ostringstream ss;
    ss << "(" << left->to_string() << " " << op << " " << right->to_string() << ")";
    return ss.str();
}

std::string BinaryExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"BinaryExpr\","
       << "\"op\":\"" << escape_json(op) << "\","
       << "\"left\":" << left->to_json() << ","
       << "\"right\":" << right->to_json() << "}";
    return ss.str();
}

// --- UnaryExpr ---
std::string UnaryExpr::to_string() const {
    std::ostringstream ss;
    ss << "(" << op << " " << child->to_string() << ")";
    return ss.str();
}

std::string UnaryExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"UnaryExpr\","
       << "\"op\":\"" << escape_json(op) << "\","
       << "\"child\":" << child->to_json() << "}";
    return ss.str();
}

// --- ColumnRefExpr ---
std::string ColumnRefExpr::to_string() const {
    if (!table_name.empty()) {
        return table_name + "." + column_name;
    }
    return column_name;
}

std::string ColumnRefExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"ColumnRefExpr\","
       << "\"table_name\":" << (table_name.empty() ? "null" : ("\"" + escape_json(table_name) + "\"")) << ","
       << "\"column_name\":\"" << escape_json(column_name) << "\"}";
    return ss.str();
}

// --- LiteralExpr ---
std::string LiteralExpr::to_string() const {
    if (lit_type == LiteralType::STRING) {
        return "'" + value + "'";
    }
    return value;
}

std::string LiteralExpr::to_json() const {
    std::ostringstream ss;
    std::string type_str;
    switch (lit_type) {
        case LiteralType::INT: type_str = "INT"; break;
        case LiteralType::FLOAT: type_str = "FLOAT"; break;
        case LiteralType::STRING: type_str = "STRING"; break;
        case LiteralType::BOOLEAN: type_str = "BOOLEAN"; break;
        case LiteralType::NULL_VALUE: type_str = "NULL"; break;
    }
    ss << "{\"node_type\":\"LiteralExpr\","
       << "\"type\":\"" << type_str << "\","
       << "\"value\":\"" << escape_json(value) << "\"}";
    return ss.str();
}

// --- StarExpr ---
std::string StarExpr::to_string() const {
    if (!table_name.empty()) {
        return table_name + ".*";
    }
    return "*";
}

std::string StarExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"StarExpr\","
       << "\"table_name\":" << (table_name.empty() ? "null" : ("\"" + escape_json(table_name) + "\"")) << "}";
    return ss.str();
}

// --- AggregateExpr ---
std::string AggregateExpr::to_string() const {
    std::ostringstream ss;
    ss << func_name << "(";
    if (is_distinct) ss << "DISTINCT ";
    if (arg) {
        ss << arg->to_string();
    }
    ss << ")";
    return ss.str();
}

std::string AggregateExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"AggregateExpr\","
       << "\"func_name\":\"" << escape_json(func_name) << "\","
       << "\"is_distinct\":" << (is_distinct ? "true" : "false") << ","
       << "\"arg\":" << (arg ? arg->to_json() : "null") << "}";
    return ss.str();
}

// --- LikeExpr ---
std::string LikeExpr::to_string() const {
    std::ostringstream ss;
    ss << "(" << expr->to_string() << (is_not ? " NOT LIKE " : " LIKE ") << pattern->to_string() << ")";
    return ss.str();
}

std::string LikeExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"LikeExpr\","
       << "\"is_not\":" << (is_not ? "true" : "false") << ","
       << "\"expr\":" << expr->to_json() << ","
       << "\"pattern\":" << pattern->to_json() << "}";
    return ss.str();
}

// --- InExpr ---
std::string InExpr::to_string() const {
    std::ostringstream ss;
    ss << "(" << expr->to_string() << (is_not ? " NOT IN (" : " IN (");
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << values[i]->to_string();
    }
    ss << "))";
    return ss.str();
}

std::string InExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"InExpr\","
       << "\"is_not\":" << (is_not ? "true" : "false") << ","
       << "\"expr\":" << expr->to_json() << ","
       << "\"values\":[";
    for (size_t i = 0; i < values.size(); ++i) {
        if (i > 0) ss << ",";
        ss << values[i]->to_json();
    }
    ss << "]}";
    return ss.str();
}

// --- BetweenExpr ---
std::string BetweenExpr::to_string() const {
    std::ostringstream ss;
    ss << "(" << expr->to_string() << (is_not ? " NOT BETWEEN " : " BETWEEN ")
       << lower->to_string() << " AND " << upper->to_string() << ")";
    return ss.str();
}

std::string BetweenExpr::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"BetweenExpr\","
       << "\"is_not\":" << (is_not ? "true" : "false") << ","
       << "\"expr\":" << expr->to_json() << ","
       << "\"lower\":" << lower->to_json() << ","
       << "\"upper\":" << upper->to_json() << "}";
    return ss.str();
}

// --- OrderByItem ---
std::string OrderByItem::to_string() const {
    return expr->to_string() + (is_desc ? " DESC" : " ASC");
}

std::string OrderByItem::to_json() const {
    std::ostringstream ss;
    ss << "{\"expr\":" << expr->to_json() << ","
       << "\"direction\":\"" << (is_desc ? "DESC" : "ASC") << "\"}";
    return ss.str();
}

// --- SelectItem ---
std::string SelectItem::to_string() const {
    std::string s = expr->to_string();
    if (!alias.empty()) {
        s += " AS " + alias;
    }
    return s;
}

std::string SelectItem::to_json() const {
    std::ostringstream ss;
    ss << "{\"expr\":" << expr->to_json() << ","
       << "\"alias\":" << (alias.empty() ? "null" : ("\"" + escape_json(alias) + "\"")) << "}";
    return ss.str();
}

// --- TableRef ---
std::string TableRef::to_string() const {
    if (kind == Kind::BASE) {
        std::string s = table_name;
        if (!alias.empty()) s += " AS " + alias;
        return s;
    } else {
        std::string s = left->to_string() + " " + join_type + " JOIN " + right->to_string();
        if (condition) {
            s += " ON " + condition->to_string();
        }
        return s;
    }
}

std::string TableRef::to_json() const {
    std::ostringstream ss;
    if (kind == Kind::BASE) {
        ss << "{\"kind\":\"BASE\","
           << "\"table_name\":\"" << escape_json(table_name) << "\","
           << "\"alias\":" << (alias.empty() ? "null" : ("\"" + escape_json(alias) + "\"")) << "}";
    } else {
        ss << "{\"kind\":\"JOIN\","
           << "\"join_type\":\"" << escape_json(join_type) << "\","
           << "\"left\":" << left->to_json() << ","
           << "\"right\":" << right->to_json() << ","
           << "\"condition\":" << (condition ? condition->to_json() : "null") << "}";
    }
    return ss.str();
}

// --- SelectStmtNode ---
std::string SelectStmtNode::to_string() const {
    std::ostringstream ss;
    ss << "SELECT ";
    if (is_distinct) ss << "DISTINCT ";
    for (size_t i = 0; i < select_list.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << select_list[i]->to_string();
    }
    if (!from_clause.empty()) {
        ss << " FROM ";
        for (size_t i = 0; i < from_clause.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << from_clause[i]->to_string();
        }
    }
    if (where_clause) {
        ss << " WHERE " << where_clause->to_string();
    }
    if (!group_by_clause.empty()) {
        ss << " GROUP BY ";
        for (size_t i = 0; i < group_by_clause.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << group_by_clause[i]->to_string();
        }
    }
    if (having_clause) {
        ss << " HAVING " << having_clause->to_string();
    }
    if (!order_by_clause.empty()) {
        ss << " ORDER BY ";
        for (size_t i = 0; i < order_by_clause.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << order_by_clause[i]->to_string();
        }
    }
    return ss.str();
}

std::string SelectStmtNode::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"SelectStmt\","
       << "\"is_distinct\":" << (is_distinct ? "true" : "false") << ","
       << "\"select_list\":[";
    for (size_t i = 0; i < select_list.size(); ++i) {
        if (i > 0) ss << ",";
        ss << select_list[i]->to_json();
    }
    ss << "],\"from_clause\":[";
    for (size_t i = 0; i < from_clause.size(); ++i) {
        if (i > 0) ss << ",";
        ss << from_clause[i]->to_json();
    }
    ss << "],\"where_clause\":" << (where_clause ? where_clause->to_json() : "null")
       << ",\"group_by\":[";
    for (size_t i = 0; i < group_by_clause.size(); ++i) {
        if (i > 0) ss << ",";
        ss << group_by_clause[i]->to_json();
    }
    ss << "],\"having\":" << (having_clause ? having_clause->to_json() : "null")
       << ",\"order_by\":[";
    for (size_t i = 0; i < order_by_clause.size(); ++i) {
        if (i > 0) ss << ",";
        ss << order_by_clause[i]->to_json();
    }
    ss << "]}";
    return ss.str();
}

// --- SetOpQueryNode ---
std::string SetOpQueryNode::to_string() const {
    return "(" + left->to_string() + " " + op_type + " " + right->to_string() + ")";
}

std::string SetOpQueryNode::to_json() const {
    std::ostringstream ss;
    ss << "{\"node_type\":\"SetOpQuery\","
       << "\"op_type\":\"" << escape_json(op_type) << "\","
       << "\"left\":" << left->to_json() << ","
       << "\"right\":" << right->to_json() << "}";
    return ss.str();
}
