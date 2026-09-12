#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "ast.h"
#include "schema.h"
#include "ra.h"
#include "optimizer.h"
#include "parser.tab.hpp"

// Flex/Bison symbols
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yyparse();
extern int yylex_destroy();

// Global states
ASTNode* g_root = nullptr;
ParseError g_parse_error;
extern std::vector<TokenRecord> g_tokens;
extern int yylineno;
extern int yycolumn;

int main(int argc, char* argv[]) {
    std::string sql_input;
    bool has_explicit_sql = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--schema") {
            std::cout << SchemaCatalog::instance().to_json() << std::endl;
            return 0;
        } else if (arg == "--schema-file" && i + 1 < argc) {
            std::string schema_path = argv[++i];
            SchemaCatalog::instance().load_from_file(schema_path);
        } else if (arg == "--schema-json" && i + 1 < argc) {
            std::string schema_json = argv[++i];
            SchemaCatalog::instance().load_from_json(schema_json);
        } else if (arg == "-c" && i + 1 < argc) {
            sql_input = argv[++i];
            has_explicit_sql = true;
        } else if (arg == "-c" && i + 1 == argc) {
            sql_input = "";
            has_explicit_sql = true;
        } else if (arg[0] != '-') {
            if (!has_explicit_sql) {
                sql_input = arg;
                has_explicit_sql = true;
            }
        }
    }

    if (!has_explicit_sql) {
        // Read from stdin until EOF
        std::ostringstream ss;
        ss << std::cin.rdbuf();
        sql_input = ss.str();
    }

    // Check if input is empty or whitespace only
    bool has_content = false;
    for (char c : sql_input) {
        if (!isspace((unsigned char)c)) {
            has_content = true;
            break;
        }
    }
    if (!has_content) {
        std::cout << "{\n"
                  << "  \"success\": false,\n"
                  << "  \"error\": {\n"
                  << "    \"message\": \"Empty SQL input.\",\n"
                  << "    \"line\": 1,\n"
                  << "    \"col\": 1\n"
                  << "  },\n"
                  << "  \"tokens\": []\n"
                  << "}\n";
        return 0;
    }

    // Reset globals
    g_root = nullptr;
    g_parse_error.has_error = false;
    g_parse_error.message = "";
    g_parse_error.line = 1;
    g_parse_error.column = 1;
    g_tokens.clear();
    yylineno = 1;
    yycolumn = 1;

    YY_BUFFER_STATE buffer = yy_scan_string(sql_input.c_str());
    int parse_res = yyparse();
    yy_delete_buffer(buffer);

    // Build JSON output
    std::ostringstream json;
    json << "{\n";

    if (parse_res != 0 || g_parse_error.has_error || g_root == nullptr) {
        json << "  \"success\": false,\n";
        json << "  \"error\": {\n";
        json << "    \"message\": \"" << escape_json(g_parse_error.message.empty() ? "Syntax error" : g_parse_error.message) << "\",\n";
        json << "    \"line\": " << g_parse_error.line << ",\n";
        json << "    \"col\": " << g_parse_error.column << "\n";
        json << "  },\n";
        json << "  \"tokens\": [\n";
        for (size_t i = 0; i < g_tokens.size(); ++i) {
            json << "    " << g_tokens[i].to_json() << (i + 1 < g_tokens.size() ? ",\n" : "\n");
        }
        json << "  ]\n";
        json << "}\n";

        std::cout << json.str();
        if (g_root) delete g_root;
        return 0;
    }

    // 1. Schema Validation
    ValidationResult val_res = SchemaCatalog::instance().validate_ast(g_root);

    // 2. Relational Algebra Translation
    std::shared_ptr<RANode> ra_root = RATranslator::translate(g_root);

    // 3. Relational Algebra Optimization
    std::shared_ptr<RANode> ra_opt = QueryOptimizer::optimize(ra_root);

    json << "  \"success\": true,\n";
    json << "  \"sql\": \"" << escape_json(sql_input) << "\",\n";
    json << "  \"tokens\": [\n";
    for (size_t i = 0; i < g_tokens.size(); ++i) {
        json << "    " << g_tokens[i].to_json() << (i + 1 < g_tokens.size() ? ",\n" : "\n");
    }
    json << "  ],\n";
    json << "  \"ast\": " << g_root->to_json() << ",\n";
    json << "  \"schema_validation\": " << val_res.to_json() << ",\n";
    if (ra_root) {
        json << "  \"ra_unoptimized\": " << ra_root->to_json() << ",\n";
        json << "  \"ra_string_unoptimized\": \"" << escape_json(ra_root->to_linear_string()) << "\",\n";
    } else {
        json << "  \"ra_unoptimized\": null,\n";
        json << "  \"ra_string_unoptimized\": \"\",\n";
    }
    if (ra_opt) {
        json << "  \"ra_optimized\": " << ra_opt->to_json() << ",\n";
        json << "  \"ra_string_optimized\": \"" << escape_json(ra_opt->to_linear_string()) << "\"\n";
    } else {
        json << "  \"ra_optimized\": null,\n";
        json << "  \"ra_string_optimized\": \"\"\n";
    }
    json << "}\n";

    std::cout << json.str();

    delete g_root;
    return 0;
}
