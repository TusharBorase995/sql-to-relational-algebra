#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "json_helper.h"

// Forward declarations
class ASTNode;
class ExprNode;
struct SelectItem;
struct TableRef;

// Base AST Node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::string node_type() const = 0;
    virtual std::string to_json() const = 0;
    virtual std::string to_string() const = 0;
};

// Base Expression Node
class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
};

// Binary Expression: left OP right (e.g. =, <, >, <=, >=, <>, AND, OR)
class BinaryExpr : public ExprNode {
public:
    std::string op;
    ExprNode* left;
    ExprNode* right;

    BinaryExpr(const std::string& op, ExprNode* left, ExprNode* right)
        : op(op), left(left), right(right) {}

    ~BinaryExpr() override {
        delete left;
        delete right;
    }

    std::string node_type() const override { return "BinaryExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Unary Expression: OP child (e.g. NOT)
class UnaryExpr : public ExprNode {
public:
    std::string op;
    ExprNode* child;

    UnaryExpr(const std::string& op, ExprNode* child)
        : op(op), child(child) {}

    ~UnaryExpr() override {
        delete child;
    }

    std::string node_type() const override { return "UnaryExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Column Reference (e.g. "col" or "table.col")
class ColumnRefExpr : public ExprNode {
public:
    std::string table_name; // empty if unqualified
    std::string column_name;

    ColumnRefExpr(const std::string& col)
        : table_name(""), column_name(col) {}

    ColumnRefExpr(const std::string& tbl, const std::string& col)
        : table_name(tbl), column_name(col) {}

    std::string node_type() const override { return "ColumnRefExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Literal Expression: INT, FLOAT, STRING, BOOLEAN, NULL
class LiteralExpr : public ExprNode {
public:
    enum class LiteralType {
        INT,
        FLOAT,
        STRING,
        BOOLEAN,
        NULL_VALUE
    };

    LiteralType lit_type;
    std::string value;

    LiteralExpr(LiteralType type, const std::string& val)
        : lit_type(type), value(val) {}

    std::string node_type() const override { return "LiteralExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Wildcard Expression: "*" or "table.*"
class StarExpr : public ExprNode {
public:
    std::string table_name; // empty for "*"

    StarExpr(const std::string& tbl = "")
        : table_name(tbl) {}

    std::string node_type() const override { return "StarExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Aggregate Function Expression: COUNT, SUM, AVG, MIN, MAX
class AggregateExpr : public ExprNode {
public:
    std::string func_name; // "COUNT", "SUM", "AVG", "MIN", "MAX"
    bool is_distinct = false;
    ExprNode* arg = nullptr; // nullptr or StarExpr for COUNT(*)

    AggregateExpr(const std::string& name, bool distinct, ExprNode* arg)
        : func_name(name), is_distinct(distinct), arg(arg) {}

    ~AggregateExpr() override {
        delete arg;
    }

    std::string node_type() const override { return "AggregateExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// LIKE Expression: expr [NOT] LIKE pattern
class LikeExpr : public ExprNode {
public:
    ExprNode* expr;
    ExprNode* pattern;
    bool is_not = false;

    LikeExpr(ExprNode* e, ExprNode* p, bool not_flag = false)
        : expr(e), pattern(p), is_not(not_flag) {}

    ~LikeExpr() override {
        delete expr;
        delete pattern;
    }

    std::string node_type() const override { return "LikeExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// IN Expression: expr [NOT] IN (val1, val2, ...)
class InExpr : public ExprNode {
public:
    ExprNode* expr;
    std::vector<ExprNode*> values;
    bool is_not = false;

    InExpr(ExprNode* e, const std::vector<ExprNode*>& vals, bool not_flag = false)
        : expr(e), values(vals), is_not(not_flag) {}

    ~InExpr() override {
        delete expr;
        for (auto* v : values) delete v;
    }

    std::string node_type() const override { return "InExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// BETWEEN Expression: expr [NOT] BETWEEN lower AND upper
class BetweenExpr : public ExprNode {
public:
    ExprNode* expr;
    ExprNode* lower;
    ExprNode* upper;
    bool is_not = false;

    BetweenExpr(ExprNode* e, ExprNode* l, ExprNode* u, bool not_flag = false)
        : expr(e), lower(l), upper(u), is_not(not_flag) {}

    ~BetweenExpr() override {
        delete expr;
        delete lower;
        delete upper;
    }

    std::string node_type() const override { return "BetweenExpr"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Order By Item
struct OrderByItem {
    ExprNode* expr;
    bool is_desc = false; // false = ASC, true = DESC

    OrderByItem(ExprNode* e, bool desc = false)
        : expr(e), is_desc(desc) {}

    ~OrderByItem() {
        delete expr;
    }

    std::string to_json() const;
    std::string to_string() const;
};

// Select Item: expression with optional alias
struct SelectItem {
    ExprNode* expr;
    std::string alias;

    SelectItem(ExprNode* e, const std::string& a = "")
        : expr(e), alias(a) {}

    ~SelectItem() {
        delete expr;
    }

    std::string to_json() const;
    std::string to_string() const;
};

// Table Reference in FROM clause: Base table or Join
struct TableRef {
    enum class Kind { BASE, JOIN };
    Kind kind;

    // For BASE
    std::string table_name;
    std::string alias;

    // For JOIN
    std::string join_type; // "INNER", "LEFT", "RIGHT", "CROSS"
    TableRef* left = nullptr;
    TableRef* right = nullptr;
    ExprNode* condition = nullptr;

    // Base constructor
    TableRef(const std::string& name, const std::string& alias = "")
        : kind(Kind::BASE), table_name(name), alias(alias) {}

    // Join constructor
    TableRef(const std::string& jtype, TableRef* l, TableRef* r, ExprNode* cond)
        : kind(Kind::JOIN), join_type(jtype), left(l), right(r), condition(cond) {}

    ~TableRef() {
        delete left;
        delete right;
        delete condition;
    }

    std::string to_json() const;
    std::string to_string() const;
};

// Select Statement Node
class SelectStmtNode : public ASTNode {
public:
    bool is_distinct = false;
    std::vector<SelectItem*> select_list;
    std::vector<TableRef*> from_clause;
    ExprNode* where_clause = nullptr;
    std::vector<ExprNode*> group_by_clause;
    ExprNode* having_clause = nullptr;
    std::vector<OrderByItem*> order_by_clause;

    SelectStmtNode() = default;

    ~SelectStmtNode() override {
        for (auto* item : select_list) delete item;
        for (auto* tbl : from_clause) delete tbl;
        delete where_clause;
        for (auto* grp : group_by_clause) delete grp;
        delete having_clause;
        for (auto* ord : order_by_clause) delete ord;
    }

    std::string node_type() const override { return "SelectStmt"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Set Operation Query Node: UNION, INTERSECT, EXCEPT
class SetOpQueryNode : public ASTNode {
public:
    std::string op_type; // "UNION", "INTERSECT", "EXCEPT"
    ASTNode* left = nullptr;
    ASTNode* right = nullptr;

    SetOpQueryNode(const std::string& op, ASTNode* l, ASTNode* r)
        : op_type(op), left(l), right(r) {}

    ~SetOpQueryNode() override {
        delete left;
        delete right;
    }

    std::string node_type() const override { return "SetOpQuery"; }
    std::string to_string() const override;
    std::string to_json() const override;
};

// Token tracking for JSON token stream
struct TokenRecord {
    std::string type;
    std::string lexeme;
    int line;
    int column;

    std::string to_json() const {
        std::ostringstream ss;
        ss << "{\"type\":\"" << escape_json(type) << "\","
           << "\"lexeme\":\"" << escape_json(lexeme) << "\","
           << "\"line\":" << line << ","
           << "\"col\":" << column << "}";
        return ss.str();
    }
};

// Global parse state
struct ParseError {
    bool has_error = false;
    std::string message;
    int line = 0;
    int column = 0;
};

#endif // AST_H
