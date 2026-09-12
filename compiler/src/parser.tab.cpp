/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src\\parser.y"

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

#line 98 "src\\parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 4,                /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 5,              /* FLOAT_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_SELECT = 7,                     /* SELECT  */
  YYSYMBOL_FROM = 8,                       /* FROM  */
  YYSYMBOL_WHERE = 9,                      /* WHERE  */
  YYSYMBOL_DISTINCT = 10,                  /* DISTINCT  */
  YYSYMBOL_JOIN = 11,                      /* JOIN  */
  YYSYMBOL_INNER = 12,                     /* INNER  */
  YYSYMBOL_LEFT = 13,                      /* LEFT  */
  YYSYMBOL_RIGHT = 14,                     /* RIGHT  */
  YYSYMBOL_OUTER = 15,                     /* OUTER  */
  YYSYMBOL_ON = 16,                        /* ON  */
  YYSYMBOL_GROUP = 17,                     /* GROUP  */
  YYSYMBOL_BY = 18,                        /* BY  */
  YYSYMBOL_HAVING = 19,                    /* HAVING  */
  YYSYMBOL_ORDER = 20,                     /* ORDER  */
  YYSYMBOL_ASC = 21,                       /* ASC  */
  YYSYMBOL_DESC = 22,                      /* DESC  */
  YYSYMBOL_LIKE = 23,                      /* LIKE  */
  YYSYMBOL_IN = 24,                        /* IN  */
  YYSYMBOL_BETWEEN = 25,                   /* BETWEEN  */
  YYSYMBOL_COUNT = 26,                     /* COUNT  */
  YYSYMBOL_SUM = 27,                       /* SUM  */
  YYSYMBOL_AVG = 28,                       /* AVG  */
  YYSYMBOL_MIN = 29,                       /* MIN  */
  YYSYMBOL_MAX = 30,                       /* MAX  */
  YYSYMBOL_UNION = 31,                     /* UNION  */
  YYSYMBOL_INTERSECT = 32,                 /* INTERSECT  */
  YYSYMBOL_EXCEPT = 33,                    /* EXCEPT  */
  YYSYMBOL_MINUS = 34,                     /* MINUS  */
  YYSYMBOL_ALL = 35,                       /* ALL  */
  YYSYMBOL_AND = 36,                       /* AND  */
  YYSYMBOL_OR = 37,                        /* OR  */
  YYSYMBOL_NOT = 38,                       /* NOT  */
  YYSYMBOL_AS = 39,                        /* AS  */
  YYSYMBOL_TRUE_KW = 40,                   /* TRUE_KW  */
  YYSYMBOL_FALSE_KW = 41,                  /* FALSE_KW  */
  YYSYMBOL_NULL_KW = 42,                   /* NULL_KW  */
  YYSYMBOL_EQ = 43,                        /* EQ  */
  YYSYMBOL_NEQ = 44,                       /* NEQ  */
  YYSYMBOL_LT = 45,                        /* LT  */
  YYSYMBOL_LTE = 46,                       /* LTE  */
  YYSYMBOL_GT = 47,                        /* GT  */
  YYSYMBOL_GTE = 48,                       /* GTE  */
  YYSYMBOL_COMMA = 49,                     /* COMMA  */
  YYSYMBOL_DOT = 50,                       /* DOT  */
  YYSYMBOL_STAR = 51,                      /* STAR  */
  YYSYMBOL_LPAREN = 52,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 53,                    /* RPAREN  */
  YYSYMBOL_SEMICOLON = 54,                 /* SEMICOLON  */
  YYSYMBOL_YYACCEPT = 55,                  /* $accept  */
  YYSYMBOL_query = 56,                     /* query  */
  YYSYMBOL_opt_semicolon = 57,             /* opt_semicolon  */
  YYSYMBOL_set_query = 58,                 /* set_query  */
  YYSYMBOL_opt_all = 59,                   /* opt_all  */
  YYSYMBOL_select_stmt = 60,               /* select_stmt  */
  YYSYMBOL_opt_distinct = 61,              /* opt_distinct  */
  YYSYMBOL_select_list = 62,               /* select_list  */
  YYSYMBOL_select_item = 63,               /* select_item  */
  YYSYMBOL_opt_alias = 64,                 /* opt_alias  */
  YYSYMBOL_from_clause = 65,               /* from_clause  */
  YYSYMBOL_table_ref_list = 66,            /* table_ref_list  */
  YYSYMBOL_table_ref = 67,                 /* table_ref  */
  YYSYMBOL_join_type = 68,                 /* join_type  */
  YYSYMBOL_table_atom = 69,                /* table_atom  */
  YYSYMBOL_opt_where = 70,                 /* opt_where  */
  YYSYMBOL_opt_group_by = 71,              /* opt_group_by  */
  YYSYMBOL_opt_having = 72,                /* opt_having  */
  YYSYMBOL_opt_order_by = 73,              /* opt_order_by  */
  YYSYMBOL_order_list = 74,                /* order_list  */
  YYSYMBOL_order_item = 75,                /* order_item  */
  YYSYMBOL_opt_asc_desc = 76,              /* opt_asc_desc  */
  YYSYMBOL_expr_list = 77,                 /* expr_list  */
  YYSYMBOL_in_expr_list = 78,              /* in_expr_list  */
  YYSYMBOL_expr = 79,                      /* expr  */
  YYSYMBOL_bit_expr = 80,                  /* bit_expr  */
  YYSYMBOL_aggregate_expr = 81,            /* aggregate_expr  */
  YYSYMBOL_column_ref = 82,                /* column_ref  */
  YYSYMBOL_literal = 83                    /* literal  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   366

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  179

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   100,   100,   106,   108,   112,   115,   118,   121,   124,
     127,   133,   134,   135,   139,   165,   166,   170,   175,   182,
     185,   189,   196,   197,   198,   202,   208,   213,   220,   223,
     227,   233,   234,   235,   236,   237,   241,   246,   252,   253,
     257,   258,   262,   263,   267,   268,   272,   277,   284,   290,
     291,   292,   296,   301,   308,   313,   320,   323,   326,   329,
     332,   335,   339,   343,   346,   349,   355,   358,   361,   364,
     367,   370,   373,   376,   379,   382,   388,   391,   394,   397,
     400,   403,   406,   409,   412,   418,   422,   430,   434,   438,
     442,   445,   448
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "INT_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", "SELECT", "FROM",
  "WHERE", "DISTINCT", "JOIN", "INNER", "LEFT", "RIGHT", "OUTER", "ON",
  "GROUP", "BY", "HAVING", "ORDER", "ASC", "DESC", "LIKE", "IN", "BETWEEN",
  "COUNT", "SUM", "AVG", "MIN", "MAX", "UNION", "INTERSECT", "EXCEPT",
  "MINUS", "ALL", "AND", "OR", "NOT", "AS", "TRUE_KW", "FALSE_KW",
  "NULL_KW", "EQ", "NEQ", "LT", "LTE", "GT", "GTE", "COMMA", "DOT", "STAR",
  "LPAREN", "RPAREN", "SEMICOLON", "$accept", "query", "opt_semicolon",
  "set_query", "opt_all", "select_stmt", "opt_distinct", "select_list",
  "select_item", "opt_alias", "from_clause", "table_ref_list", "table_ref",
  "join_type", "table_atom", "opt_where", "opt_group_by", "opt_having",
  "opt_order_by", "order_list", "order_item", "opt_asc_desc", "expr_list",
  "in_expr_list", "expr", "bit_expr", "aggregate_expr", "column_ref",
  "literal", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -4,     4,    -4,    20,   131,   -94,   -94,   106,    92,   -94,
      24,    24,    24,    24,   -94,   -94,   -13,   -94,   -94,   -94,
      -9,    -7,    -5,    -2,     8,    77,   -94,   -94,   -94,   -94,
      77,     7,   -94,   190,   151,   -94,   -94,   -94,   -94,   -94,
     -94,    -4,    -4,    -4,    -4,     3,    36,   149,   178,    77,
      77,    11,   148,    61,    -1,   106,   -94,    77,    27,   206,
      77,    77,   114,    69,   -94,   206,   206,   206,   206,   206,
     206,    62,   -94,    62,    62,   -94,   -94,    77,   -26,   214,
      77,   217,    77,   236,   239,   257,   110,   -94,    10,    -1,
      86,    67,    78,   -94,   -94,   143,    77,   306,   148,   305,
      77,    68,   206,   -94,   318,   318,   318,   318,   318,   318,
     260,   -94,   -94,   263,   -94,   281,   -94,   -94,   -94,   -94,
       5,    77,   104,    -1,    -1,   -94,   116,   125,   139,   -23,
     302,   206,   143,    77,   312,   -94,   -94,   -94,   -94,   302,
     133,   137,    78,   144,   -94,   -94,    -1,    77,   -94,   151,
     -20,   206,    77,    77,   141,    77,   153,   302,   -94,   151,
     121,   302,   302,   156,   -94,   302,    77,    77,    77,   302,
     302,   154,   -94,   299,    77,   -94,   -94,   -94,   -94
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    15,     0,     0,     3,     9,    16,     0,     0,     1,
      11,    11,    11,    11,     4,     2,    85,    87,    88,    89,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    19,
       0,     0,    17,    22,    65,    75,    73,    74,    10,    13,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    58,     0,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,    86,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    72,    22,     0,
      38,    25,    26,    28,    18,    59,     0,     0,    57,    56,
       0,     0,     0,    23,    66,    67,    68,    69,    70,    71,
       0,    76,    78,     0,    80,     0,    82,    83,    84,    36,
       0,     0,    40,     0,     0,    31,    32,    34,     0,     0,
      54,     0,    60,     0,     0,    77,    79,    81,    37,    39,
       0,    42,    27,     0,    33,    35,     0,     0,    61,    63,
       0,     0,     0,     0,    44,     0,     0,    55,    62,    64,
      41,    52,    43,     0,    14,    30,     0,     0,     0,    29,
      53,    45,    46,    49,     0,    50,    51,    48,    47
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -94,   -94,   -94,    26,    89,   -94,   -94,   -94,   145,   129,
     -94,   -94,   -85,   -94,   -93,   -94,   -94,   -94,   -94,   -94,
       6,   -94,   -94,    59,   -25,   -58,   -94,   -94,   -94
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,    15,     4,    41,     5,     7,    31,    32,    64,
      90,    91,    92,   128,    93,   122,   141,   154,   164,   171,
     172,   177,   160,   129,    33,    34,    35,    36,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,    97,    88,     1,   120,    53,    75,   104,   105,   106,
     107,   108,   109,    56,     6,    54,   124,   125,   126,   127,
       9,    79,    81,    83,    84,    85,   147,   111,     8,   147,
     148,   143,    95,   158,    39,    98,    99,    45,   142,    51,
      17,    18,    19,    46,   134,    47,    77,    48,     2,    63,
      49,    89,   110,   156,    76,   113,    55,   115,   138,    40,
      50,    86,    20,    21,    22,    23,    24,    71,    72,    73,
      74,   130,   103,   149,    25,   132,    26,    27,    28,    96,
      51,    17,    18,    19,    57,    58,    59,    78,    30,   124,
     125,   126,   127,   159,    11,   121,   139,    60,    61,    62,
      42,    43,    44,    20,    21,    22,    23,    24,   130,    16,
      17,    18,    19,    75,    87,    25,   123,    26,    27,    28,
     133,   140,   157,    10,    11,    12,    13,   161,   162,    30,
     165,   144,    20,    21,    22,    23,    24,   100,   101,   102,
     145,   169,   170,   173,    25,    38,    26,    27,    28,   173,
     146,   152,    51,    17,    18,    19,   153,    29,    30,    80,
     155,   163,    10,    11,    12,    13,    -1,    -1,    59,   166,
     167,    57,    58,    59,   168,    20,    21,    22,    23,    24,
     178,    51,    17,    18,    19,    14,    62,    25,    82,    26,
      27,    28,   150,    56,    65,    66,    67,    68,    69,    70,
      94,    30,     0,   174,    20,    21,    22,    23,    24,    51,
      17,    18,    19,    57,    58,    59,    25,   119,    26,    27,
      28,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      30,     0,    20,    21,    22,    23,    24,    57,    58,    59,
      57,    58,    59,     0,     0,     0,    26,    27,    28,     0,
      60,    61,    62,    60,    61,    62,     0,     0,    30,    57,
      58,    59,    57,    58,    59,     0,     0,   112,     0,     0,
     114,     0,    60,    61,    62,    60,    61,    62,     0,     0,
      57,    58,    59,    57,    58,    59,    57,    58,    59,   116,
       0,     0,   117,    60,    61,    62,    60,    61,    62,    60,
      61,    62,     0,     0,    57,    58,    59,     0,     0,     0,
     118,     0,     0,   135,     0,     0,   136,    60,    61,    62,
     175,   176,    57,    58,    59,    57,    58,    59,    57,    58,
      59,     0,     0,     0,   137,    60,    61,    62,    60,    61,
      62,    60,   131,    62,     0,     0,     0,     0,   151,    65,
      66,    67,    68,    69,    70,    65,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
      25,    59,     3,     7,    89,    30,     3,    65,    66,    67,
      68,    69,    70,     3,    10,     8,    11,    12,    13,    14,
       0,    46,    47,    48,    49,    50,    49,    53,     2,    49,
      53,   124,    57,    53,    10,    60,    61,    50,   123,     3,
       4,     5,     6,    52,   102,    52,    10,    52,    52,    39,
      52,    52,    77,   146,    51,    80,    49,    82,    53,    35,
      52,    50,    26,    27,    28,    29,    30,    41,    42,    43,
      44,    96,     3,   131,    38,   100,    40,    41,    42,    52,
       3,     4,     5,     6,    23,    24,    25,    51,    52,    11,
      12,    13,    14,   151,    32,     9,   121,    36,    37,    38,
      11,    12,    13,    26,    27,    28,    29,    30,   133,     3,
       4,     5,     6,     3,    53,    38,    49,    40,    41,    42,
      52,    17,   147,    31,    32,    33,    34,   152,   153,    52,
     155,    15,    26,    27,    28,    29,    30,    23,    24,    25,
      15,   166,   167,   168,    38,    53,    40,    41,    42,   174,
      11,    18,     3,     4,     5,     6,    19,    51,    52,    10,
      16,    20,    31,    32,    33,    34,    23,    24,    25,    16,
      49,    23,    24,    25,    18,    26,    27,    28,    29,    30,
     174,     3,     4,     5,     6,    54,    38,    38,    10,    40,
      41,    42,   133,     3,    43,    44,    45,    46,    47,    48,
      55,    52,    -1,    49,    26,    27,    28,    29,    30,     3,
       4,     5,     6,    23,    24,    25,    38,    88,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,
      52,    -1,    26,    27,    28,    29,    30,    23,    24,    25,
      23,    24,    25,    -1,    -1,    -1,    40,    41,    42,    -1,
      36,    37,    38,    36,    37,    38,    -1,    -1,    52,    23,
      24,    25,    23,    24,    25,    -1,    -1,    53,    -1,    -1,
      53,    -1,    36,    37,    38,    36,    37,    38,    -1,    -1,
      23,    24,    25,    23,    24,    25,    23,    24,    25,    53,
      -1,    -1,    53,    36,    37,    38,    36,    37,    38,    36,
      37,    38,    -1,    -1,    23,    24,    25,    -1,    -1,    -1,
      53,    -1,    -1,    53,    -1,    -1,    53,    36,    37,    38,
      21,    22,    23,    24,    25,    23,    24,    25,    23,    24,
      25,    -1,    -1,    -1,    53,    36,    37,    38,    36,    37,
      38,    36,    36,    38,    -1,    -1,    -1,    -1,    36,    43,
      44,    45,    46,    47,    48,    43,    44,    45,    46,    47,
      48,    43,    44,    45,    46,    47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,    52,    56,    58,    60,    10,    61,    58,     0,
      31,    32,    33,    34,    54,    57,     3,     4,     5,     6,
      26,    27,    28,    29,    30,    38,    40,    41,    42,    51,
      52,    62,    63,    79,    80,    81,    82,    83,    53,    10,
      35,    59,    59,    59,    59,    50,    52,    52,    52,    52,
      52,     3,    79,    79,     8,    49,     3,    23,    24,    25,
      36,    37,    38,    39,    64,    43,    44,    45,    46,    47,
      48,    58,    58,    58,    58,     3,    51,    10,    51,    79,
      10,    79,    10,    79,    79,    79,    50,    53,     3,    52,
      65,    66,    67,    69,    63,    79,    52,    80,    79,    79,
      23,    24,    25,     3,    80,    80,    80,    80,    80,    80,
      79,    53,    53,    79,    53,    79,    53,    53,    53,    64,
      67,     9,    70,    49,    11,    12,    13,    14,    68,    78,
      79,    36,    79,    52,    80,    53,    53,    53,    53,    79,
      17,    71,    67,    69,    15,    15,    11,    49,    53,    80,
      78,    36,    18,    19,    72,    16,    69,    79,    53,    80,
      77,    79,    79,    20,    73,    79,    16,    49,    18,    79,
      79,    74,    75,    79,    49,    21,    22,    76,    75
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    57,    57,    58,    58,    58,    58,    58,
      58,    59,    59,    59,    60,    61,    61,    62,    62,    63,
      63,    63,    64,    64,    64,    65,    66,    66,    67,    67,
      67,    68,    68,    68,    68,    68,    69,    69,    70,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    76,
      76,    76,    77,    77,    78,    78,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    82,    82,    83,    83,    83,
      83,    83,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     1,     4,     4,     4,     4,     1,
       3,     0,     1,     1,     9,     0,     1,     1,     3,     1,
       3,     2,     0,     2,     1,     1,     1,     3,     1,     6,
       5,     1,     1,     2,     1,     2,     2,     3,     0,     2,
       0,     3,     0,     2,     0,     3,     1,     3,     2,     0,
       1,     1,     1,     3,     1,     3,     3,     3,     2,     3,
       4,     5,     6,     5,     6,     1,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     4,     5,     4,     5,
       4,     5,     4,     4,     4,     1,     3,     1,     1,     1,
       1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* query: set_query opt_semicolon  */
#line 100 "src\\parser.y"
                            {
        g_root = (yyvsp[-1].ast_node);
        (yyval.ast_node) = (yyvsp[-1].ast_node);
    }
#line 1715 "src\\parser.tab.cpp"
    break;

  case 5: /* set_query: set_query UNION opt_all set_query  */
#line 112 "src\\parser.y"
                                                  {
        (yyval.ast_node) = new SetOpQueryNode((yyvsp[-1].int_val) ? "UNION ALL" : "UNION", (yyvsp[-3].ast_node), (yyvsp[0].ast_node));
    }
#line 1723 "src\\parser.tab.cpp"
    break;

  case 6: /* set_query: set_query INTERSECT opt_all set_query  */
#line 115 "src\\parser.y"
                                                          {
        (yyval.ast_node) = new SetOpQueryNode((yyvsp[-1].int_val) ? "INTERSECT ALL" : "INTERSECT", (yyvsp[-3].ast_node), (yyvsp[0].ast_node));
    }
#line 1731 "src\\parser.tab.cpp"
    break;

  case 7: /* set_query: set_query EXCEPT opt_all set_query  */
#line 118 "src\\parser.y"
                                                    {
        (yyval.ast_node) = new SetOpQueryNode((yyvsp[-1].int_val) ? "EXCEPT ALL" : "EXCEPT", (yyvsp[-3].ast_node), (yyvsp[0].ast_node));
    }
#line 1739 "src\\parser.tab.cpp"
    break;

  case 8: /* set_query: set_query MINUS opt_all set_query  */
#line 121 "src\\parser.y"
                                                   {
        (yyval.ast_node) = new SetOpQueryNode((yyvsp[-1].int_val) ? "EXCEPT ALL" : "EXCEPT", (yyvsp[-3].ast_node), (yyvsp[0].ast_node));
    }
#line 1747 "src\\parser.tab.cpp"
    break;

  case 9: /* set_query: select_stmt  */
#line 124 "src\\parser.y"
                {
        (yyval.ast_node) = (yyvsp[0].select_stmt);
    }
#line 1755 "src\\parser.tab.cpp"
    break;

  case 10: /* set_query: LPAREN set_query RPAREN  */
#line 127 "src\\parser.y"
                            {
        (yyval.ast_node) = (yyvsp[-1].ast_node);
    }
#line 1763 "src\\parser.tab.cpp"
    break;

  case 11: /* opt_all: %empty  */
#line 133 "src\\parser.y"
                { (yyval.int_val) = 0; }
#line 1769 "src\\parser.tab.cpp"
    break;

  case 12: /* opt_all: ALL  */
#line 134 "src\\parser.y"
                { (yyval.int_val) = 1; }
#line 1775 "src\\parser.tab.cpp"
    break;

  case 13: /* opt_all: DISTINCT  */
#line 135 "src\\parser.y"
                { (yyval.int_val) = 0; }
#line 1781 "src\\parser.tab.cpp"
    break;

  case 14: /* select_stmt: SELECT opt_distinct select_list FROM from_clause opt_where opt_group_by opt_having opt_order_by  */
#line 139 "src\\parser.y"
                                                                                                    {
        auto* stmt = new SelectStmtNode();
        stmt->is_distinct = ((yyvsp[-7].int_val) != 0);
        if ((yyvsp[-6].select_list)) {
            stmt->select_list = *(yyvsp[-6].select_list);
            delete (yyvsp[-6].select_list);
        }
        if ((yyvsp[-4].table_list)) {
            stmt->from_clause = *(yyvsp[-4].table_list);
            delete (yyvsp[-4].table_list);
        }
        stmt->where_clause = (yyvsp[-3].expr_node);
        if ((yyvsp[-2].expr_list)) {
            stmt->group_by_clause = *(yyvsp[-2].expr_list);
            delete (yyvsp[-2].expr_list);
        }
        stmt->having_clause = (yyvsp[-1].expr_node);
        if ((yyvsp[0].order_list)) {
            stmt->order_by_clause = *(yyvsp[0].order_list);
            delete (yyvsp[0].order_list);
        }
        (yyval.select_stmt) = stmt;
    }
#line 1809 "src\\parser.tab.cpp"
    break;

  case 15: /* opt_distinct: %empty  */
#line 165 "src\\parser.y"
                { (yyval.int_val) = 0; }
#line 1815 "src\\parser.tab.cpp"
    break;

  case 16: /* opt_distinct: DISTINCT  */
#line 166 "src\\parser.y"
                { (yyval.int_val) = 1; }
#line 1821 "src\\parser.tab.cpp"
    break;

  case 17: /* select_list: select_item  */
#line 170 "src\\parser.y"
                {
        auto* list = new std::vector<SelectItem*>();
        list->push_back((yyvsp[0].select_item));
        (yyval.select_list) = list;
    }
#line 1831 "src\\parser.tab.cpp"
    break;

  case 18: /* select_list: select_list COMMA select_item  */
#line 175 "src\\parser.y"
                                  {
        (yyvsp[-2].select_list)->push_back((yyvsp[0].select_item));
        (yyval.select_list) = (yyvsp[-2].select_list);
    }
#line 1840 "src\\parser.tab.cpp"
    break;

  case 19: /* select_item: STAR  */
#line 182 "src\\parser.y"
         {
        (yyval.select_item) = new SelectItem(new StarExpr(""));
    }
#line 1848 "src\\parser.tab.cpp"
    break;

  case 20: /* select_item: IDENTIFIER DOT STAR  */
#line 185 "src\\parser.y"
                        {
        (yyval.select_item) = new SelectItem(new StarExpr((yyvsp[-2].str_val)));
        free((yyvsp[-2].str_val));
    }
#line 1857 "src\\parser.tab.cpp"
    break;

  case 21: /* select_item: expr opt_alias  */
#line 189 "src\\parser.y"
                   {
        (yyval.select_item) = new SelectItem((yyvsp[-1].expr_node), (yyvsp[0].str_val) ? (yyvsp[0].str_val) : "");
        if ((yyvsp[0].str_val)) free((yyvsp[0].str_val));
    }
#line 1866 "src\\parser.tab.cpp"
    break;

  case 22: /* opt_alias: %empty  */
#line 196 "src\\parser.y"
                { (yyval.str_val) = nullptr; }
#line 1872 "src\\parser.tab.cpp"
    break;

  case 23: /* opt_alias: AS IDENTIFIER  */
#line 197 "src\\parser.y"
                  { (yyval.str_val) = (yyvsp[0].str_val); }
#line 1878 "src\\parser.tab.cpp"
    break;

  case 24: /* opt_alias: IDENTIFIER  */
#line 198 "src\\parser.y"
                  { (yyval.str_val) = (yyvsp[0].str_val); }
#line 1884 "src\\parser.tab.cpp"
    break;

  case 25: /* from_clause: table_ref_list  */
#line 202 "src\\parser.y"
                   {
        (yyval.table_list) = (yyvsp[0].table_list);
    }
#line 1892 "src\\parser.tab.cpp"
    break;

  case 26: /* table_ref_list: table_ref  */
#line 208 "src\\parser.y"
              {
        auto* list = new std::vector<TableRef*>();
        list->push_back((yyvsp[0].table_ref));
        (yyval.table_list) = list;
    }
#line 1902 "src\\parser.tab.cpp"
    break;

  case 27: /* table_ref_list: table_ref_list COMMA table_ref  */
#line 213 "src\\parser.y"
                                   {
        (yyvsp[-2].table_list)->push_back((yyvsp[0].table_ref));
        (yyval.table_list) = (yyvsp[-2].table_list);
    }
#line 1911 "src\\parser.tab.cpp"
    break;

  case 28: /* table_ref: table_atom  */
#line 220 "src\\parser.y"
               {
        (yyval.table_ref) = (yyvsp[0].table_ref);
    }
#line 1919 "src\\parser.tab.cpp"
    break;

  case 29: /* table_ref: table_ref join_type JOIN table_atom ON expr  */
#line 223 "src\\parser.y"
                                                {
        (yyval.table_ref) = new TableRef((yyvsp[-4].str_val), (yyvsp[-5].table_ref), (yyvsp[-2].table_ref), (yyvsp[0].expr_node));
        free((yyvsp[-4].str_val));
    }
#line 1928 "src\\parser.tab.cpp"
    break;

  case 30: /* table_ref: table_ref JOIN table_atom ON expr  */
#line 227 "src\\parser.y"
                                      {
        (yyval.table_ref) = new TableRef("INNER", (yyvsp[-4].table_ref), (yyvsp[-2].table_ref), (yyvsp[0].expr_node));
    }
#line 1936 "src\\parser.tab.cpp"
    break;

  case 31: /* join_type: INNER  */
#line 233 "src\\parser.y"
                { (yyval.str_val) = my_strdup("INNER"); }
#line 1942 "src\\parser.tab.cpp"
    break;

  case 32: /* join_type: LEFT  */
#line 234 "src\\parser.y"
                { (yyval.str_val) = my_strdup("LEFT"); }
#line 1948 "src\\parser.tab.cpp"
    break;

  case 33: /* join_type: LEFT OUTER  */
#line 235 "src\\parser.y"
                { (yyval.str_val) = my_strdup("LEFT"); }
#line 1954 "src\\parser.tab.cpp"
    break;

  case 34: /* join_type: RIGHT  */
#line 236 "src\\parser.y"
                { (yyval.str_val) = my_strdup("RIGHT"); }
#line 1960 "src\\parser.tab.cpp"
    break;

  case 35: /* join_type: RIGHT OUTER  */
#line 237 "src\\parser.y"
                { (yyval.str_val) = my_strdup("RIGHT"); }
#line 1966 "src\\parser.tab.cpp"
    break;

  case 36: /* table_atom: IDENTIFIER opt_alias  */
#line 241 "src\\parser.y"
                         {
        (yyval.table_ref) = new TableRef((yyvsp[-1].str_val), (yyvsp[0].str_val) ? (yyvsp[0].str_val) : "");
        free((yyvsp[-1].str_val));
        if ((yyvsp[0].str_val)) free((yyvsp[0].str_val));
    }
#line 1976 "src\\parser.tab.cpp"
    break;

  case 37: /* table_atom: LPAREN table_ref RPAREN  */
#line 246 "src\\parser.y"
                            {
        (yyval.table_ref) = (yyvsp[-1].table_ref);
    }
#line 1984 "src\\parser.tab.cpp"
    break;

  case 38: /* opt_where: %empty  */
#line 252 "src\\parser.y"
                { (yyval.expr_node) = nullptr; }
#line 1990 "src\\parser.tab.cpp"
    break;

  case 39: /* opt_where: WHERE expr  */
#line 253 "src\\parser.y"
                { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 1996 "src\\parser.tab.cpp"
    break;

  case 40: /* opt_group_by: %empty  */
#line 257 "src\\parser.y"
                { (yyval.expr_list) = nullptr; }
#line 2002 "src\\parser.tab.cpp"
    break;

  case 41: /* opt_group_by: GROUP BY expr_list  */
#line 258 "src\\parser.y"
                       { (yyval.expr_list) = (yyvsp[0].expr_list); }
#line 2008 "src\\parser.tab.cpp"
    break;

  case 42: /* opt_having: %empty  */
#line 262 "src\\parser.y"
                { (yyval.expr_node) = nullptr; }
#line 2014 "src\\parser.tab.cpp"
    break;

  case 43: /* opt_having: HAVING expr  */
#line 263 "src\\parser.y"
                { (yyval.expr_node) = (yyvsp[0].expr_node); }
#line 2020 "src\\parser.tab.cpp"
    break;

  case 44: /* opt_order_by: %empty  */
#line 267 "src\\parser.y"
                { (yyval.order_list) = nullptr; }
#line 2026 "src\\parser.tab.cpp"
    break;

  case 45: /* opt_order_by: ORDER BY order_list  */
#line 268 "src\\parser.y"
                        { (yyval.order_list) = (yyvsp[0].order_list); }
#line 2032 "src\\parser.tab.cpp"
    break;

  case 46: /* order_list: order_item  */
#line 272 "src\\parser.y"
               {
        auto* list = new std::vector<OrderByItem*>();
        list->push_back((yyvsp[0].order_item));
        (yyval.order_list) = list;
    }
#line 2042 "src\\parser.tab.cpp"
    break;

  case 47: /* order_list: order_list COMMA order_item  */
#line 277 "src\\parser.y"
                                {
        (yyvsp[-2].order_list)->push_back((yyvsp[0].order_item));
        (yyval.order_list) = (yyvsp[-2].order_list);
    }
#line 2051 "src\\parser.tab.cpp"
    break;

  case 48: /* order_item: expr opt_asc_desc  */
#line 284 "src\\parser.y"
                      {
        (yyval.order_item) = new OrderByItem((yyvsp[-1].expr_node), ((yyvsp[0].int_val) != 0));
    }
#line 2059 "src\\parser.tab.cpp"
    break;

  case 49: /* opt_asc_desc: %empty  */
#line 290 "src\\parser.y"
                { (yyval.int_val) = 0; }
#line 2065 "src\\parser.tab.cpp"
    break;

  case 50: /* opt_asc_desc: ASC  */
#line 291 "src\\parser.y"
                { (yyval.int_val) = 0; }
#line 2071 "src\\parser.tab.cpp"
    break;

  case 51: /* opt_asc_desc: DESC  */
#line 292 "src\\parser.y"
                { (yyval.int_val) = 1; }
#line 2077 "src\\parser.tab.cpp"
    break;

  case 52: /* expr_list: expr  */
#line 296 "src\\parser.y"
         {
        auto* list = new std::vector<ExprNode*>();
        list->push_back((yyvsp[0].expr_node));
        (yyval.expr_list) = list;
    }
#line 2087 "src\\parser.tab.cpp"
    break;

  case 53: /* expr_list: expr_list COMMA expr  */
#line 301 "src\\parser.y"
                         {
        (yyvsp[-2].expr_list)->push_back((yyvsp[0].expr_node));
        (yyval.expr_list) = (yyvsp[-2].expr_list);
    }
#line 2096 "src\\parser.tab.cpp"
    break;

  case 54: /* in_expr_list: expr  */
#line 308 "src\\parser.y"
         {
        auto* list = new std::vector<ExprNode*>();
        list->push_back((yyvsp[0].expr_node));
        (yyval.expr_list) = list;
    }
#line 2106 "src\\parser.tab.cpp"
    break;

  case 55: /* in_expr_list: in_expr_list COMMA expr  */
#line 313 "src\\parser.y"
                            {
        (yyvsp[-2].expr_list)->push_back((yyvsp[0].expr_node));
        (yyval.expr_list) = (yyvsp[-2].expr_list);
    }
#line 2115 "src\\parser.tab.cpp"
    break;

  case 56: /* expr: expr OR expr  */
#line 320 "src\\parser.y"
                 {
        (yyval.expr_node) = new BinaryExpr("OR", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2123 "src\\parser.tab.cpp"
    break;

  case 57: /* expr: expr AND expr  */
#line 323 "src\\parser.y"
                  {
        (yyval.expr_node) = new BinaryExpr("AND", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2131 "src\\parser.tab.cpp"
    break;

  case 58: /* expr: NOT expr  */
#line 326 "src\\parser.y"
             {
        (yyval.expr_node) = new UnaryExpr("NOT", (yyvsp[0].expr_node));
    }
#line 2139 "src\\parser.tab.cpp"
    break;

  case 59: /* expr: expr LIKE expr  */
#line 329 "src\\parser.y"
                   {
        (yyval.expr_node) = new LikeExpr((yyvsp[-2].expr_node), (yyvsp[0].expr_node), false);
    }
#line 2147 "src\\parser.tab.cpp"
    break;

  case 60: /* expr: expr NOT LIKE expr  */
#line 332 "src\\parser.y"
                       {
        (yyval.expr_node) = new LikeExpr((yyvsp[-3].expr_node), (yyvsp[0].expr_node), true);
    }
#line 2155 "src\\parser.tab.cpp"
    break;

  case 61: /* expr: expr IN LPAREN in_expr_list RPAREN  */
#line 335 "src\\parser.y"
                                       {
        (yyval.expr_node) = new InExpr((yyvsp[-4].expr_node), *(yyvsp[-1].expr_list), false);
        delete (yyvsp[-1].expr_list);
    }
#line 2164 "src\\parser.tab.cpp"
    break;

  case 62: /* expr: expr NOT IN LPAREN in_expr_list RPAREN  */
#line 339 "src\\parser.y"
                                           {
        (yyval.expr_node) = new InExpr((yyvsp[-5].expr_node), *(yyvsp[-1].expr_list), true);
        delete (yyvsp[-1].expr_list);
    }
#line 2173 "src\\parser.tab.cpp"
    break;

  case 63: /* expr: expr BETWEEN bit_expr AND bit_expr  */
#line 343 "src\\parser.y"
                                                     {
        (yyval.expr_node) = new BetweenExpr((yyvsp[-4].expr_node), (yyvsp[-2].expr_node), (yyvsp[0].expr_node), false);
    }
#line 2181 "src\\parser.tab.cpp"
    break;

  case 64: /* expr: expr NOT BETWEEN bit_expr AND bit_expr  */
#line 346 "src\\parser.y"
                                                         {
        (yyval.expr_node) = new BetweenExpr((yyvsp[-5].expr_node), (yyvsp[-2].expr_node), (yyvsp[0].expr_node), true);
    }
#line 2189 "src\\parser.tab.cpp"
    break;

  case 65: /* expr: bit_expr  */
#line 349 "src\\parser.y"
             {
        (yyval.expr_node) = (yyvsp[0].expr_node);
    }
#line 2197 "src\\parser.tab.cpp"
    break;

  case 66: /* bit_expr: bit_expr EQ bit_expr  */
#line 355 "src\\parser.y"
                         {
        (yyval.expr_node) = new BinaryExpr("=", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2205 "src\\parser.tab.cpp"
    break;

  case 67: /* bit_expr: bit_expr NEQ bit_expr  */
#line 358 "src\\parser.y"
                          {
        (yyval.expr_node) = new BinaryExpr("<>", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2213 "src\\parser.tab.cpp"
    break;

  case 68: /* bit_expr: bit_expr LT bit_expr  */
#line 361 "src\\parser.y"
                         {
        (yyval.expr_node) = new BinaryExpr("<", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2221 "src\\parser.tab.cpp"
    break;

  case 69: /* bit_expr: bit_expr LTE bit_expr  */
#line 364 "src\\parser.y"
                          {
        (yyval.expr_node) = new BinaryExpr("<=", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2229 "src\\parser.tab.cpp"
    break;

  case 70: /* bit_expr: bit_expr GT bit_expr  */
#line 367 "src\\parser.y"
                         {
        (yyval.expr_node) = new BinaryExpr(">", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2237 "src\\parser.tab.cpp"
    break;

  case 71: /* bit_expr: bit_expr GTE bit_expr  */
#line 370 "src\\parser.y"
                          {
        (yyval.expr_node) = new BinaryExpr(">=", (yyvsp[-2].expr_node), (yyvsp[0].expr_node));
    }
#line 2245 "src\\parser.tab.cpp"
    break;

  case 72: /* bit_expr: LPAREN expr RPAREN  */
#line 373 "src\\parser.y"
                       {
        (yyval.expr_node) = (yyvsp[-1].expr_node);
    }
#line 2253 "src\\parser.tab.cpp"
    break;

  case 73: /* bit_expr: column_ref  */
#line 376 "src\\parser.y"
               {
        (yyval.expr_node) = (yyvsp[0].expr_node);
    }
#line 2261 "src\\parser.tab.cpp"
    break;

  case 74: /* bit_expr: literal  */
#line 379 "src\\parser.y"
            {
        (yyval.expr_node) = (yyvsp[0].expr_node);
    }
#line 2269 "src\\parser.tab.cpp"
    break;

  case 75: /* bit_expr: aggregate_expr  */
#line 382 "src\\parser.y"
                   {
        (yyval.expr_node) = (yyvsp[0].expr_node);
    }
#line 2277 "src\\parser.tab.cpp"
    break;

  case 76: /* aggregate_expr: COUNT LPAREN STAR RPAREN  */
#line 388 "src\\parser.y"
                             {
        (yyval.expr_node) = new AggregateExpr("COUNT", false, new StarExpr(""));
    }
#line 2285 "src\\parser.tab.cpp"
    break;

  case 77: /* aggregate_expr: COUNT LPAREN DISTINCT expr RPAREN  */
#line 391 "src\\parser.y"
                                      {
        (yyval.expr_node) = new AggregateExpr("COUNT", true, (yyvsp[-1].expr_node));
    }
#line 2293 "src\\parser.tab.cpp"
    break;

  case 78: /* aggregate_expr: COUNT LPAREN expr RPAREN  */
#line 394 "src\\parser.y"
                             {
        (yyval.expr_node) = new AggregateExpr("COUNT", false, (yyvsp[-1].expr_node));
    }
#line 2301 "src\\parser.tab.cpp"
    break;

  case 79: /* aggregate_expr: SUM LPAREN DISTINCT expr RPAREN  */
#line 397 "src\\parser.y"
                                    {
        (yyval.expr_node) = new AggregateExpr("SUM", true, (yyvsp[-1].expr_node));
    }
#line 2309 "src\\parser.tab.cpp"
    break;

  case 80: /* aggregate_expr: SUM LPAREN expr RPAREN  */
#line 400 "src\\parser.y"
                           {
        (yyval.expr_node) = new AggregateExpr("SUM", false, (yyvsp[-1].expr_node));
    }
#line 2317 "src\\parser.tab.cpp"
    break;

  case 81: /* aggregate_expr: AVG LPAREN DISTINCT expr RPAREN  */
#line 403 "src\\parser.y"
                                    {
        (yyval.expr_node) = new AggregateExpr("AVG", true, (yyvsp[-1].expr_node));
    }
#line 2325 "src\\parser.tab.cpp"
    break;

  case 82: /* aggregate_expr: AVG LPAREN expr RPAREN  */
#line 406 "src\\parser.y"
                           {
        (yyval.expr_node) = new AggregateExpr("AVG", false, (yyvsp[-1].expr_node));
    }
#line 2333 "src\\parser.tab.cpp"
    break;

  case 83: /* aggregate_expr: MIN LPAREN expr RPAREN  */
#line 409 "src\\parser.y"
                           {
        (yyval.expr_node) = new AggregateExpr("MIN", false, (yyvsp[-1].expr_node));
    }
#line 2341 "src\\parser.tab.cpp"
    break;

  case 84: /* aggregate_expr: MAX LPAREN expr RPAREN  */
#line 412 "src\\parser.y"
                           {
        (yyval.expr_node) = new AggregateExpr("MAX", false, (yyvsp[-1].expr_node));
    }
#line 2349 "src\\parser.tab.cpp"
    break;

  case 85: /* column_ref: IDENTIFIER  */
#line 418 "src\\parser.y"
               {
        (yyval.expr_node) = new ColumnRefExpr((yyvsp[0].str_val));
        free((yyvsp[0].str_val));
    }
#line 2358 "src\\parser.tab.cpp"
    break;

  case 86: /* column_ref: IDENTIFIER DOT IDENTIFIER  */
#line 422 "src\\parser.y"
                              {
        (yyval.expr_node) = new ColumnRefExpr((yyvsp[-2].str_val), (yyvsp[0].str_val));
        free((yyvsp[-2].str_val));
        free((yyvsp[0].str_val));
    }
#line 2368 "src\\parser.tab.cpp"
    break;

  case 87: /* literal: INT_LITERAL  */
#line 430 "src\\parser.y"
                {
        (yyval.expr_node) = new LiteralExpr(LiteralExpr::LiteralType::INT, (yyvsp[0].str_val));
        free((yyvsp[0].str_val));
    }
#line 2377 "src\\parser.tab.cpp"
    break;

  case 88: /* literal: FLOAT_LITERAL  */
#line 434 "src\\parser.y"
                  {
        (yyval.expr_node) = new LiteralExpr(LiteralExpr::LiteralType::FLOAT, (yyvsp[0].str_val));
        free((yyvsp[0].str_val));
    }
#line 2386 "src\\parser.tab.cpp"
    break;

  case 89: /* literal: STRING_LITERAL  */
#line 438 "src\\parser.y"
                   {
        (yyval.expr_node) = new LiteralExpr(LiteralExpr::LiteralType::STRING, (yyvsp[0].str_val));
        free((yyvsp[0].str_val));
    }
#line 2395 "src\\parser.tab.cpp"
    break;

  case 90: /* literal: TRUE_KW  */
#line 442 "src\\parser.y"
            {
        (yyval.expr_node) = new LiteralExpr(LiteralExpr::LiteralType::BOOLEAN, "TRUE");
    }
#line 2403 "src\\parser.tab.cpp"
    break;

  case 91: /* literal: FALSE_KW  */
#line 445 "src\\parser.y"
             {
        (yyval.expr_node) = new LiteralExpr(LiteralExpr::LiteralType::BOOLEAN, "FALSE");
    }
#line 2411 "src\\parser.tab.cpp"
    break;

  case 92: /* literal: NULL_KW  */
#line 448 "src\\parser.y"
            {
        (yyval.expr_node) = new LiteralExpr(LiteralExpr::LiteralType::NULL_VALUE, "NULL");
    }
#line 2419 "src\\parser.tab.cpp"
    break;


#line 2423 "src\\parser.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 453 "src\\parser.y"


void yyerror(const char* s) {
    g_parse_error.has_error = true;
    g_parse_error.message = s;
    g_parse_error.line = yylloc.first_line;
    g_parse_error.column = yylloc.first_column;
}
