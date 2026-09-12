/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_TAB_HPP_INCLUDED
# define YY_YY_SRC_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 28 "src\\parser.y"

    #include "ast.h"
    #include <vector>
    #include <string>

#line 55 "src\\parser.tab.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    INT_LITERAL = 259,             /* INT_LITERAL  */
    FLOAT_LITERAL = 260,           /* FLOAT_LITERAL  */
    STRING_LITERAL = 261,          /* STRING_LITERAL  */
    SELECT = 262,                  /* SELECT  */
    FROM = 263,                    /* FROM  */
    WHERE = 264,                   /* WHERE  */
    DISTINCT = 265,                /* DISTINCT  */
    JOIN = 266,                    /* JOIN  */
    INNER = 267,                   /* INNER  */
    LEFT = 268,                    /* LEFT  */
    RIGHT = 269,                   /* RIGHT  */
    OUTER = 270,                   /* OUTER  */
    ON = 271,                      /* ON  */
    GROUP = 272,                   /* GROUP  */
    BY = 273,                      /* BY  */
    HAVING = 274,                  /* HAVING  */
    ORDER = 275,                   /* ORDER  */
    ASC = 276,                     /* ASC  */
    DESC = 277,                    /* DESC  */
    LIKE = 278,                    /* LIKE  */
    IN = 279,                      /* IN  */
    BETWEEN = 280,                 /* BETWEEN  */
    COUNT = 281,                   /* COUNT  */
    SUM = 282,                     /* SUM  */
    AVG = 283,                     /* AVG  */
    MIN = 284,                     /* MIN  */
    MAX = 285,                     /* MAX  */
    UNION = 286,                   /* UNION  */
    INTERSECT = 287,               /* INTERSECT  */
    EXCEPT = 288,                  /* EXCEPT  */
    MINUS = 289,                   /* MINUS  */
    ALL = 290,                     /* ALL  */
    AND = 291,                     /* AND  */
    OR = 292,                      /* OR  */
    NOT = 293,                     /* NOT  */
    AS = 294,                      /* AS  */
    TRUE_KW = 295,                 /* TRUE_KW  */
    FALSE_KW = 296,                /* FALSE_KW  */
    NULL_KW = 297,                 /* NULL_KW  */
    EQ = 298,                      /* EQ  */
    NEQ = 299,                     /* NEQ  */
    LT = 300,                      /* LT  */
    LTE = 301,                     /* LTE  */
    GT = 302,                      /* GT  */
    GTE = 303,                     /* GTE  */
    COMMA = 304,                   /* COMMA  */
    DOT = 305,                     /* DOT  */
    STAR = 306,                    /* STAR  */
    LPAREN = 307,                  /* LPAREN  */
    RPAREN = 308,                  /* RPAREN  */
    SEMICOLON = 309                /* SEMICOLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 37 "src\\parser.y"

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

#line 141 "src\\parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_TAB_HPP_INCLUDED  */
