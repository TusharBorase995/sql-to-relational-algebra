%{
#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#ifndef fileno
#define fileno _fileno
#endif
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "ast.h"
#include "parser.tab.hpp"

// Global token list and parse error
std::vector<TokenRecord> g_tokens;
extern ParseError g_parse_error;

int yycolumn = 1;

static inline char* my_strdup(const char* s) {
    if (!s) return nullptr;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) {
        memcpy(copy, s, len + 1);
    }
    return copy;
}

#define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_line = yylineno; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

static int record_tok(int token_type, const char* name, const char* lex) {
    TokenRecord tr;
    tr.type = name;
    tr.lexeme = lex;
    tr.line = yylloc.first_line;
    tr.column = yylloc.first_column;
    g_tokens.push_back(tr);
    return token_type;
}
%}

%option noyywrap
%option yylineno
%option case-insensitive
%option nounistd

%%

[ \t]+          { /* whitespace: ignore */ }
\r              { /* carriage return: ignore */ }
\n              { yycolumn = 1; }

"--".*          {
                    // Single line comment
                    record_tok(0, "COMMENT", yytext);
                }

"/*"([^*]|\*+[^*/])*\*+"/" {
                    // Multi-line comment
                    record_tok(0, "COMMENT", yytext);
                    // Adjust column for internal newlines
                    for (int i = 0; i < yyleng; ++i) {
                        if (yytext[i] == '\n') {
                            yycolumn = 1;
                        }
                    }
                }

"SELECT"        { return record_tok(SELECT, "SELECT", yytext); }
"FROM"          { return record_tok(FROM, "FROM", yytext); }
"WHERE"         { return record_tok(WHERE, "WHERE", yytext); }
"DISTINCT"      { return record_tok(DISTINCT, "DISTINCT", yytext); }
"JOIN"          { return record_tok(JOIN, "JOIN", yytext); }
"INNER"         { return record_tok(INNER, "INNER", yytext); }
"LEFT"          { return record_tok(LEFT, "LEFT", yytext); }
"RIGHT"         { return record_tok(RIGHT, "RIGHT", yytext); }
"OUTER"         { return record_tok(OUTER, "OUTER", yytext); }
"ON"            { return record_tok(ON, "ON", yytext); }
"GROUP"         { return record_tok(GROUP, "GROUP", yytext); }
"BY"            { return record_tok(BY, "BY", yytext); }
"HAVING"        { return record_tok(HAVING, "HAVING", yytext); }
"ORDER"         { return record_tok(ORDER, "ORDER", yytext); }
"ASC"           { return record_tok(ASC, "ASC", yytext); }
"DESC"          { return record_tok(DESC, "DESC", yytext); }
"LIKE"          { return record_tok(LIKE, "LIKE", yytext); }
"IN"            { return record_tok(IN, "IN", yytext); }
"BETWEEN"       { return record_tok(BETWEEN, "BETWEEN", yytext); }
"COUNT"         { return record_tok(COUNT, "COUNT", yytext); }
"SUM"           { return record_tok(SUM, "SUM", yytext); }
"AVG"           { return record_tok(AVG, "AVG", yytext); }
"MIN"           { return record_tok(MIN, "MIN", yytext); }
"MAX"           { return record_tok(MAX, "MAX", yytext); }
"UNION"         { return record_tok(UNION, "UNION", yytext); }
"INTERSECT"     { return record_tok(INTERSECT, "INTERSECT", yytext); }
"EXCEPT"        { return record_tok(EXCEPT, "EXCEPT", yytext); }
"MINUS"         { return record_tok(MINUS, "MINUS", yytext); }
"ALL"           { return record_tok(ALL, "ALL", yytext); }
"AND"           { return record_tok(AND, "AND", yytext); }
"OR"            { return record_tok(OR, "OR", yytext); }
"NOT"           { return record_tok(NOT, "NOT", yytext); }
"AS"            { return record_tok(AS, "AS", yytext); }
"TRUE"          { return record_tok(TRUE_KW, "TRUE", yytext); }
"FALSE"         { return record_tok(FALSE_KW, "FALSE", yytext); }
"NULL"          { return record_tok(NULL_KW, "NULL", yytext); }

"="             { return record_tok(EQ, "EQ", yytext); }
"<>"            { return record_tok(NEQ, "NEQ", yytext); }
"!="            { return record_tok(NEQ, "NEQ", yytext); }
"<="            { return record_tok(LTE, "LTE", yytext); }
">="            { return record_tok(GTE, "GTE", yytext); }
"<"             { return record_tok(LT, "LT", yytext); }
">"             { return record_tok(GT, "GT", yytext); }
"*"             { return record_tok(STAR, "STAR", yytext); }
"."             { return record_tok(DOT, "DOT", yytext); }
","             { return record_tok(COMMA, "COMMA", yytext); }
"("             { return record_tok(LPAREN, "LPAREN", yytext); }
")"             { return record_tok(RPAREN, "RPAREN", yytext); }
";"             { return record_tok(SEMICOLON, "SEMICOLON", yytext); }

[0-9]+"."[0-9]+ {
                    yylval.str_val = my_strdup(yytext);
                    return record_tok(FLOAT_LITERAL, "FLOAT_LITERAL", yytext);
                }

[0-9]+          {
                    yylval.str_val = my_strdup(yytext);
                    return record_tok(INT_LITERAL, "INT_LITERAL", yytext);
                }

'([^'\\]|\\.)*' {
                    // Extract content without single quotes
                    std::string raw(yytext + 1, yyleng - 2);
                    yylval.str_val = my_strdup(raw.c_str());
                    return record_tok(STRING_LITERAL, "STRING_LITERAL", yytext);
                }

[a-zA-Z_][a-zA-Z0-9_]* {
                    yylval.str_val = my_strdup(yytext);
                    return record_tok(IDENTIFIER, "IDENTIFIER", yytext);
                }

.               {
                    // Unexpected character
                    char msg[64];
                    snprintf(msg, sizeof(msg), "Unrecognized character '%s'", yytext);
                    g_parse_error.has_error = true;
                    g_parse_error.message = msg;
                    g_parse_error.line = yylineno;
                    g_parse_error.column = yycolumn - yyleng;
                    return record_tok(0, "UNKNOWN", yytext);
                }

%%
