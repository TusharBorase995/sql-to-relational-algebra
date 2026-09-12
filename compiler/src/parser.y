%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "ast.h"

// Prototypes
int yylex(void);
void yyerror(const char* s);

static inline char* my_strdup(const char* s) {
    if (!s) return nullptr;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) {
        memcpy(copy, s, len + 1);
    }
    return copy;
}

// Global AST root and error state
extern ASTNode* g_root;
extern ParseError g_parse_error;
%}

%code requires {
    #include "ast.h"
    #include <vector>
    #include <string>
}

%locations
%define parse.error verbose

%union {
    int int_val;
    char* str_val;
    ASTNode* ast_node;
    SelectStmtNode* select_stmt;
    SelectItem* select_item;
    std::vector<SelectItem*>* select_list;
    TableRef* table_ref;
    std::vector<TableRef*>* table_list;
    ExprNode* expr_node;
    std::vector<ExprNode*>* expr_list;
    OrderByItem* order_item;
    std::vector<OrderByItem*>* order_list;
}

/* Tokens */
%token <str_val> IDENTIFIER
%token <str_val> INT_LITERAL
%token <str_val> FLOAT_LITERAL
%token <str_val> STRING_LITERAL

/* Keywords for Tier 1 & Tier 2 & Tier 3 */
%token SELECT FROM WHERE DISTINCT JOIN INNER LEFT RIGHT OUTER ON
%token GROUP BY HAVING ORDER ASC DESC
%token LIKE IN BETWEEN
%token COUNT SUM AVG MIN MAX
%token UNION INTERSECT EXCEPT MINUS ALL
%token AND OR NOT AS
%token TRUE_KW FALSE_KW NULL_KW

/* Operators and Punctuation */
%token EQ NEQ LT LTE GT GTE
%token COMMA DOT STAR LPAREN RPAREN
%token SEMICOLON

/* Operator Precedence */
%left UNION EXCEPT MINUS
%left INTERSECT
%left OR
%left AND
%right NOT
%nonassoc EQ NEQ LT LTE GT GTE LIKE IN
%nonassoc BETWEEN

/* Non-terminal types */
%type <ast_node> query set_query
%type <select_stmt> select_stmt
%type <int_val> opt_distinct opt_asc_desc opt_all
%type <select_list> select_list
%type <select_item> select_item
%type <str_val> opt_alias join_type
%type <table_list> from_clause table_ref_list
%type <table_ref> table_ref table_atom
%type <expr_node> opt_where opt_having expr bit_expr column_ref literal aggregate_expr
%type <expr_list> opt_group_by expr_list in_expr_list
%type <order_list> opt_order_by order_list
%type <order_item> order_item

%start query

%%

query:
    set_query opt_semicolon {
        g_root = $1;
        $$ = $1;
    }
;

opt_semicolon:
    /* empty */
  | SEMICOLON
;

set_query:
    set_query UNION opt_all set_query %prec UNION {
        $$ = new SetOpQueryNode($3 ? "UNION ALL" : "UNION", $1, $4);
    }
  | set_query INTERSECT opt_all set_query %prec INTERSECT {
        $$ = new SetOpQueryNode($3 ? "INTERSECT ALL" : "INTERSECT", $1, $4);
    }
  | set_query EXCEPT opt_all set_query %prec EXCEPT {
        $$ = new SetOpQueryNode($3 ? "EXCEPT ALL" : "EXCEPT", $1, $4);
    }
  | set_query MINUS opt_all set_query %prec EXCEPT {
        $$ = new SetOpQueryNode($3 ? "EXCEPT ALL" : "EXCEPT", $1, $4);
    }
  | select_stmt {
        $$ = $1;
    }
  | LPAREN set_query RPAREN {
        $$ = $2;
    }
;

opt_all:
    /* empty */ { $$ = 0; }
  | ALL         { $$ = 1; }
  | DISTINCT    { $$ = 0; }
;

select_stmt:
    SELECT opt_distinct select_list FROM from_clause opt_where opt_group_by opt_having opt_order_by {
        auto* stmt = new SelectStmtNode();
        stmt->is_distinct = ($2 != 0);
        if ($3) {
            stmt->select_list = *$3;
            delete $3;
        }
        if ($5) {
            stmt->from_clause = *$5;
            delete $5;
        }
        stmt->where_clause = $6;
        if ($7) {
            stmt->group_by_clause = *$7;
            delete $7;
        }
        stmt->having_clause = $8;
        if ($9) {
            stmt->order_by_clause = *$9;
            delete $9;
        }
        $$ = stmt;
    }
;

opt_distinct:
    /* empty */ { $$ = 0; }
  | DISTINCT    { $$ = 1; }
;

select_list:
    select_item {
        auto* list = new std::vector<SelectItem*>();
        list->push_back($1);
        $$ = list;
    }
  | select_list COMMA select_item {
        $1->push_back($3);
        $$ = $1;
    }
;

select_item:
    STAR {
        $$ = new SelectItem(new StarExpr(""));
    }
  | IDENTIFIER DOT STAR {
        $$ = new SelectItem(new StarExpr($1));
        free($1);
    }
  | expr opt_alias {
        $$ = new SelectItem($1, $2 ? $2 : "");
        if ($2) free($2);
    }
;

opt_alias:
    /* empty */ { $$ = nullptr; }
  | AS IDENTIFIER { $$ = $2; }
  | IDENTIFIER    { $$ = $1; }
;

from_clause:
    table_ref_list {
        $$ = $1;
    }
;

table_ref_list:
    table_ref {
        auto* list = new std::vector<TableRef*>();
        list->push_back($1);
        $$ = list;
    }
  | table_ref_list COMMA table_ref {
        $1->push_back($3);
        $$ = $1;
    }
;

table_ref:
    table_atom {
        $$ = $1;
    }
  | table_ref join_type JOIN table_atom ON expr {
        $$ = new TableRef($2, $1, $4, $6);
        free($2);
    }
  | table_ref JOIN table_atom ON expr {
        $$ = new TableRef("INNER", $1, $3, $5);
    }
;

join_type:
    INNER       { $$ = my_strdup("INNER"); }
  | LEFT        { $$ = my_strdup("LEFT"); }
  | LEFT OUTER  { $$ = my_strdup("LEFT"); }
  | RIGHT       { $$ = my_strdup("RIGHT"); }
  | RIGHT OUTER { $$ = my_strdup("RIGHT"); }
;

table_atom:
    IDENTIFIER opt_alias {
        $$ = new TableRef($1, $2 ? $2 : "");
        free($1);
        if ($2) free($2);
    }
  | LPAREN table_ref RPAREN {
        $$ = $2;
    }
;

opt_where:
    /* empty */ { $$ = nullptr; }
  | WHERE expr  { $$ = $2; }
;

opt_group_by:
    /* empty */ { $$ = nullptr; }
  | GROUP BY expr_list { $$ = $3; }
;

opt_having:
    /* empty */ { $$ = nullptr; }
  | HAVING expr { $$ = $2; }
;

opt_order_by:
    /* empty */ { $$ = nullptr; }
  | ORDER BY order_list { $$ = $3; }
;

order_list:
    order_item {
        auto* list = new std::vector<OrderByItem*>();
        list->push_back($1);
        $$ = list;
    }
  | order_list COMMA order_item {
        $1->push_back($3);
        $$ = $1;
    }
;

order_item:
    expr opt_asc_desc {
        $$ = new OrderByItem($1, ($2 != 0));
    }
;

opt_asc_desc:
    /* empty */ { $$ = 0; }
  | ASC         { $$ = 0; }
  | DESC        { $$ = 1; }
;

expr_list:
    expr {
        auto* list = new std::vector<ExprNode*>();
        list->push_back($1);
        $$ = list;
    }
  | expr_list COMMA expr {
        $1->push_back($3);
        $$ = $1;
    }
;

in_expr_list:
    expr {
        auto* list = new std::vector<ExprNode*>();
        list->push_back($1);
        $$ = list;
    }
  | in_expr_list COMMA expr {
        $1->push_back($3);
        $$ = $1;
    }
;

expr:
    expr OR expr {
        $$ = new BinaryExpr("OR", $1, $3);
    }
  | expr AND expr {
        $$ = new BinaryExpr("AND", $1, $3);
    }
  | NOT expr {
        $$ = new UnaryExpr("NOT", $2);
    }
  | expr LIKE expr {
        $$ = new LikeExpr($1, $3, false);
    }
  | expr NOT LIKE expr {
        $$ = new LikeExpr($1, $4, true);
    }
  | expr IN LPAREN in_expr_list RPAREN {
        $$ = new InExpr($1, *$4, false);
        delete $4;
    }
  | expr NOT IN LPAREN in_expr_list RPAREN {
        $$ = new InExpr($1, *$5, true);
        delete $5;
    }
  | expr BETWEEN bit_expr AND bit_expr %prec BETWEEN {
        $$ = new BetweenExpr($1, $3, $5, false);
    }
  | expr NOT BETWEEN bit_expr AND bit_expr %prec BETWEEN {
        $$ = new BetweenExpr($1, $4, $6, true);
    }
  | bit_expr {
        $$ = $1;
    }
;

bit_expr:
    bit_expr EQ bit_expr {
        $$ = new BinaryExpr("=", $1, $3);
    }
  | bit_expr NEQ bit_expr {
        $$ = new BinaryExpr("<>", $1, $3);
    }
  | bit_expr LT bit_expr {
        $$ = new BinaryExpr("<", $1, $3);
    }
  | bit_expr LTE bit_expr {
        $$ = new BinaryExpr("<=", $1, $3);
    }
  | bit_expr GT bit_expr {
        $$ = new BinaryExpr(">", $1, $3);
    }
  | bit_expr GTE bit_expr {
        $$ = new BinaryExpr(">=", $1, $3);
    }
  | LPAREN expr RPAREN {
        $$ = $2;
    }
  | column_ref {
        $$ = $1;
    }
  | literal {
        $$ = $1;
    }
  | aggregate_expr {
        $$ = $1;
    }
;

aggregate_expr:
    COUNT LPAREN STAR RPAREN {
        $$ = new AggregateExpr("COUNT", false, new StarExpr(""));
    }
  | COUNT LPAREN DISTINCT expr RPAREN {
        $$ = new AggregateExpr("COUNT", true, $4);
    }
  | COUNT LPAREN expr RPAREN {
        $$ = new AggregateExpr("COUNT", false, $3);
    }
  | SUM LPAREN DISTINCT expr RPAREN {
        $$ = new AggregateExpr("SUM", true, $4);
    }
  | SUM LPAREN expr RPAREN {
        $$ = new AggregateExpr("SUM", false, $3);
    }
  | AVG LPAREN DISTINCT expr RPAREN {
        $$ = new AggregateExpr("AVG", true, $4);
    }
  | AVG LPAREN expr RPAREN {
        $$ = new AggregateExpr("AVG", false, $3);
    }
  | MIN LPAREN expr RPAREN {
        $$ = new AggregateExpr("MIN", false, $3);
    }
  | MAX LPAREN expr RPAREN {
        $$ = new AggregateExpr("MAX", false, $3);
    }
;

column_ref:
    IDENTIFIER {
        $$ = new ColumnRefExpr($1);
        free($1);
    }
  | IDENTIFIER DOT IDENTIFIER {
        $$ = new ColumnRefExpr($1, $3);
        free($1);
        free($3);
    }
;

literal:
    INT_LITERAL {
        $$ = new LiteralExpr(LiteralExpr::LiteralType::INT, $1);
        free($1);
    }
  | FLOAT_LITERAL {
        $$ = new LiteralExpr(LiteralExpr::LiteralType::FLOAT, $1);
        free($1);
    }
  | STRING_LITERAL {
        $$ = new LiteralExpr(LiteralExpr::LiteralType::STRING, $1);
        free($1);
    }
  | TRUE_KW {
        $$ = new LiteralExpr(LiteralExpr::LiteralType::BOOLEAN, "TRUE");
    }
  | FALSE_KW {
        $$ = new LiteralExpr(LiteralExpr::LiteralType::BOOLEAN, "FALSE");
    }
  | NULL_KW {
        $$ = new LiteralExpr(LiteralExpr::LiteralType::NULL_VALUE, "NULL");
    }
;

%%

void yyerror(const char* s) {
    g_parse_error.has_error = true;
    g_parse_error.message = s;
    g_parse_error.line = yylloc.first_line;
    g_parse_error.column = yylloc.first_column;
}
