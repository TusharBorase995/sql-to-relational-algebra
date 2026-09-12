#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "ast.h"
#include "json_helper.h"

struct ColumnMeta {
    std::string name;
    std::string type;
    bool is_primary_key = false;
    bool is_foreign_key = false;
    std::string fk_target_table;
    std::string fk_target_column;
};

struct TableMeta {
    std::string name;
    std::vector<ColumnMeta> columns;

    bool has_column(const std::string& col) const {
        for (const auto& c : columns) {
            if (c.name == col) return true;
        }
        return false;
    }

    const ColumnMeta* get_column(const std::string& col) const {
        for (const auto& c : columns) {
            if (c.name == col) return &c;
        }
        return nullptr;
    }
};

struct ValidationResult {
    bool valid = true;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    std::string to_json() const {
        std::ostringstream ss;
        ss << "{\"valid\":" << (valid ? "true" : "false") << ","
           << "\"errors\":[";
        for (size_t i = 0; i < errors.size(); ++i) {
            if (i > 0) ss << ",";
            ss << "\"" << escape_json(errors[i]) << "\"";
        }
        ss << "],\"warnings\":[";
        for (size_t i = 0; i < warnings.size(); ++i) {
            if (i > 0) ss << ",";
            ss << "\"" << escape_json(warnings[i]) << "\"";
        }
        ss << "]}";
        return ss.str();
    }
};

class SchemaCatalog {
public:
    static SchemaCatalog& instance();

    const std::unordered_map<std::string, TableMeta>& get_tables() const {
        return tables_;
    }

    bool has_table(const std::string& tbl) const {
        return tables_.find(tbl) != tables_.end();
    }

    const TableMeta* get_table(const std::string& tbl) const {
        auto it = tables_.find(tbl);
        if (it != tables_.end()) return &it->second;
        return nullptr;
    }

    void reset_to_defaults();
    void clear();
    void add_table(const TableMeta& table);
    bool load_from_json(const std::string& json_str);
    bool load_from_file(const std::string& filepath);

    bool is_lenient() const { return lenient_mode_; }
    void set_lenient(bool l) { lenient_mode_ = l; }

    ValidationResult validate_ast(const ASTNode* root) const;
    std::string to_json() const;

private:
    SchemaCatalog();
    std::unordered_map<std::string, TableMeta> tables_;
    bool lenient_mode_ = true;

    void validate_select_stmt(const SelectStmtNode* stmt, ValidationResult& res) const;
    void extract_table_refs(const TableRef* ref, std::unordered_map<std::string, std::string>& scope, ValidationResult& res) const;
    void validate_expr(const ExprNode* expr, const std::unordered_map<std::string, std::string>& scope, ValidationResult& res) const;
};

#endif // SCHEMA_H
