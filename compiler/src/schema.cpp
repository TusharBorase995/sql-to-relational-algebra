#include "schema.h"
#include "simple_json.h"
#include <fstream>
#include <algorithm>

SchemaCatalog& SchemaCatalog::instance() {
    static SchemaCatalog cat;
    return cat;
}

SchemaCatalog::SchemaCatalog() {
    reset_to_defaults();
}

void SchemaCatalog::clear() {
    tables_.clear();
}

void SchemaCatalog::add_table(const TableMeta& table) {
    tables_[table.name] = table;
}

void SchemaCatalog::reset_to_defaults() {
    clear();

    // 1. students
    TableMeta students;
    students.name = "students";
    students.columns = {
        {"id", "INT", true, false, "", ""},
        {"name", "VARCHAR", false, false, "", ""},
        {"age", "INT", false, false, "", ""},
        {"dept_id", "INT", false, true, "departments", "id"},
        {"gpa", "FLOAT", false, false, "", ""}
    };
    tables_["students"] = students;

    // 2. departments
    TableMeta departments;
    departments.name = "departments";
    departments.columns = {
        {"id", "INT", true, false, "", ""},
        {"name", "VARCHAR", false, false, "", ""},
        {"building", "VARCHAR", false, false, "", ""}
    };
    tables_["departments"] = departments;

    // 3. courses
    TableMeta courses;
    courses.name = "courses";
    courses.columns = {
        {"id", "INT", true, false, "", ""},
        {"code", "VARCHAR", false, false, "", ""},
        {"title", "VARCHAR", false, false, "", ""},
        {"dept_id", "INT", false, true, "departments", "id"},
        {"credits", "INT", false, false, "", ""}
    };
    tables_["courses"] = courses;

    // 4. enrollments
    TableMeta enrollments;
    enrollments.name = "enrollments";
    enrollments.columns = {
        {"student_id", "INT", false, true, "students", "id"},
        {"course_id", "INT", false, true, "courses", "id"},
        {"semester", "VARCHAR", false, false, "", ""},
        {"grade", "VARCHAR", false, false, "", ""}
    };
    tables_["enrollments"] = enrollments;
}

bool SchemaCatalog::load_from_json(const std::string& json_str) {
    JsonVal root;
    if (!SimpleJsonParser::parse(json_str, root)) {
        return false;
    }

    const JsonVal* tbls = nullptr;
    if (root.has_key("tables") && root["tables"].is_arr()) {
        tbls = &root["tables"];
    } else if (root.is_arr()) {
        tbls = &root;
    } else {
        return false;
    }

    clear();
    for (size_t i = 0; i < tbls->arr.size(); ++i) {
        const auto& t_val = (*tbls)[i];
        if (!t_val.is_obj() || !t_val.has_key("name")) continue;
        TableMeta tm;
        tm.name = t_val["name"].as_string();
        if (t_val.has_key("columns") && t_val["columns"].is_arr()) {
            const auto& cols_arr = t_val["columns"];
            for (size_t j = 0; j < cols_arr.arr.size(); ++j) {
                const auto& c_val = cols_arr[j];
                if (!c_val.is_obj() || !c_val.has_key("name")) continue;
                ColumnMeta cm;
                cm.name = c_val["name"].as_string();
                cm.type = c_val.has_key("type") ? c_val["type"].as_string("VARCHAR") : "VARCHAR";
                cm.is_primary_key = c_val.has_key("is_pk") ? c_val["is_pk"].as_bool() : false;
                cm.is_foreign_key = c_val.has_key("is_fk") ? c_val["is_fk"].as_bool() : false;
                if (c_val.has_key("fk_target")) {
                    std::string target = c_val["fk_target"].as_string();
                    size_t dot = target.find('.');
                    if (dot != std::string::npos) {
                        cm.fk_target_table = target.substr(0, dot);
                        cm.fk_target_column = target.substr(dot + 1);
                    } else {
                        cm.fk_target_table = target;
                    }
                }
                tm.columns.push_back(cm);
            }
        }
        add_table(tm);
    }
    return true;
}

bool SchemaCatalog::load_from_file(const std::string& filepath) {
    std::ifstream f(filepath);
    if (!f.is_open()) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    return load_from_json(ss.str());
}

std::string SchemaCatalog::to_json() const {
    std::ostringstream ss;
    ss << "{\"tables\":[";
    bool first_t = true;
    for (const auto& kv : tables_) {
        if (!first_t) ss << ",";
        first_t = false;
        const auto& t = kv.second;
        ss << "{\"name\":\"" << escape_json(t.name) << "\",\"columns\":[";
        for (size_t i = 0; i < t.columns.size(); ++i) {
            if (i > 0) ss << ",";
            const auto& c = t.columns[i];
            ss << "{\"name\":\"" << escape_json(c.name) << "\","
               << "\"type\":\"" << escape_json(c.type) << "\","
               << "\"is_pk\":" << (c.is_primary_key ? "true" : "false") << ","
               << "\"is_fk\":" << (c.is_foreign_key ? "true" : "false");
            if (c.is_foreign_key) {
                ss << ",\"fk_target\":\"" << escape_json(c.fk_target_table + "." + c.fk_target_column) << "\"";
            }
            ss << "}";
        }
        ss << "]}";
    }
    ss << "]}";
    return ss.str();
}

void SchemaCatalog::extract_table_refs(const TableRef* ref, std::unordered_map<std::string, std::string>& scope, ValidationResult& res) const {
    if (!ref) return;
    if (ref->kind == TableRef::Kind::BASE) {
        std::string key = ref->alias.empty() ? ref->table_name : ref->alias;
        scope[key] = ref->table_name;
        if (!ref->alias.empty()) {
            scope[ref->table_name] = ref->table_name;
        }

        if (!has_table(ref->table_name)) {
            if (lenient_mode_) {
                res.warnings.push_back("Table '" + ref->table_name + "' is not registered in schema catalog (treated as ad-hoc custom relation).");
            } else {
                res.valid = false;
                res.errors.push_back("Table '" + ref->table_name + "' does not exist in schema.");
            }
        }
    } else {
        extract_table_refs(ref->left, scope, res);
        extract_table_refs(ref->right, scope, res);
        if (ref->condition) {
            validate_expr(ref->condition, scope, res);
        }
    }
}

void SchemaCatalog::validate_expr(const ExprNode* expr, const std::unordered_map<std::string, std::string>& scope, ValidationResult& res) const {
    if (!expr) return;

    if (auto col = dynamic_cast<const ColumnRefExpr*>(expr)) {
        if (!col->table_name.empty()) {
            auto it = scope.find(col->table_name);
            if (it == scope.end()) {
                res.valid = false;
                res.errors.push_back("Unknown table or alias '" + col->table_name + "' in column reference '" + col->to_string() + "'.");
            } else {
                const TableMeta* meta = get_table(it->second);
                if (meta) {
                    if (!meta->has_column(col->column_name)) {
                        res.valid = false;
                        res.errors.push_back("Column '" + col->column_name + "' does not exist in table '" + it->second + "'.");
                    }
                }
                // If table is ad-hoc (not found in schema catalog), allow any column name without error
            }
        } else {
            // Unqualified column reference
            std::vector<std::string> matching_tables;
            std::set<std::string> unique_base_tables;
            bool has_ad_hoc_table = false;
            for (const auto& kv : scope) {
                unique_base_tables.insert(kv.second);
            }

            for (const auto& btable : unique_base_tables) {
                const TableMeta* meta = get_table(btable);
                if (meta) {
                    if (meta->has_column(col->column_name)) {
                        matching_tables.push_back(btable);
                    }
                } else {
                    has_ad_hoc_table = true;
                }
            }

            if (matching_tables.empty()) {
                if (!has_ad_hoc_table) {
                    res.valid = false;
                    res.errors.push_back("Column '" + col->column_name + "' does not exist in any referenced table.");
                }
            } else if (matching_tables.size() > 1) {
                res.valid = false;
                std::string err = "Ambiguous column reference '" + col->column_name + "' found in multiple tables: ";
                for (size_t i = 0; i < matching_tables.size(); ++i) {
                    if (i > 0) err += ", ";
                    err += matching_tables[i];
                }
                res.errors.push_back(err);
            }
        }
    } else if (auto bin = dynamic_cast<const BinaryExpr*>(expr)) {
        validate_expr(bin->left, scope, res);
        validate_expr(bin->right, scope, res);
    } else if (auto un = dynamic_cast<const UnaryExpr*>(expr)) {
        validate_expr(un->child, scope, res);
    } else if (auto like = dynamic_cast<const LikeExpr*>(expr)) {
        validate_expr(like->expr, scope, res);
        validate_expr(like->pattern, scope, res);
    } else if (auto in = dynamic_cast<const InExpr*>(expr)) {
        validate_expr(in->expr, scope, res);
        for (auto v : in->values) validate_expr(v, scope, res);
    } else if (auto btw = dynamic_cast<const BetweenExpr*>(expr)) {
        validate_expr(btw->expr, scope, res);
        validate_expr(btw->lower, scope, res);
        validate_expr(btw->upper, scope, res);
    } else if (auto agg = dynamic_cast<const AggregateExpr*>(expr)) {
        if (agg->arg) validate_expr(agg->arg, scope, res);
    } else if (auto star = dynamic_cast<const StarExpr*>(expr)) {
        if (!star->table_name.empty()) {
            if (scope.find(star->table_name) == scope.end()) {
                res.valid = false;
                res.errors.push_back("Unknown table or alias '" + star->table_name + "' in wildcard '" + star->to_string() + "'.");
            }
        }
    }
}

void SchemaCatalog::validate_select_stmt(const SelectStmtNode* stmt, ValidationResult& res) const {
    if (!stmt) return;

    std::unordered_map<std::string, std::string> scope;
    for (const auto* tbl : stmt->from_clause) {
        extract_table_refs(tbl, scope, res);
    }

    // Validate select items
    for (const auto* item : stmt->select_list) {
        if (item && item->expr) {
            validate_expr(item->expr, scope, res);
        }
    }

    // Validate where clause
    if (stmt->where_clause) {
        validate_expr(stmt->where_clause, scope, res);
    }

    // Validate group by clause
    for (const auto* grp : stmt->group_by_clause) {
        validate_expr(grp, scope, res);
    }

    // Validate having clause
    if (stmt->having_clause) {
        validate_expr(stmt->having_clause, scope, res);
    }

    // Validate order by clause
    for (const auto* ord : stmt->order_by_clause) {
        if (ord && ord->expr) {
            validate_expr(ord->expr, scope, res);
        }
    }
}

ValidationResult SchemaCatalog::validate_ast(const ASTNode* root) const {
    ValidationResult res;
    if (!root) return res;

    if (auto sel = dynamic_cast<const SelectStmtNode*>(root)) {
        validate_select_stmt(sel, res);
    } else if (auto set_op = dynamic_cast<const SetOpQueryNode*>(root)) {
        ValidationResult left_res = validate_ast(set_op->left);
        ValidationResult right_res = validate_ast(set_op->right);
        res.valid = left_res.valid && right_res.valid;
        res.errors.insert(res.errors.end(), left_res.errors.begin(), left_res.errors.end());
        res.errors.insert(res.errors.end(), right_res.errors.begin(), right_res.errors.end());
        res.warnings.insert(res.warnings.end(), left_res.warnings.begin(), left_res.warnings.end());
        res.warnings.insert(res.warnings.end(), right_res.warnings.end(), right_res.warnings.end());
    }

    return res;
}
