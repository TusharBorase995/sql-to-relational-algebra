#ifndef RA_H
#define RA_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "ast.h"
#include "json_helper.h"

enum class RAOpType {
    RELATION,
    PROJECTION,
    SELECTION,
    THETA_JOIN,
    CROSS_PRODUCT,
    AGGREGATION,
    SORT,
    DISTINCT_OP,
    SET_UNION,
    SET_INTERSECT,
    SET_EXCEPT
};

struct RANode {
    int id = 0;
    RAOpType type;
    std::string op_symbol;
    std::string op_name;
    std::string details;
    std::vector<std::string> attributes;
    std::string condition;
    std::string join_type; // "INNER", "LEFT", "RIGHT"
    std::vector<std::shared_ptr<RANode>> children;

    std::string to_linear_string() const;
    std::string to_json() const;
};

class RATranslator {
public:
    static std::shared_ptr<RANode> translate(const ASTNode* root);

private:
    static int next_id_;
    static std::shared_ptr<RANode> translate_select(const SelectStmtNode* stmt);
    static std::shared_ptr<RANode> translate_table_ref(const TableRef* ref);
    static std::shared_ptr<RANode> translate_set_op(const SetOpQueryNode* set_op);
    static void collect_aggregates(const ExprNode* expr, std::vector<std::string>& aggs);
};

#endif // RA_H
