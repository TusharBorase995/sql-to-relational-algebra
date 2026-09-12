#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <memory>
#include <string>
#include <vector>
#include <set>
#include "ra.h"
#include "schema.h"

class QueryOptimizer {
public:
    static std::shared_ptr<RANode> optimize(const std::shared_ptr<RANode>& unoptimized_root);

private:
    static int next_opt_id_;

    static std::shared_ptr<RANode> clone_node(const std::shared_ptr<RANode>& node);
    static void renumber_ids(std::shared_ptr<RANode>& node);

    // Rule 1: Merge cascaded selections σ(σ(R)) -> σ_{c1 AND c2}(R)
    static std::shared_ptr<RANode> merge_selections(std::shared_ptr<RANode> root);

    // Rule 2: Push selections down through Joins, Cross Products, and Projections
    static std::shared_ptr<RANode> push_selections(std::shared_ptr<RANode> root);

    // Rule 3: Merge cascaded projections π(π(R)) -> π(R)
    static std::shared_ptr<RANode> merge_projections(std::shared_ptr<RANode> root);

    // Rule 4: Push projections down through joins/cross products where valid
    static std::shared_ptr<RANode> push_projections(std::shared_ptr<RANode> root);

    // Helpers
    static std::vector<std::string> split_conjuncts(const std::string& cond);
    static std::string join_conjuncts(const std::vector<std::string>& conjuncts);
    static std::set<std::string> get_table_names_in_subtree(const std::shared_ptr<RANode>& node);
    static std::set<std::string> get_referenced_tables_in_expr(const std::string& expr);
    static std::set<std::string> get_referenced_columns_in_expr(const std::string& expr);
    static std::set<std::string> get_available_columns(const std::shared_ptr<RANode>& node);
};

#endif // OPTIMIZER_H
