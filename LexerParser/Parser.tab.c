/* A Bison parser, made by GNU Bison 3.8.  */

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
#define YYBISON 30800

/* Bison version string.  */
#define YYBISON_VERSION "3.8"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 4 "./Parser.y"

#include <cstdio>
#include <vector>
#include <ctime>
#include "../defines.h"
#include "../SystemManager/SystemManager.h"
#include "../QueryManager/QueryManager.h"
#include "../IndexManager/IndexManager.h"
#include "../RecordManager/RecordManager.h"
extern int yylex(void);
void yyerror(const char *s, ...);

#line 84 "Parser.tab.c"

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

#include "Parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_STR_INT = 4,                    /* STR_INT  */
  YYSYMBOL_STR_FLOAT = 5,                  /* STR_FLOAT  */
  YYSYMBOL_STR_VARCHAR = 6,                /* STR_VARCHAR  */
  YYSYMBOL_SAFE = 7,                       /* SAFE  */
  YYSYMBOL_QUIT = 8,                       /* QUIT  */
  YYSYMBOL_CREATE = 9,                     /* CREATE  */
  YYSYMBOL_DROP = 10,                      /* DROP  */
  YYSYMBOL_SHOW = 11,                      /* SHOW  */
  YYSYMBOL_USE = 12,                       /* USE  */
  YYSYMBOL_DESC = 13,                      /* DESC  */
  YYSYMBOL_INSERT = 14,                    /* INSERT  */
  YYSYMBOL_DELETE = 15,                    /* DELETE  */
  YYSYMBOL_UPDATE = 16,                    /* UPDATE  */
  YYSYMBOL_SELECT = 17,                    /* SELECT  */
  YYSYMBOL_ALTER = 18,                     /* ALTER  */
  YYSYMBOL_ADD = 19,                       /* ADD  */
  YYSYMBOL_INTO = 20,                      /* INTO  */
  YYSYMBOL_VALUES = 21,                    /* VALUES  */
  YYSYMBOL_FROM = 22,                      /* FROM  */
  YYSYMBOL_WHERE = 23,                     /* WHERE  */
  YYSYMBOL_SET = 24,                       /* SET  */
  YYSYMBOL_NOT = 25,                       /* NOT  */
  YYSYMBOL_DEFAULT = 26,                   /* DEFAULT  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_DATABASES = 28,                 /* DATABASES  */
  YYSYMBOL_DATABASE = 29,                  /* DATABASE  */
  YYSYMBOL_TABLES = 30,                    /* TABLES  */
  YYSYMBOL_TABLE = 31,                     /* TABLE  */
  YYSYMBOL_INDEX = 32,                     /* INDEX  */
  YYSYMBOL_PRIMARY = 33,                   /* PRIMARY  */
  YYSYMBOL_FOREIGN = 34,                   /* FOREIGN  */
  YYSYMBOL_KEY = 35,                       /* KEY  */
  YYSYMBOL_CONSTRAINT = 36,                /* CONSTRAINT  */
  YYSYMBOL_REFERENCES = 37,                /* REFERENCES  */
  YYSYMBOL_T_INT = 38,                     /* T_INT  */
  YYSYMBOL_T_FLOAT = 39,                   /* T_FLOAT  */
  YYSYMBOL_T_VARCHAR = 40,                 /* T_VARCHAR  */
  YYSYMBOL_STR_NULL = 41,                  /* STR_NULL  */
  YYSYMBOL_42_ = 42,                       /* ';'  */
  YYSYMBOL_43_ = 43,                       /* '('  */
  YYSYMBOL_44_ = 44,                       /* ')'  */
  YYSYMBOL_45_ = 45,                       /* ','  */
  YYSYMBOL_46_ = 46,                       /* '.'  */
  YYSYMBOL_47_ = 47,                       /* '<'  */
  YYSYMBOL_48_ = 48,                       /* '='  */
  YYSYMBOL_49_ = 49,                       /* '>'  */
  YYSYMBOL_50_ = 50,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_program = 52,                   /* program  */
  YYSYMBOL_statements = 53,                /* statements  */
  YYSYMBOL_statement = 54,                 /* statement  */
  YYSYMBOL_db_statement = 55,              /* db_statement  */
  YYSYMBOL_table_statement = 56,           /* table_statement  */
  YYSYMBOL_alter_statement = 57,           /* alter_statement  */
  YYSYMBOL_field_list = 58,                /* field_list  */
  YYSYMBOL_field = 59,                     /* field  */
  YYSYMBOL_pk = 60,                        /* pk  */
  YYSYMBOL_fk_list = 61,                   /* fk_list  */
  YYSYMBOL_fk = 62,                        /* fk  */
  YYSYMBOL_attr_type = 63,                 /* attr_type  */
  YYSYMBOL_not_null = 64,                  /* not_null  */
  YYSYMBOL_default = 65,                   /* default  */
  YYSYMBOL_value_lists = 66,               /* value_lists  */
  YYSYMBOL_value_list = 67,                /* value_list  */
  YYSYMBOL_values = 68,                    /* values  */
  YYSYMBOL_where_and_clause = 69,          /* where_and_clause  */
  YYSYMBOL_cond_list = 70,                 /* cond_list  */
  YYSYMBOL_cond_expr = 71,                 /* cond_expr  */
  YYSYMBOL_col_expr = 72,                  /* col_expr  */
  YYSYMBOL_val_expr = 73,                  /* val_expr  */
  YYSYMBOL_cmp_op = 74,                    /* cmp_op  */
  YYSYMBOL_set_list = 75,                  /* set_list  */
  YYSYMBOL_set_expr = 76,                  /* set_expr  */
  YYSYMBOL_assign = 77,                    /* assign  */
  YYSYMBOL_selectors = 78,                 /* selectors  */
  YYSYMBOL_column_list = 79                /* column_list  */
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  39
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  205

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   296


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
      43,    44,    50,     2,    45,     2,    46,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    42,
      47,    48,    49,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    54,    54,    58,    59,    63,    64,    65,    66,    73,
      77,    81,    85,    89,    93,   100,   110,   117,   125,   130,
     134,   138,   143,   155,   161,   168,   176,   186,   190,   194,
     198,   202,   206,   214,   221,   230,   238,   249,   257,   263,
     272,   282,   286,   294,   297,   305,   308,   316,   322,   331,
     338,   344,   355,   358,   363,   369,   378,   382,   389,   393,
     399,   403,   407,   411,   418,   422,   426,   430,   434,   438,
     446,   452,   461,   468,   475,   479,   486,   492
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "STR_INT", "STR_FLOAT", "STR_VARCHAR", "SAFE", "QUIT", "CREATE", "DROP",
  "SHOW", "USE", "DESC", "INSERT", "DELETE", "UPDATE", "SELECT", "ALTER",
  "ADD", "INTO", "VALUES", "FROM", "WHERE", "SET", "NOT", "DEFAULT", "AND",
  "DATABASES", "DATABASE", "TABLES", "TABLE", "INDEX", "PRIMARY",
  "FOREIGN", "KEY", "CONSTRAINT", "REFERENCES", "T_INT", "T_FLOAT",
  "T_VARCHAR", "STR_NULL", "';'", "'('", "')'", "','", "'.'", "'<'", "'='",
  "'>'", "'*'", "$accept", "program", "statements", "statement",
  "db_statement", "table_statement", "alter_statement", "field_list",
  "field", "pk", "fk_list", "fk", "attr_type", "not_null", "default",
  "value_lists", "value_list", "values", "where_and_clause", "cond_list",
  "cond_expr", "col_expr", "val_expr", "cmp_op", "set_list", "set_expr",
  "assign", "selectors", "column_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-86)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      53,    -1,   -23,     2,    17,    10,    20,    22,    19,    27,
      51,     0,    24,    75,    53,   -86,   -86,   -86,   -86,    63,
     -86,    85,    86,    87,    88,    50,    52,    54,    55,    56,
      90,    92,    76,    57,   -86,   -86,    77,    59,    98,   -86,
     -86,   102,    65,    66,    68,    69,   -86,   -86,   -86,   -86,
     -86,    91,    93,   105,   110,   111,   105,     7,    94,   -86,
     114,   -86,   -86,    78,   105,    80,    70,    -9,   -86,   -86,
      -8,   -86,    -4,   -16,    78,    34,    13,   -86,     6,   -24,
     -86,    96,   -86,    -6,   -86,   -86,     6,   105,    82,   116,
      83,    84,    95,    97,    99,    18,   -18,   -86,   -86,   100,
     101,    89,     1,    33,    35,   -86,   -86,   -86,   -86,    37,
     -86,   -86,    78,   105,    36,   -86,    72,     3,   -86,   -86,
     -86,    93,   -86,   125,   103,   126,   130,   104,   106,   -86,
     131,   107,   108,   -86,   109,   112,   -86,   -86,   113,   115,
      42,   117,   115,   -86,     6,   -86,   -86,   -86,   -86,   -86,
     -86,   -86,   118,   119,   -86,   120,   121,   123,   124,   127,
     -86,     6,   -86,   129,   132,   -86,   122,   -86,   -86,   -86,
     -86,   128,   -86,   134,   133,   135,   101,   -86,   137,   143,
     -86,   -86,   -86,   136,   138,   108,   139,   140,   144,   141,
     -86,   -86,   142,   -86,   147,   148,   145,   146,   149,   150,
     151,   152,   155,   -86,   -86
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     4,     5,     6,     7,     0,
       8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    74,    77,     0,    75,     0,     1,
       3,     0,     0,     0,     0,     0,    14,    11,    13,    12,
      20,     0,    52,     0,     0,     0,     0,     0,     0,     9,
       0,    10,    19,     0,     0,     0,     0,    52,    71,    59,
      52,    76,     0,     0,     0,     0,     0,    34,     0,     0,
      48,    53,    55,     0,    23,    73,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,    42,     0,
      43,     0,     0,     0,     0,    60,    61,    62,    63,     0,
      51,    22,     0,     0,    64,    68,    66,     0,    72,    70,
      24,    52,    25,     0,     0,     0,     0,     0,     0,    21,
       0,     0,    45,    18,     0,     0,    33,    39,     0,     0,
       0,     0,     0,    49,     0,    47,    54,    65,    69,    67,
      57,    56,     0,     0,    29,     0,     0,     0,     0,     0,
      44,     0,    35,     0,     0,    16,     0,    17,    38,    50,
      26,     0,    30,     0,     0,     0,    43,    46,     0,     0,
      15,    28,    27,     0,     0,    45,     0,     0,     0,     0,
      36,    37,     0,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -86,   -86,   -86,   154,   -86,   -86,   -86,   -86,    67,   -86,
      58,    -5,   -86,   -22,   -29,   153,    45,   -86,   -65,   -86,
      60,   -11,   -85,   -86,   -86,    71,   -86,   -86,   -86
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    13,    14,    15,    16,    17,    18,    76,    77,   103,
     104,   137,   100,   132,   162,    79,    80,   109,    65,    81,
      82,    66,   110,   117,    67,    68,    86,    36,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      35,   118,    88,    33,    75,    90,    33,   105,   106,   107,
     105,   106,   107,    19,    64,    64,    94,    72,   111,    20,
      95,   112,    25,    28,   129,    29,    73,   112,    91,    92,
      93,    21,   151,    22,   134,   135,    87,    89,    26,    30,
      27,   114,   115,   116,   108,    71,    23,   108,    24,    31,
      34,   127,   128,    83,    32,    38,   152,   101,   102,   169,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    97,    98,    99,    39,   177,   138,   139,   141,
     142,   143,   144,    41,   147,   148,   166,   142,    42,    43,
      44,    45,    46,    51,    47,    52,    48,    49,    50,    55,
      53,    57,    83,    54,    56,    58,   150,    59,    33,    60,
      61,    62,    63,    69,    70,    74,    64,    75,    85,   121,
     149,    78,    84,   113,   120,   122,   131,   123,   153,   155,
     124,   133,   125,   156,   161,   159,   183,   168,   184,   157,
     186,   158,   126,   130,   163,   154,   187,   164,   160,   135,
     196,   197,   200,   201,   185,   165,   190,   145,   119,   167,
     170,   140,   172,   171,   180,   173,   174,   175,    40,   136,
     181,   176,   178,   146,     0,   179,   182,     0,   194,   195,
     188,     0,   189,   191,   192,     0,   193,     0,   198,   199,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96
};

static const yytype_int16 yycheck[] =
{
      11,    86,    67,     3,     3,    70,     3,     4,     5,     6,
       4,     5,     6,    14,    23,    23,    32,    10,    42,    42,
      36,    45,    12,     3,    42,     3,    19,    45,    32,    33,
      34,    29,   117,    31,    33,    34,    45,    45,    28,    20,
      30,    47,    48,    49,    41,    56,    29,    41,    31,    22,
      50,    33,    34,    64,     3,    31,   121,    44,    45,   144,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    38,    39,    40,     0,   161,    44,    45,    44,
      45,    44,    45,    20,    48,    49,    44,    45,     3,     3,
       3,     3,    42,     3,    42,     3,    42,    42,    42,    22,
      24,     3,   113,    46,    45,     3,   117,    42,     3,    43,
      42,    42,    21,     3,     3,    21,    23,     3,    48,     3,
      48,    43,    42,    27,    42,    42,    25,    43,     3,     3,
      35,    42,    35,     3,    26,     4,     3,   142,     3,    35,
       3,    35,    43,    43,    35,    42,     3,    35,    41,    34,
       3,     3,     3,     3,   176,    42,   185,   112,    87,    42,
      42,   103,    42,    44,    42,    44,    43,    43,    14,   102,
      42,    44,    43,   113,    -1,    43,    42,    -1,    37,    37,
      44,    -1,    44,    44,    44,    -1,    42,    -1,    43,    43,
      -1,    -1,    -1,    -1,    -1,    44,    44,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    52,    53,    54,    55,    56,    57,    14,
      42,    29,    31,    29,    31,    12,    28,    30,     3,     3,
      20,    22,     3,     3,    50,    72,    78,    79,    31,     0,
      54,    20,     3,     3,     3,     3,    42,    42,    42,    42,
      42,     3,     3,    24,    46,    22,    45,     3,     3,    42,
      43,    42,    42,    21,    23,    69,    72,    75,    76,     3,
       3,    72,    10,    19,    21,     3,    58,    59,    43,    66,
      67,    70,    71,    72,    42,    48,    77,    45,    69,    45,
      69,    32,    33,    34,    32,    36,    66,    38,    39,    40,
      63,    44,    45,    60,    61,     4,     5,     6,    41,    68,
      73,    42,    45,    27,    47,    48,    49,    74,    73,    76,
      42,     3,    42,    43,    35,    35,    43,    33,    34,    42,
      43,    25,    64,    42,    33,    34,    59,    62,    44,    45,
      61,    44,    45,    44,    45,    67,    71,    48,    49,    48,
      72,    73,    69,     3,    42,     3,     3,    35,    35,     4,
      41,    26,    65,    35,    35,    42,    44,    42,    62,    73,
      42,    44,    42,    44,    43,    43,    44,    73,    43,    43,
      42,    42,    42,     3,     3,    64,     3,     3,    44,    44,
      65,    44,    44,    42,    37,    37,     3,     3,    43,    43,
       3,     3,    44,    44,    42
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    54,    54,    54,    55,
      55,    55,    55,    55,    55,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    57,    57,    57,
      57,    57,    57,    58,    58,    59,    59,    60,    61,    61,
      62,    63,    63,    64,    64,    65,    65,    66,    66,    67,
      68,    68,    69,    69,    70,    70,    71,    71,    72,    72,
      73,    73,    73,    73,    74,    74,    74,    74,    74,    74,
      75,    75,    76,    77,    78,    78,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     2,     4,
       4,     3,     3,     3,     3,     9,     8,     8,     7,     4,
       3,     7,     6,     5,     6,     6,     8,     9,     9,     7,
       8,    11,    16,     3,     1,     4,     7,     6,     3,     2,
      10,     1,     1,     0,     2,     0,     2,     3,     1,     3,
       3,     1,     0,     2,     3,     1,     3,     3,     1,     3,
       1,     1,     1,     1,     1,     2,     1,     2,     1,     2,
       3,     1,     3,     1,     1,     1,     3,     1
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
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
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (QueryManager *qm, SystemManager *sm)
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 8: /* statement: QUIT ';'  */
#line 67 "./Parser.y"
    {
		YYACCEPT;
    }
#line 1282 "Parser.tab.c"
    break;

  case 9: /* db_statement: CREATE DATABASE IDENTIFIER ';'  */
#line 74 "./Parser.y"
    {
        sm->createDB((yyvsp[-1].str));
    }
#line 1290 "Parser.tab.c"
    break;

  case 10: /* db_statement: DROP DATABASE IDENTIFIER ';'  */
#line 78 "./Parser.y"
    {
        sm->dropDB((yyvsp[-1].str));
    }
#line 1298 "Parser.tab.c"
    break;

  case 11: /* db_statement: SHOW DATABASES ';'  */
#line 82 "./Parser.y"
    {
        sm->showDBs();
    }
#line 1306 "Parser.tab.c"
    break;

  case 12: /* db_statement: USE IDENTIFIER ';'  */
#line 86 "./Parser.y"
    {
        sm->useDB((yyvsp[-1].str));
    }
#line 1314 "Parser.tab.c"
    break;

  case 13: /* db_statement: SHOW TABLES ';'  */
#line 90 "./Parser.y"
    {
        sm->showTables();
    }
#line 1322 "Parser.tab.c"
    break;

  case 14: /* db_statement: SHOW USE ';'  */
#line 94 "./Parser.y"
    {
        sm->showUse();
    }
#line 1330 "Parser.tab.c"
    break;

  case 15: /* table_statement: CREATE TABLE IDENTIFIER '(' field_list pk fk_list ')' ';'  */
#line 101 "./Parser.y"
    {
        sm->createTable((yyvsp[-6].str), (yyvsp[-4].columnList));
        delete (yyvsp[-4].columnList);
        sm->addPrimaryKey((yyvsp[-6].str), (yyvsp[-3].pk)->_columnName, true);
        delete (yyvsp[-3].pk);
        for (FK &fk : *(yyvsp[-2].fkList))
            sm->addForeignKey(fk._priColName, (yyvsp[-6].str), fk._forColName, fk._priTbName, fk._priColName, true);
        delete (yyvsp[-2].fkList);
    }
#line 1344 "Parser.tab.c"
    break;

  case 16: /* table_statement: CREATE TABLE IDENTIFIER '(' field_list pk ')' ';'  */
#line 111 "./Parser.y"
    {
        sm->createTable((yyvsp[-5].str), (yyvsp[-3].columnList));
        delete (yyvsp[-3].columnList);
        sm->addPrimaryKey((yyvsp[-5].str), (yyvsp[-2].pk)->_columnName, true);
        delete (yyvsp[-2].pk);
    }
#line 1355 "Parser.tab.c"
    break;

  case 17: /* table_statement: CREATE TABLE IDENTIFIER '(' field_list fk_list ')' ';'  */
#line 118 "./Parser.y"
    {
        sm->createTable((yyvsp[-5].str), (yyvsp[-3].columnList));
        delete (yyvsp[-3].columnList);
        for (FK &fk : *(yyvsp[-2].fkList))
            sm->addForeignKey(fk._priColName, (yyvsp[-5].str), fk._forColName, fk._priTbName, fk._priColName, true);
        delete (yyvsp[-2].fkList);
    }
#line 1367 "Parser.tab.c"
    break;

  case 18: /* table_statement: CREATE TABLE IDENTIFIER '(' field_list ')' ';'  */
#line 126 "./Parser.y"
    {
        sm->createTable((yyvsp[-4].str), (yyvsp[-2].columnList));
        delete (yyvsp[-2].columnList);
    }
#line 1376 "Parser.tab.c"
    break;

  case 19: /* table_statement: DROP TABLE IDENTIFIER ';'  */
#line 131 "./Parser.y"
    {
        sm->dropTable((yyvsp[-1].str));
    }
#line 1384 "Parser.tab.c"
    break;

  case 20: /* table_statement: DESC IDENTIFIER ';'  */
#line 135 "./Parser.y"
    {
        sm->descTable((yyvsp[-1].str));
    }
#line 1392 "Parser.tab.c"
    break;

  case 21: /* table_statement: SAFE INSERT INTO IDENTIFIER VALUES value_lists ';'  */
#line 139 "./Parser.y"
    {
        qm->safeInsert((yyvsp[-3].str), (yyvsp[-1].valLists));
        delete (yyvsp[-1].valLists);
    }
#line 1401 "Parser.tab.c"
    break;

  case 22: /* table_statement: INSERT INTO IDENTIFIER VALUES value_lists ';'  */
#line 144 "./Parser.y"
    {
        clock_t start = clock();
        int success = 0;
        for (std::vector<Expr> values : *(yyvsp[-1].valLists)){
            if (qm->insertRec((yyvsp[-3].str), &values))
                success++;
        }
        printf("Successfully insert %d record of %d.\n", success, (int)((yyvsp[-1].valLists)->size()));
        printf("Spend %fs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
        delete (yyvsp[-1].valLists);
    }
#line 1417 "Parser.tab.c"
    break;

  case 23: /* table_statement: DELETE FROM IDENTIFIER where_and_clause ';'  */
#line 156 "./Parser.y"
    {
        qm->deleteRec((yyvsp[-2].str), (yyvsp[-1].exprList));
        if ((yyvsp[-1].exprList) != nullptr)
            delete (yyvsp[-1].exprList);
    }
#line 1427 "Parser.tab.c"
    break;

  case 24: /* table_statement: UPDATE IDENTIFIER SET set_list where_and_clause ';'  */
#line 162 "./Parser.y"
    {
        qm->updateRec((yyvsp[-4].str), (yyvsp[-2].exprList), (yyvsp[-1].exprList));
        delete (yyvsp[-2].exprList);
        if ((yyvsp[-1].exprList) != nullptr)
            delete (yyvsp[-1].exprList);
    }
#line 1438 "Parser.tab.c"
    break;

  case 25: /* table_statement: SELECT selectors FROM IDENTIFIER where_and_clause ';'  */
#line 169 "./Parser.y"
    {
        qm->selectRec(false, (yyvsp[-2].str), nullptr, (yyvsp[-4].exprList), (yyvsp[-1].exprList));
        if ((yyvsp[-4].exprList) != nullptr)
            delete (yyvsp[-4].exprList);
        if ((yyvsp[-1].exprList) != nullptr)
            delete (yyvsp[-1].exprList);
    }
#line 1450 "Parser.tab.c"
    break;

  case 26: /* table_statement: SELECT selectors FROM IDENTIFIER ',' IDENTIFIER where_and_clause ';'  */
#line 177 "./Parser.y"
    {
        qm->selectRec(true, (yyvsp[-4].str), (yyvsp[-2].str), (yyvsp[-6].exprList), (yyvsp[-1].exprList));
        if ((yyvsp[-6].exprList) != nullptr)
            delete (yyvsp[-6].exprList);
        if ((yyvsp[-1].exprList) != nullptr)
            delete (yyvsp[-1].exprList);
    }
#line 1462 "Parser.tab.c"
    break;

  case 27: /* alter_statement: ALTER TABLE IDENTIFIER ADD INDEX '(' IDENTIFIER ')' ';'  */
#line 187 "./Parser.y"
    {
        sm->addIndex((yyvsp[-6].str), (yyvsp[-2].str));
    }
#line 1470 "Parser.tab.c"
    break;

  case 28: /* alter_statement: ALTER TABLE IDENTIFIER DROP INDEX '(' IDENTIFIER ')' ';'  */
#line 191 "./Parser.y"
    {
        sm->dropIndex((yyvsp[-6].str), (yyvsp[-2].str));
    }
#line 1478 "Parser.tab.c"
    break;

  case 29: /* alter_statement: ALTER TABLE IDENTIFIER DROP PRIMARY KEY ';'  */
#line 195 "./Parser.y"
    {
        sm->dropPrimaryKey((yyvsp[-4].str));
    }
#line 1486 "Parser.tab.c"
    break;

  case 30: /* alter_statement: ALTER TABLE IDENTIFIER DROP FOREIGN KEY IDENTIFIER ';'  */
#line 199 "./Parser.y"
    {
        sm->dropForeignKey((yyvsp[-5].str), (yyvsp[-1].str));
    }
#line 1494 "Parser.tab.c"
    break;

  case 31: /* alter_statement: ALTER TABLE IDENTIFIER ADD CONSTRAINT PRIMARY KEY '(' IDENTIFIER ')' ';'  */
#line 203 "./Parser.y"
    {
        sm->addPrimaryKey((yyvsp[-8].str), (yyvsp[-2].str));
    }
#line 1502 "Parser.tab.c"
    break;

  case 32: /* alter_statement: ALTER TABLE IDENTIFIER ADD CONSTRAINT FOREIGN KEY '(' IDENTIFIER ')' REFERENCES IDENTIFIER '(' IDENTIFIER ')' ';'  */
#line 207 "./Parser.y"
    {
        sm->addForeignKey((yyvsp[-2].str), (yyvsp[-13].str), (yyvsp[-7].str), (yyvsp[-4].str), (yyvsp[-2].str));
    }
#line 1510 "Parser.tab.c"
    break;

  case 33: /* field_list: field_list ',' field  */
#line 215 "./Parser.y"
    {
        (yyval.columnList) = (yyvsp[-2].columnList);
        (yyval.columnList)->push_back(*(yyvsp[0].column));
        delete (yyvsp[0].column);

    }
#line 1521 "Parser.tab.c"
    break;

  case 34: /* field_list: field  */
#line 222 "./Parser.y"
    {
        (yyval.columnList) = new std::vector<Column>;
        (yyval.columnList)->push_back(*(yyvsp[0].column));
        delete (yyvsp[0].column);
    }
#line 1531 "Parser.tab.c"
    break;

  case 35: /* field: IDENTIFIER attr_type not_null default  */
#line 231 "./Parser.y"
    {
        (yyval.column) = new Column;
        NameUtil::cpy((yyval.column)->_name, (yyvsp[-3].str));
        (yyval.column)->_attrType = (yyvsp[-2].type);
        (yyval.column)->_attrLen = 4;
        (yyval.column)->_canBeNull = !(yyvsp[-1].boolean);
    }
#line 1543 "Parser.tab.c"
    break;

  case 36: /* field: IDENTIFIER T_VARCHAR '(' STR_INT ')' not_null default  */
#line 239 "./Parser.y"
    {
        (yyval.column) = new Column;
        NameUtil::cpy((yyval.column)->_name, (yyvsp[-6].str));
        (yyval.column)->_attrType = VARCHAR;
        (yyval.column)->_attrLen = atoi((yyvsp[-3].str));
        (yyval.column)->_canBeNull = !(yyvsp[-1].boolean);
    }
#line 1555 "Parser.tab.c"
    break;

  case 37: /* pk: ',' PRIMARY KEY '(' IDENTIFIER ')'  */
#line 250 "./Parser.y"
    {
        (yyval.pk) = new PK;
        NameUtil::cpy((yyval.pk)->_columnName, (yyvsp[-1].str));
    }
#line 1564 "Parser.tab.c"
    break;

  case 38: /* fk_list: fk_list ',' fk  */
#line 258 "./Parser.y"
    {
        (yyval.fkList) = (yyvsp[-2].fkList);
        (yyval.fkList)->push_back(*(yyvsp[0].fk));
        delete (yyvsp[0].fk);
    }
#line 1574 "Parser.tab.c"
    break;

  case 39: /* fk_list: ',' fk  */
#line 264 "./Parser.y"
    {
        (yyval.fkList) = new std::vector<FK>;
        (yyval.fkList)->push_back(*(yyvsp[0].fk));
        delete (yyvsp[0].fk);
    }
#line 1584 "Parser.tab.c"
    break;

  case 40: /* fk: FOREIGN KEY '(' IDENTIFIER ')' REFERENCES IDENTIFIER '(' IDENTIFIER ')'  */
#line 273 "./Parser.y"
    {
        (yyval.fk) = new FK;
        NameUtil::cpy((yyval.fk)->_forColName, (yyvsp[-6].str));
        NameUtil::cpy((yyval.fk)->_priTbName, (yyvsp[-3].str));
        NameUtil::cpy((yyval.fk)->_priColName, (yyvsp[-1].str));
    }
#line 1595 "Parser.tab.c"
    break;

  case 41: /* attr_type: T_INT  */
#line 283 "./Parser.y"
    {
        (yyval.type) = INT;
    }
#line 1603 "Parser.tab.c"
    break;

  case 42: /* attr_type: T_FLOAT  */
#line 287 "./Parser.y"
    {
        (yyval.type) = FLOAT;
    }
#line 1611 "Parser.tab.c"
    break;

  case 43: /* not_null: %empty  */
#line 294 "./Parser.y"
    {
        (yyval.boolean) = false;
    }
#line 1619 "Parser.tab.c"
    break;

  case 44: /* not_null: NOT STR_NULL  */
#line 298 "./Parser.y"
    {
        (yyval.boolean) = true;
    }
#line 1627 "Parser.tab.c"
    break;

  case 45: /* default: %empty  */
#line 305 "./Parser.y"
    {
        (yyval.expr) = nullptr;
    }
#line 1635 "Parser.tab.c"
    break;

  case 46: /* default: DEFAULT val_expr  */
#line 309 "./Parser.y"
    {
        (yyval.expr) = (yyvsp[0].expr);
    }
#line 1643 "Parser.tab.c"
    break;

  case 47: /* value_lists: value_lists ',' value_list  */
#line 317 "./Parser.y"
    {
        (yyval.valLists) = (yyvsp[-2].valLists);
        (yyval.valLists)->push_back(*(yyvsp[0].exprList));
        delete (yyvsp[0].exprList);
    }
#line 1653 "Parser.tab.c"
    break;

  case 48: /* value_lists: value_list  */
#line 323 "./Parser.y"
    {
        (yyval.valLists) = new std::vector<std::vector<Expr>>;
        (yyval.valLists)->push_back(*(yyvsp[0].exprList));
        delete (yyvsp[0].exprList);
    }
#line 1663 "Parser.tab.c"
    break;

  case 49: /* value_list: '(' values ')'  */
#line 332 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[-1].exprList);
    }
#line 1671 "Parser.tab.c"
    break;

  case 50: /* values: values ',' val_expr  */
#line 339 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[-2].exprList);
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1681 "Parser.tab.c"
    break;

  case 51: /* values: val_expr  */
#line 345 "./Parser.y"
    {
        (yyval.exprList) = new std::vector<Expr>;
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1691 "Parser.tab.c"
    break;

  case 52: /* where_and_clause: %empty  */
#line 355 "./Parser.y"
    {
        (yyval.exprList) = nullptr;
    }
#line 1699 "Parser.tab.c"
    break;

  case 53: /* where_and_clause: WHERE cond_list  */
#line 359 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[0].exprList);
    }
#line 1707 "Parser.tab.c"
    break;

  case 54: /* cond_list: cond_list AND cond_expr  */
#line 364 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[-2].exprList);
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1717 "Parser.tab.c"
    break;

  case 55: /* cond_list: cond_expr  */
#line 370 "./Parser.y"
    {
        (yyval.exprList) = new std::vector<Expr>;
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1727 "Parser.tab.c"
    break;

  case 56: /* cond_expr: col_expr cmp_op val_expr  */
#line 379 "./Parser.y"
    {
        (yyval.expr) = new Expr((yyvsp[-2].expr), (yyvsp[0].expr), (yyvsp[-1].op));
    }
#line 1735 "Parser.tab.c"
    break;

  case 57: /* cond_expr: col_expr cmp_op col_expr  */
#line 383 "./Parser.y"
    {
        (yyval.expr) = new Expr((yyvsp[-2].expr), (yyvsp[0].expr), (yyvsp[-1].op));
    }
#line 1743 "Parser.tab.c"
    break;

  case 58: /* col_expr: IDENTIFIER  */
#line 390 "./Parser.y"
    {
        (yyval.expr) = new Expr((yyvsp[0].str), nullptr);
    }
#line 1751 "Parser.tab.c"
    break;

  case 59: /* col_expr: IDENTIFIER '.' IDENTIFIER  */
#line 394 "./Parser.y"
    {
        (yyval.expr) = new Expr((yyvsp[0].str), (yyvsp[-2].str));
    }
#line 1759 "Parser.tab.c"
    break;

  case 60: /* val_expr: STR_INT  */
#line 400 "./Parser.y"
    {
        (yyval.expr) = new Expr(atoi((yyvsp[0].str)));
    }
#line 1767 "Parser.tab.c"
    break;

  case 61: /* val_expr: STR_FLOAT  */
#line 404 "./Parser.y"
    {
        (yyval.expr) = new Expr((float)(atof((yyvsp[0].str))));
    }
#line 1775 "Parser.tab.c"
    break;

  case 62: /* val_expr: STR_VARCHAR  */
#line 408 "./Parser.y"
    {
        (yyval.expr) = new Expr((yyvsp[0].str), 200);
    }
#line 1783 "Parser.tab.c"
    break;

  case 63: /* val_expr: STR_NULL  */
#line 412 "./Parser.y"
    {
        (yyval.expr) = new Expr(true);
    }
#line 1791 "Parser.tab.c"
    break;

  case 64: /* cmp_op: '<'  */
#line 419 "./Parser.y"
    {
        (yyval.op) = LT;
    }
#line 1799 "Parser.tab.c"
    break;

  case 65: /* cmp_op: '<' '='  */
#line 423 "./Parser.y"
    {
        (yyval.op) = LE;
    }
#line 1807 "Parser.tab.c"
    break;

  case 66: /* cmp_op: '>'  */
#line 427 "./Parser.y"
    {
        (yyval.op) = GT;
    }
#line 1815 "Parser.tab.c"
    break;

  case 67: /* cmp_op: '>' '='  */
#line 431 "./Parser.y"
    {
        (yyval.op) = GE;
    }
#line 1823 "Parser.tab.c"
    break;

  case 68: /* cmp_op: '='  */
#line 435 "./Parser.y"
    {
        (yyval.op) = EQ;
    }
#line 1831 "Parser.tab.c"
    break;

  case 69: /* cmp_op: '<' '>'  */
#line 439 "./Parser.y"
    {
        (yyval.op) = NE;
    }
#line 1839 "Parser.tab.c"
    break;

  case 70: /* set_list: set_list ',' set_expr  */
#line 447 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[-2].exprList);
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1849 "Parser.tab.c"
    break;

  case 71: /* set_list: set_expr  */
#line 453 "./Parser.y"
    {
        (yyval.exprList) = new std::vector<Expr>;
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1859 "Parser.tab.c"
    break;

  case 72: /* set_expr: col_expr assign val_expr  */
#line 462 "./Parser.y"
    {
        (yyval.expr) = new Expr((yyvsp[-2].expr), (yyvsp[0].expr), (yyvsp[-1].op));
    }
#line 1867 "Parser.tab.c"
    break;

  case 73: /* assign: '='  */
#line 469 "./Parser.y"
    {
        (yyval.op) = EQ;
    }
#line 1875 "Parser.tab.c"
    break;

  case 74: /* selectors: '*'  */
#line 476 "./Parser.y"
    {
        (yyval.exprList) = nullptr;
    }
#line 1883 "Parser.tab.c"
    break;

  case 75: /* selectors: column_list  */
#line 480 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[0].exprList);
    }
#line 1891 "Parser.tab.c"
    break;

  case 76: /* column_list: column_list ',' col_expr  */
#line 487 "./Parser.y"
    {
        (yyval.exprList) = (yyvsp[-2].exprList);
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1901 "Parser.tab.c"
    break;

  case 77: /* column_list: col_expr  */
#line 493 "./Parser.y"
    {
        (yyval.exprList) = new std::vector<Expr>;
        (yyval.exprList)->push_back(*(yyvsp[0].expr));
        delete (yyvsp[0].expr);
    }
#line 1911 "Parser.tab.c"
    break;


#line 1915 "Parser.tab.c"

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
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 499 "./Parser.y"


void yyerror(const char *s, ...)
{
     fprintf(stderr, "%s\n", s);
}
