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
#line 9 "subc.y"

/* Prologue section */
#include <stdio.h>
#include <stdlib.h>  /* malloc */
#include <string.h>  /* strlen, memcpy */
#include "subc.h"

int   yylex ();
int   yyerror (char* s);
int   get_lineno();
char* yyget_text();
void  reduce(char* s);

/* [Project 3] Add filename variable for error messages */
char* filename;

/* Function declarations support */
static struct decl *pending_func = NULL;  /* curent function declaration passing */

/* [Project 4] 함수 코드 생성용 */
static char *current_func_name = NULL;     /* 현재 함수명 (복사본) */
static int pending_function_body = 0;      /* 다음 compound_stmt가 함수 body임 */
static int in_function_body = 0;           /* 현재 compound_stmt가 함수 body 처리 중 */
static int local_offset = 0;               /* 지역 변수 offset (fp+1부터 시작) */

/* [Phase 5] 제어흐름 라벨 스택 */
static char *if_label_stack[100];          /* if/else 라벨 스택 */
static int if_label_top = 0;

/* [Phase 5-2] 루프 라벨 스택 (break/continue 지원) */
struct loop_labels {
    char *cond_label;      /* 조건 검사 위치 (while: begin, for: cond) */
    char *continue_label;  /* continue 타겟 (while: cond, for: update) */
    char *break_label;     /* break 타겟 (루프 종료점) */
    char *body_label;      /* for 전용: body 시작 (update 건너뛰기용) */
};
static struct loop_labels loop_stack[100];
static int loop_stack_top = 0;

/* [Project 4] Phase 3.5: need_rvalue 문맥 플래그 */
static int need_rvalue = 1;                /* 기본: 값 필요 */
static int need_rvalue_stack[100];
static int need_rvalue_sp = 0;

#define SAVE_NEED_RVALUE() do { \
  if (need_rvalue_sp >= 100) { fprintf(stderr, "need_rvalue stack overflow\n"); exit(1); } \
  need_rvalue_stack[need_rvalue_sp++] = need_rvalue; \
} while(0)

#define RESTORE_NEED_RVALUE() do { \
  if (need_rvalue_sp <= 0) { fprintf(stderr, "need_rvalue stack underflow\n"); exit(1); } \
  need_rvalue = need_rvalue_stack[--need_rvalue_sp]; \
} while(0)

/* helper function: string duplicate */
static char* my_strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

/* helper function: pointer level add */
static struct decl *apply_pointers(struct decl *base, int levels) {
    int i;
    struct decl *t = base;
    for (i = 0; i < levels && t != NULL; i++) {
        t = makeptrdecl(t);
    }
    return t;
}

#line 144 "subc.tab.c"

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

#include "subc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TYPE = 3,                       /* TYPE  */
  YYSYMBOL_STRUCT = 4,                     /* STRUCT  */
  YYSYMBOL_SYM_NULL = 5,                   /* SYM_NULL  */
  YYSYMBOL_RETURN = 6,                     /* RETURN  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_BREAK = 11,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 12,                  /* CONTINUE  */
  YYSYMBOL_LOGICAL_OR = 13,                /* LOGICAL_OR  */
  YYSYMBOL_LOGICAL_AND = 14,               /* LOGICAL_AND  */
  YYSYMBOL_INCOP = 15,                     /* INCOP  */
  YYSYMBOL_DECOP = 16,                     /* DECOP  */
  YYSYMBOL_STRUCTOP = 17,                  /* STRUCTOP  */
  YYSYMBOL_RELOP = 18,                     /* RELOP  */
  YYSYMBOL_EQUOP = 19,                     /* EQUOP  */
  YYSYMBOL_ID = 20,                        /* ID  */
  YYSYMBOL_CHAR_CONST = 21,                /* CHAR_CONST  */
  YYSYMBOL_STRING = 22,                    /* STRING  */
  YYSYMBOL_INTEGER_CONST = 23,             /* INTEGER_CONST  */
  YYSYMBOL_24_ = 24,                       /* ','  */
  YYSYMBOL_25_ = 25,                       /* '='  */
  YYSYMBOL_26_ = 26,                       /* '+'  */
  YYSYMBOL_27_ = 27,                       /* '-'  */
  YYSYMBOL_28_ = 28,                       /* '*'  */
  YYSYMBOL_29_ = 29,                       /* '/'  */
  YYSYMBOL_30_ = 30,                       /* '%'  */
  YYSYMBOL_31_ = 31,                       /* '!'  */
  YYSYMBOL_UMINUS = 32,                    /* UMINUS  */
  YYSYMBOL_USTAR = 33,                     /* USTAR  */
  YYSYMBOL_UAMP = 34,                      /* UAMP  */
  YYSYMBOL_35_ = 35,                       /* '('  */
  YYSYMBOL_36_ = 36,                       /* ')'  */
  YYSYMBOL_37_ = 37,                       /* '['  */
  YYSYMBOL_38_ = 38,                       /* ']'  */
  YYSYMBOL_39_ = 39,                       /* '.'  */
  YYSYMBOL_LOWER_THAN_ELSE = 40,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_41_ = 41,                       /* ';'  */
  YYSYMBOL_42_ = 42,                       /* '{'  */
  YYSYMBOL_43_ = 43,                       /* '}'  */
  YYSYMBOL_44_ = 44,                       /* '&'  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_program = 46,                   /* program  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_ext_def_list = 48,              /* ext_def_list  */
  YYSYMBOL_ext_def = 49,                   /* ext_def  */
  YYSYMBOL_50_2 = 50,                      /* $@2  */
  YYSYMBOL_type_specifier = 51,            /* type_specifier  */
  YYSYMBOL_struct_specifier = 52,          /* struct_specifier  */
  YYSYMBOL_53_3 = 53,                      /* $@3  */
  YYSYMBOL_func_decl = 54,                 /* func_decl  */
  YYSYMBOL_55_4 = 55,                      /* @4  */
  YYSYMBOL_pointers = 56,                  /* pointers  */
  YYSYMBOL_param_list = 57,                /* param_list  */
  YYSYMBOL_param_decl = 58,                /* param_decl  */
  YYSYMBOL_def_list = 59,                  /* def_list  */
  YYSYMBOL_def = 60,                       /* def  */
  YYSYMBOL_compound_stmt = 61,             /* compound_stmt  */
  YYSYMBOL_62_5 = 62,                      /* @5  */
  YYSYMBOL_63_6 = 63,                      /* $@6  */
  YYSYMBOL_stmt_list = 64,                 /* stmt_list  */
  YYSYMBOL_stmt = 65,                      /* stmt  */
  YYSYMBOL_66_7 = 66,                      /* $@7  */
  YYSYMBOL_67_8 = 67,                      /* $@8  */
  YYSYMBOL_68_9 = 68,                      /* $@9  */
  YYSYMBOL_if_head = 69,                   /* if_head  */
  YYSYMBOL_while_head = 70,                /* while_head  */
  YYSYMBOL_71_10 = 71,                     /* $@10  */
  YYSYMBOL_for_head = 72,                  /* for_head  */
  YYSYMBOL_73_11 = 73,                     /* $@11  */
  YYSYMBOL_expr_e = 74,                    /* expr_e  */
  YYSYMBOL_expr = 75,                      /* expr  */
  YYSYMBOL_assignment = 76,                /* assignment  */
  YYSYMBOL_77_12 = 77,                     /* $@12  */
  YYSYMBOL_binary = 78,                    /* binary  */
  YYSYMBOL_unary = 79,                     /* unary  */
  YYSYMBOL_80_13 = 80,                     /* $@13  */
  YYSYMBOL_args = 81,                      /* args  */
  YYSYMBOL_args_opt = 82                   /* args_opt  */
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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   282


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
       2,     2,     2,    31,     2,     2,     2,    30,    44,     2,
      35,    36,    28,    26,    24,    27,    39,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    41,
       2,    25,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    32,
      33,    34,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   131,   131,   131,   155,   160,   166,   196,   228,   233,
     232,   248,   254,   263,   262,   285,   302,   341,   340,   395,
     401,   408,   412,   419,   441,   468,   473,   479,   508,   543,
     600,   542,   638,   643,   649,   659,   663,   679,   678,   707,
     711,   718,   717,   731,   740,   739,   756,   764,   776,   787,
     786,   808,   807,   840,   846,   857,   865,   871,   870,   924,
     958,   992,  1015,  1038,  1061,  1084,  1107,  1130,  1153,  1168,
    1168,  1174,  1186,  1199,  1212,  1278,  1296,  1314,  1355,  1396,
    1417,  1438,  1456,  1487,  1548,  1584,  1629,  1756,  1771,  1781,
    1803,  1807
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
  "\"end of file\"", "error", "\"invalid token\"", "TYPE", "STRUCT",
  "SYM_NULL", "RETURN", "IF", "ELSE", "WHILE", "FOR", "BREAK", "CONTINUE",
  "LOGICAL_OR", "LOGICAL_AND", "INCOP", "DECOP", "STRUCTOP", "RELOP",
  "EQUOP", "ID", "CHAR_CONST", "STRING", "INTEGER_CONST", "','", "'='",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "UMINUS", "USTAR", "UAMP",
  "'('", "')'", "'['", "']'", "'.'", "LOWER_THAN_ELSE", "';'", "'{'",
  "'}'", "'&'", "$accept", "program", "$@1", "ext_def_list", "ext_def",
  "$@2", "type_specifier", "struct_specifier", "$@3", "func_decl", "@4",
  "pointers", "param_list", "param_decl", "def_list", "def",
  "compound_stmt", "@5", "$@6", "stmt_list", "stmt", "$@7", "$@8", "$@9",
  "if_head", "while_head", "$@10", "for_head", "$@11", "expr_e", "expr",
  "assignment", "$@12", "binary", "unary", "$@13", "args", "args_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-82)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-58)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -82,    11,   -82,   -82,    31,   -82,     0,   -82,    -2,   -13,
     -82,     1,    -2,    18,   -82,     9,   -82,   -82,    -4,   -82,
     -82,   -82,     6,    48,   -82,   -82,     2,   -82,    31,    35,
      31,   -82,    -2,   -82,   -82,    -2,    -6,   -82,    39,   -82,
      54,    63,    31,   -82,   -82,    80,   -12,    51,   -82,   -82,
      52,    59,    62,    64,    57,    65,   151,   151,    69,   -82,
     -82,   -82,   151,   151,   151,   -82,   -82,   -82,   151,   -82,
     -82,   120,   120,   151,    68,   -82,   162,     7,    82,   -82,
      87,   -82,   151,   151,   -82,   151,   -82,   -82,   -82,   -82,
     151,    33,    33,    33,   151,    33,   104,   -82,   -82,   -82,
     -82,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     -82,   -82,    93,   151,    94,   103,    95,    96,    97,   101,
     151,    98,   -82,   121,   108,   110,   -82,   113,   179,    33,
     184,    90,   197,    49,    49,   -82,   -82,   -82,   -82,   112,
     -82,   151,   111,   -82,   -82,   -82,   117,   -82,   151,   -82,
     -82,   120,   120,   -82,   -82,   -82,   -82,   151,   -82,   -82,
     -82,   116,   -82
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     5,     1,     3,    11,     0,     4,    20,    12,
       9,    15,    20,     0,     8,     0,    13,    19,     0,    29,
      10,    26,    17,     0,     6,    26,     0,    16,     0,     0,
      30,    14,    20,    12,    25,    20,     0,    21,     0,    33,
       0,     0,     0,    18,     7,     0,     0,    23,    22,    87,
      37,     0,     0,     0,     0,     0,     0,     0,    74,    72,
      73,    71,     0,     0,     0,    69,    39,    31,     0,    35,
      32,     0,     0,    54,     0,    55,    56,    68,     0,    27,
       0,    36,     0,     0,    49,    54,    46,    47,    79,    80,
      90,    75,    82,    76,     0,    81,    40,    43,    44,    53,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      77,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    91,     0,     0,    41,     0,    67,    68,
      66,    59,    60,    61,    62,    63,    64,    65,    85,     0,
      84,     0,     0,    24,    38,    48,     0,    51,     0,    86,
      70,     0,     0,    83,    58,    28,    50,    54,    89,    42,
      45,     0,    52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -82,   -82,   -82,   -82,   -82,   -82,    -1,   150,   -82,   -82,
     -82,     4,   -82,   118,   133,   -82,   144,   -82,   -82,   -82,
     -70,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   -81,
     -73,    22,   -82,   -40,   -49,   -82,   -82,   -82
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     4,     7,    15,    32,    33,    21,    10,
      28,    13,    36,    37,    26,    34,    69,    25,    39,    45,
      70,    82,   151,   127,    71,    72,   120,    73,   157,    98,
      74,    75,   115,    76,    77,    94,   123,   124
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      99,    96,    97,     8,   121,     5,     6,    88,    89,   118,
     119,     3,    99,    91,    92,    93,    17,   122,    42,    95,
      11,   125,   110,   111,   112,    78,    12,    35,    14,    79,
      43,    22,   -57,    23,     5,     6,    40,    24,    18,    41,
     139,    35,    27,    16,   113,    31,   114,   146,   110,   111,
     112,    19,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   128,   130,   131,   132,   133,   134,   135,   136,   137,
     113,    29,   114,    38,    46,   158,   161,   107,   108,   109,
      44,   159,   160,    47,    99,    49,    50,    51,    80,    52,
      53,    54,    55,    81,    83,    56,    57,    84,    86,    85,
      58,    59,    60,    61,    90,   116,    87,    62,    63,   100,
     117,    64,   126,   138,   140,    65,   105,   106,   107,   108,
     109,    66,    19,    67,    68,    49,    50,    51,   141,    52,
      53,    54,    55,   142,   143,    56,    57,   145,   144,   147,
      58,    59,    60,    61,   149,   148,   150,    62,    63,   152,
     153,    64,   155,   156,     9,    65,    49,   162,    30,    20,
      48,    66,    19,   154,    68,     0,    56,    57,     0,     0,
       0,    58,    59,    60,    61,   101,   102,     0,    62,    63,
     103,   104,    64,     0,     0,     0,    65,     0,   105,   106,
     107,   108,   109,   102,     0,    68,     0,   103,   104,     0,
       0,     0,   103,   104,     0,   105,   106,   107,   108,   109,
     105,   106,   107,   108,   109,   103,     0,     0,     0,     0,
       0,     0,     0,   105,   106,   107,   108,   109
};

static const yytype_int16 yycheck[] =
{
      73,    71,    72,     4,    85,     3,     4,    56,    57,    82,
      83,     0,    85,    62,    63,    64,    12,    90,    24,    68,
      20,    94,    15,    16,    17,    37,    28,    28,    41,    41,
      36,    35,    25,    37,     3,     4,    32,    41,    20,    35,
     113,    42,    36,    42,    37,    43,    39,   120,    15,    16,
      17,    42,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      37,    23,    39,    38,    20,   148,   157,    28,    29,    30,
      41,   151,   152,    20,   157,     5,     6,     7,    37,     9,
      10,    11,    12,    41,    35,    15,    16,    35,    41,    35,
      20,    21,    22,    23,    35,    23,    41,    27,    28,    41,
      23,    31,     8,    20,    20,    35,    26,    27,    28,    29,
      30,    41,    42,    43,    44,     5,     6,     7,    25,     9,
      10,    11,    12,    38,    38,    15,    16,    36,    41,    41,
      20,    21,    22,    23,    36,    24,    36,    27,    28,    36,
      38,    31,    41,    36,     4,    35,     5,    41,    25,    15,
      42,    41,    42,   141,    44,    -1,    15,    16,    -1,    -1,
      -1,    20,    21,    22,    23,    13,    14,    -1,    27,    28,
      18,    19,    31,    -1,    -1,    -1,    35,    -1,    26,    27,
      28,    29,    30,    14,    -1,    44,    -1,    18,    19,    -1,
      -1,    -1,    18,    19,    -1,    26,    27,    28,    29,    30,
      26,    27,    28,    29,    30,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,    47,     0,    48,     3,     4,    49,    51,    52,
      54,    20,    28,    56,    41,    50,    42,    56,    20,    42,
      61,    53,    35,    37,    41,    62,    59,    36,    55,    23,
      59,    43,    51,    52,    60,    51,    57,    58,    38,    63,
      56,    56,    24,    36,    41,    64,    20,    20,    58,     5,
       6,     7,     9,    10,    11,    12,    15,    16,    20,    21,
      22,    23,    27,    28,    31,    35,    41,    43,    44,    61,
      65,    69,    70,    72,    75,    76,    78,    79,    37,    41,
      37,    41,    66,    35,    35,    35,    41,    41,    79,    79,
      35,    79,    79,    79,    80,    79,    65,    65,    74,    75,
      41,    13,    14,    18,    19,    26,    27,    28,    29,    30,
      15,    16,    17,    37,    39,    77,    23,    23,    75,    75,
      71,    74,    75,    81,    82,    75,     8,    68,    78,    79,
      78,    78,    78,    78,    78,    78,    78,    78,    20,    75,
      20,    25,    38,    38,    41,    36,    75,    41,    24,    36,
      36,    67,    36,    38,    76,    41,    36,    73,    75,    65,
      65,    74,    41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    47,    46,    48,    48,    49,    49,    49,    50,
      49,    51,    51,    53,    52,    52,    54,    55,    54,    56,
      56,    57,    57,    58,    58,    59,    59,    60,    60,    62,
      63,    61,    64,    64,    65,    65,    65,    66,    65,    65,
      65,    67,    65,    65,    68,    65,    65,    65,    69,    71,
      70,    73,    72,    74,    74,    75,    76,    77,    76,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    80,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    81,    81,
      82,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     4,     7,     2,     0,
       3,     1,     1,     0,     6,     2,     5,     0,     7,     2,
       0,     1,     3,     3,     6,     2,     0,     4,     7,     0,
       0,     6,     2,     0,     2,     1,     2,     0,     4,     1,
       2,     0,     5,     2,     0,     5,     2,     2,     4,     0,
       5,     0,     7,     1,     0,     1,     1,     0,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     0,
       4,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     4,     3,     3,     4,     1,     1,     3,
       0,     1
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
  case 2: /* $@1: %empty  */
#line 131 "subc.y"
    {
      /* Startup code - main도 함수이므로 caller 템플릿 적용 */
      emit("push_const 0");        /* return slot (main 반환값) */
      emit("push_const EXIT");     /* return address */
      emit("push_reg fp");         /* old_fp */
      emit("push_reg sp");
      emit("pop_reg fp");          /* fp = sp */
      emit("jump main");

      /* EXIT 레이블 (함수들 앞에 위치) */
      emit_label("EXIT");
      emit("exit");
    }
#line 1349 "subc.tab.c"
    break;

  case 3: /* program: $@1 ext_def_list  */
#line 145 "subc.y"
    {
      /* Lglob 및 문자열 테이블 */
      emit("Lglob. data %d", global_words);
      emit_string_table();

      REDUCE("program->ext_def_list");
    }
#line 1361 "subc.tab.c"
    break;

  case 4: /* ext_def_list: ext_def_list ext_def  */
#line 156 "subc.y"
  {
    REDUCE("ext_def_list->ext_def_list ext_def");
  }
#line 1369 "subc.tab.c"
    break;

  case 5: /* ext_def_list: %empty  */
#line 160 "subc.y"
  {
    REDUCE("ext_def_list->epsilon");
  }
#line 1377 "subc.tab.c"
    break;

  case 6: /* ext_def: type_specifier pointers ID ';'  */
#line 167 "subc.y"
  {
    if ((yyvsp[-3].declptr) == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = (yyvsp[-3].declptr);
      int i;  /* C90 compatibility */
      
      /* apply pointer layers */
      for (i = 0; i < (yyvsp[-2].intVal); i++) {
        basetype = makeptrdecl(basetype);
      }
      
      struct decl *vardecl = makevardecl(basetype);
      
      /* redeclaration check */
      if (findcurrentdecl((yyvsp[-1].idptr)) != NULL) {
        error_redeclaration();
      } else {
        /* [Project 4] 전역 변수 offset 할당 */
        vardecl->offset = global_words;
        vardecl->is_global = 1;
        global_words += size_in_words(vardecl);

        declare((yyvsp[-1].idptr), vardecl);
      }
    }
    /* do not use $$ */
    REDUCE("ext_def->type_specifier pointers ID ';'");
  }
#line 1411 "subc.tab.c"
    break;

  case 7: /* ext_def: type_specifier pointers ID '[' INTEGER_CONST ']' ';'  */
#line 197 "subc.y"
  {
    if ((yyvsp[-6].declptr) == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = (yyvsp[-6].declptr);
      int i;  /* C90 compatibility */
      
      /* apply pointer layers */
      for (i = 0; i < (yyvsp[-5].intVal); i++) {
        basetype = makeptrdecl(basetype);
      }
      
      /* array declaration: CONST(ARRAY(VAR)) */
      struct decl *elemvar = makevardecl(basetype);
      struct decl *arraydecl = makearraydecl((yyvsp[-2].intVal), elemvar);
      struct decl *constdecl = makeconstdecl(arraydecl);
      
      if (findcurrentdecl((yyvsp[-4].idptr)) != NULL) {
        error_redeclaration();
      } else {
        /* [Project 4] 전역 배열 offset 할당 */
        constdecl->offset = global_words;
        constdecl->is_global = 1;
        global_words += size_in_words(constdecl);

        declare((yyvsp[-4].idptr), constdecl);
      }
    }
    /* do not use $$ */
    REDUCE("ext_def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'");
  }
#line 1447 "subc.tab.c"
    break;

  case 8: /* ext_def: struct_specifier ';'  */
#line 229 "subc.y"
  {
    REDUCE("ext_def->struct_specifier ';'");
  }
#line 1455 "subc.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 233 "subc.y"
    {
      /* [Project 4] 함수 정의에서만 레이블/pending 설정 */
      if (current_func_name != NULL) {
        emit_label(current_func_name);
        emitted_any_function = 1;
        pending_function_body = 1;
      }
    }
#line 1468 "subc.tab.c"
    break;

  case 10: /* ext_def: func_decl $@2 compound_stmt  */
#line 242 "subc.y"
  {
    REDUCE("ext_def->func_decl compound_stmt");
  }
#line 1476 "subc.tab.c"
    break;

  case 11: /* type_specifier: TYPE  */
#line 249 "subc.y"
  {
    /* lexer에서 이미 inttype/chartype/voidtype을 declptr로 실어줌 */
    (yyval.declptr) = (yyvsp[0].declptr);
    REDUCE("type_specifier->TYPE");
  }
#line 1486 "subc.tab.c"
    break;

  case 12: /* type_specifier: struct_specifier  */
#line 255 "subc.y"
  {
    (yyval.declptr) = (yyvsp[0].declptr);  /* struct_specifier now returns proper declptr */
    REDUCE("type_specifier->struct_specifier");
  }
#line 1495 "subc.tab.c"
    break;

  case 13: /* $@3: %empty  */
#line 263 "subc.y"
  {
    /* struct 재정의를 먼저 검사 */
    if (findstructdecl((yyvsp[-1].idptr)) != NULL) {
      error_redeclaration();
    }
    push_scope();  /* 필드 전용 */
  }
#line 1507 "subc.tab.c"
    break;

  case 14: /* struct_specifier: STRUCT ID '{' $@3 def_list '}'  */
#line 271 "subc.y"
  {
    struct ste *fields = pop_scope();  /* 필드 리스트 수집 */
    
    /* if error -> not entry */
    if (findstructdecl((yyvsp[-4].idptr)) != NULL) {
      (yyval.declptr) = NULL;
    } else {
      struct decl *structdecl = makestructdecl(fields);
      /* struct -> parrent scope */
      declare_struct((yyvsp[-4].idptr), structdecl);
      (yyval.declptr) = structdecl;
    }
    REDUCE("struct_specifier->STRUCT ID '{' def_list '}'");
  }
#line 1526 "subc.tab.c"
    break;

  case 15: /* struct_specifier: STRUCT ID  */
#line 286 "subc.y"
  {
    /* type using -> lookup -> all scope search */
    struct decl *declptr = lookup((yyvsp[0].idptr));
    
    if (!check_is_struct_type(declptr)) {
      /* error_incomplete() already called in check_is_struct_type();
         propagate NULL to type_specifier → ext_def/def → skip declaration */
      (yyval.declptr) = NULL;
    } else {
      (yyval.declptr) = declptr;
    }
    REDUCE("struct_specifier->STRUCT ID");
  }
#line 1544 "subc.tab.c"
    break;

  case 16: /* func_decl: type_specifier pointers ID '(' ')'  */
#line 303 "subc.y"
  {
    pending_func = NULL;  /* prev function clear */
    
    if ((yyvsp[-4].declptr) == NULL) {
      (yyval.declptr) = NULL;  /* type error prop */
    } else {
      /* 반환 타입 생성 */
      struct decl *rettype = apply_pointers((yyvsp[-4].declptr), (yyvsp[-3].intVal));
      
      if (rettype == NULL) {
        (yyval.declptr) = NULL;
      } else {
        /* 함수 declaration 생성 */
        struct decl *funcdecl = makefuncdecl();
        
        /* 재선언 검사 */
        if (findcurrentdecl((yyvsp[-2].idptr)) != NULL) {
          error_redeclaration();
          (yyval.declptr) = NULL;
        } else {
          declare((yyvsp[-2].idptr), funcdecl);
          funcdecl->returntype = rettype;
          funcdecl->formals = NULL;  /* 매개변수 없음 */
          
          /* compound_stmt로 전달 */
          pending_func = funcdecl;

          /* [Project 4] 함수명 저장 */
          if (current_func_name) free(current_func_name);
          current_func_name = my_strdup((yyvsp[-2].idptr)->name);

          (yyval.declptr) = funcdecl;
        }
      }
    }
    REDUCE("func_decl->type_specifier pointers ID '(' ')'");
  }
#line 1586 "subc.tab.c"
    break;

  case 17: /* @4: %empty  */
#line 341 "subc.y"
  {
    pending_func = NULL;  /* 초기화 */
    
    struct decl *rettype = NULL;
    struct decl *funcdecl = NULL;
    
    if ((yyvsp[-3].declptr) != NULL) {
      rettype = apply_pointers((yyvsp[-3].declptr), (yyvsp[-2].intVal));
    }
    
    /* 함수 선언 자체가 유효한지 체크 */
    if ((yyvsp[-3].declptr) == NULL || rettype == NULL) {
      /* type 에러 */
      funcdecl = NULL;
    } else if (findcurrentdecl((yyvsp[-1].idptr)) != NULL) {
      error_redeclaration();
      funcdecl = NULL;
    } else {
      funcdecl = makefuncdecl();
      declare((yyvsp[-1].idptr), funcdecl);
      funcdecl->returntype = rettype;

      /* [Project 4] 함수명 저장 */
      if (current_func_name) free(current_func_name);
      current_func_name = my_strdup((yyvsp[-1].idptr)->name);
    }

    /* 매개변수 수집용 scope는 항상 연다 (균형 보장) */
    push_scope();
    /* returnid는 여기서 declare하지 않음 - formals 전용 scope */
    
    (yyval.declptr) = funcdecl;
  }
#line 1624 "subc.tab.c"
    break;

  case 18: /* func_decl: type_specifier pointers ID '(' @4 param_list ')'  */
#line 375 "subc.y"
  {
    struct decl *funcdecl = (yyvsp[-2].declptr);
    struct ste *formals = pop_scope();  /* 실패든 성공이든 항상 pop */
    
    if (funcdecl == NULL) {
      /* 에러 상태: formals 버리고 NULL 전파 */
      pending_func = NULL;
      (yyval.declptr) = NULL;
    } else {
      /* 정상 처리: formals는 헤더 정보로만 보관 */
      funcdecl->formals = formals;  /* 순수 매개변수 리스트 (reverse) */
      pending_func = funcdecl;       /* compound_stmt로 전달 */
      
      (yyval.declptr) = funcdecl;
    }
    REDUCE("func_decl->type_specifier pointers ID '(' param_list ')'");
  }
#line 1646 "subc.tab.c"
    break;

  case 19: /* pointers: '*' pointers  */
#line 396 "subc.y"
  {
    (yyval.intVal) = (yyvsp[0].intVal) + 1;  /* count pointers */
    REDUCE("pointers->'*' pointers");
  }
#line 1655 "subc.tab.c"
    break;

  case 20: /* pointers: %empty  */
#line 401 "subc.y"
  {
    (yyval.intVal) = 0;  /* no pointer */
    REDUCE("pointers->epsilon");
  }
#line 1664 "subc.tab.c"
    break;

  case 21: /* param_list: param_decl  */
#line 409 "subc.y"
  {
    REDUCE("param_list->param_decl");
  }
#line 1672 "subc.tab.c"
    break;

  case 22: /* param_list: param_list ',' param_decl  */
#line 413 "subc.y"
  {
    REDUCE("param_list->param_list ',' param_decl");
  }
#line 1680 "subc.tab.c"
    break;

  case 23: /* param_decl: type_specifier pointers ID  */
#line 420 "subc.y"
  {
    if ((yyvsp[-2].declptr) == NULL) {
      /* type 에러 전파 - declare x */
    } else {
      struct decl *paramtype = apply_pointers((yyvsp[-2].declptr), (yyvsp[-1].intVal));
      
      if (paramtype == NULL) {
        /* 포인터 생성 실패 */
      } else {
        struct decl *vardecl = makevardecl(paramtype);
        
        /* 매개변수 중복 검사 */
        if (findcurrentdecl((yyvsp[0].idptr)) != NULL) {
          error_redeclaration();
        } else {
          declare((yyvsp[0].idptr), vardecl);
        }
      }
    }
    REDUCE("param_decl->type_specifier pointers ID");
  }
#line 1706 "subc.tab.c"
    break;

  case 24: /* param_decl: type_specifier pointers ID '[' INTEGER_CONST ']'  */
#line 442 "subc.y"
  {
    if ((yyvsp[-5].declptr) == NULL) {
      /* type 에러 전파 */
    } else {
      struct decl *basetype = apply_pointers((yyvsp[-5].declptr), (yyvsp[-4].intVal));
      
      if (basetype == NULL) {
        /* 에러 전파 */
      } else {
        /* 배열 매개변수: 배열 타입 그대로 유지 (포인터 decay 없음) */
        struct decl *elemvar = makevardecl(basetype);
        struct decl *arraydecl = makearraydecl((yyvsp[-1].intVal), elemvar);
        struct decl *paramdecl = makeconstdecl(arraydecl);
        
        if (findcurrentdecl((yyvsp[-3].idptr)) != NULL) {
          error_redeclaration();
        } else {
          declare((yyvsp[-3].idptr), paramdecl);
        }
      }
    }
    REDUCE("param_decl->type_specifier pointers ID '[' INTEGER_CONST ']'");
  }
#line 1734 "subc.tab.c"
    break;

  case 25: /* def_list: def_list def  */
#line 469 "subc.y"
  {
    REDUCE("def_list->def_list def");
  }
#line 1742 "subc.tab.c"
    break;

  case 26: /* def_list: %empty  */
#line 473 "subc.y"
  {
    REDUCE("def_list->epsilon");
  }
#line 1750 "subc.tab.c"
    break;

  case 27: /* def: type_specifier pointers ID ';'  */
#line 480 "subc.y"
  {
    /* same logic as ext_def */
    if ((yyvsp[-3].declptr) == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = (yyvsp[-3].declptr);
      int i;  /* C90 compatibility */

      for (i = 0; i < (yyvsp[-2].intVal); i++) {
        basetype = makeptrdecl(basetype);
      }

      struct decl *vardecl = makevardecl(basetype);

      if (findcurrentdecl((yyvsp[-1].idptr)) != NULL) {
        error_redeclaration();
      } else {
        /* [Project 4] 지역 변수 offset 할당 */
        vardecl->offset = local_offset;
        vardecl->is_global = 0;
        local_offset += size_in_words(vardecl);

        declare((yyvsp[-1].idptr), vardecl);
      }
    }
    /* do not use $$ */
    REDUCE("def->type_specifier pointers ID ';'");
  }
#line 1783 "subc.tab.c"
    break;

  case 28: /* def: type_specifier pointers ID '[' INTEGER_CONST ']' ';'  */
#line 509 "subc.y"
  {
    /* same as ext_def array */
    if ((yyvsp[-6].declptr) == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = (yyvsp[-6].declptr);
      int i;  /* C90 compatibility */

      for (i = 0; i < (yyvsp[-5].intVal); i++) {
        basetype = makeptrdecl(basetype);
      }

      struct decl *elemvar = makevardecl(basetype);
      struct decl *arraydecl = makearraydecl((yyvsp[-2].intVal), elemvar);
      struct decl *constdecl = makeconstdecl(arraydecl);

      if (findcurrentdecl((yyvsp[-4].idptr)) != NULL) {
        error_redeclaration();
      } else {
        /* [Project 4] 지역 배열 offset 할당 */
        constdecl->offset = local_offset;
        constdecl->is_global = 0;
        local_offset += size_in_words(constdecl);

        declare((yyvsp[-4].idptr), constdecl);
      }
    }
    /* do not use $$ */
    REDUCE("def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'");
  }
#line 1818 "subc.tab.c"
    break;

  case 29: /* @5: %empty  */
#line 543 "subc.y"
  {
    push_scope();

    /* [Project 4] 이 compound_stmt가 함수 body인지 저장 */
    (yyval.intVal) = pending_function_body;

    /* [Project 4] 함수 본문 시작 처리 */
    if (pending_function_body) {
      in_function_body = 1;
      pending_function_body = 0;

      /* main_start: 레이블 */
      char start_label[128];
      snprintf(start_label, 128, "%s_start", current_func_name);
      emit_label(start_label);

      /* [Project 4] 지역 변수 offset 초기화 */
      local_offset = 1;  /* fp+1부터 시작 */
    }

    /* 함수 body인 경우 returnid와 매개변수들을 새로 declare */
    if (pending_func != NULL) {
      /* 1) returnid 설치 */
      declare(returnid, pending_func->returntype);

      /* 현재 함수의 반환 타입 추적 */
      current_func_ret_type = pending_func->returntype;

      /* 2) formals 리스트 순회하며 매개변수 설치
       * 주의: pop_scope()가 리스트를 뒤집으므로 formals는 정순 (a→b→c)
       * 스택 레이아웃: arg_0(a)이 가장 먼저 push되어 fp-2-n에 위치
       *              arg_{n-1}(c)이 마지막 push되어 fp-3에 위치 */
      /* [Project 4] 매개변수 offset 설정 */

      /* 먼저 매개변수 개수 세기 */
      int num_params = 0;
      struct ste *p = pending_func->formals;
      while (p != NULL) { num_params++; p = p->prev; }

      /* offset 할당: arg_0은 fp-(2+n), arg_{n-1}은 fp-3 */
      int param_offset = -(2 + num_params);  /* arg_0부터 시작 */
      p = pending_func->formals;
      while (p != NULL) {
        p->decl->offset = param_offset;
        p->decl->is_global = 0;  /* 지역 변수 */

        /* 새로운 ste를 body scope에 생성 */
        declare(p->name, p->decl);

        param_offset++;  /* 증가 (arg_0 → arg_1 → ... → arg_{n-1}) */
        p = p->prev;
      }

      pending_func = NULL;  /* 사용 완료 */
    }
  }
#line 1879 "subc.tab.c"
    break;

  case 30: /* $@6: %empty  */
#line 600 "subc.y"
  {
    /* [Project 4] Mid-rule: emit shift_sp for local variables after def_list */
    /* $<intVal>2: 첫 번째 mid-rule action의 값 (함수 body 여부) */
    if ((yyvsp[-1].intVal)) {
      int num_locals = local_offset - 1;  /* -1 because we started at 1 */
      if (num_locals > 0) {
        /* 스택이 위로 자람 (PUSH: sp++). 공간 확보는 sp 증가이므로 양수 사용 */
        emit("shift_sp %d", num_locals);
      }
    }
  }
#line 1895 "subc.tab.c"
    break;

  case 31: /* compound_stmt: '{' @5 def_list $@6 stmt_list '}'  */
#line 612 "subc.y"
  {
    /* [Project 4] 함수 본문 종료 처리 */
    /* $<intVal>2: 첫 번째 mid-rule action의 값 (함수 body 여부) */
    if ((yyvsp[-4].intVal)) {
      char final_label[128], end_label[128];
      snprintf(final_label, 128, "%s_final", current_func_name);
      snprintf(end_label, 128, "%s_end", current_func_name);

      emit_label(final_label);
      emit("push_reg fp");
      emit("pop_reg sp");
      emit("pop_reg fp");
      emit("pop_reg pc");
      emit_label(end_label);

      current_func_ret_type = NULL;
      in_function_body = 0;
    }

    pop_scope();

    REDUCE("compound_stmt->'{' def_list stmt_list '}'");
  }
#line 1923 "subc.tab.c"
    break;

  case 32: /* stmt_list: stmt_list stmt  */
#line 639 "subc.y"
  {
    REDUCE("stmt_list->stmt_list stmt");
  }
#line 1931 "subc.tab.c"
    break;

  case 33: /* stmt_list: %empty  */
#line 643 "subc.y"
  {
    REDUCE("stmt_list->epsilon");
  }
#line 1939 "subc.tab.c"
    break;

  case 34: /* stmt: expr ';'  */
#line 650 "subc.y"
  {
    /* [Project 4] Discard expression result */
    /* void 타입이면 스택에 값이 없으므로 shift_sp 생략 */
    if ((yyvsp[-1].expr).type != NULL && (yyvsp[-1].expr).type != voidtype) {
      emit("shift_sp -1");
    }

    REDUCE("stmt->expr ';'");
  }
#line 1953 "subc.tab.c"
    break;

  case 35: /* stmt: compound_stmt  */
#line 660 "subc.y"
  {
    REDUCE("stmt->compound_stmt");
  }
#line 1961 "subc.tab.c"
    break;

  case 36: /* stmt: RETURN ';'  */
#line 664 "subc.y"
  {
    /* current_func_ret_type 사용 */
    if (current_func_ret_type != NULL) {
      if (current_func_ret_type != voidtype) {
        error_return();  /* void가 아닌 함수에서 값 없이 return */
      }
    }
    /* [Project 4] void return: 바로 종료 라벨로 점프 */
    if (current_func_name != NULL) {
      emit("jump %s_final", current_func_name);
    }
    /* current_func_ret_type == NULL: 함수 밖 return - 조용히 무시 */
    REDUCE("stmt->RETURN ';'");
  }
#line 1980 "subc.tab.c"
    break;

  case 37: /* $@7: %empty  */
#line 679 "subc.y"
  {
    /* [Project 4] Mid-rule: return slot 주소를 먼저 생성
     * 스택 순서: [..., addr] → expr 평가 후 [..., addr, value] */
    if (current_func_name != NULL) {
      emit("push_reg fp");
      emit("push_const -2");
      emit("add");  /* stack top = fp-2 (return slot) */
    }
  }
#line 1994 "subc.tab.c"
    break;

  case 38: /* stmt: RETURN $@7 expr ';'  */
#line 689 "subc.y"
  {
    /* current_func_ret_type 사용 */
    /* 에러 전파: $3.type == NULL이면 expr에서 이미 에러 */
    if (current_func_ret_type != NULL && (yyvsp[-1].expr).type != NULL) {
      if (current_func_ret_type == voidtype) {
        error_return();  /* void 함수에서 값 반환 */
      } else if (!check_same_type(current_func_ret_type, (yyvsp[-1].expr).type)) {
        error_return();  /* 타입 불일치 */
      }
    }
    /* [Project 4] 반환값을 return slot에 저장하고 종료 라벨로 점프 */
    if (current_func_name != NULL) {
      emit("assign");  /* mem[fp-2] = value */
      emit("jump %s_final", current_func_name);
    }
    /* current_func_ret_type == NULL: 함수 밖 return - 조용히 무시 */
    REDUCE("stmt->RETURN expr ';'");
  }
#line 2017 "subc.tab.c"
    break;

  case 39: /* stmt: ';'  */
#line 708 "subc.y"
  {
    REDUCE("stmt->';'");
  }
#line 2025 "subc.tab.c"
    break;

  case 40: /* stmt: if_head stmt  */
#line 712 "subc.y"
    {
      /* [Phase 5-1] Simple if: emit else_label */
      emit_label(if_label_stack[--if_label_top]);
      REDUCE("stmt->IF '(' expr ')' stmt");
    }
#line 2035 "subc.tab.c"
    break;

  case 41: /* $@8: %empty  */
#line 718 "subc.y"
    {
      /* [Phase 5-1] if-else: emit jump to end, emit else_label, push end_label */
      char *end_label = new_label("Lend_");
      emit("jump %s", end_label);
      emit_label(if_label_stack[--if_label_top]);
      if_label_stack[if_label_top++] = end_label;
    }
#line 2047 "subc.tab.c"
    break;

  case 42: /* stmt: if_head stmt ELSE $@8 stmt  */
#line 726 "subc.y"
    {
      /* Emit end_label */
      emit_label(if_label_stack[--if_label_top]);
      REDUCE("stmt->IF '(' expr ')' stmt ELSE stmt");
    }
#line 2057 "subc.tab.c"
    break;

  case 43: /* stmt: while_head stmt  */
#line 732 "subc.y"
    {
      /* [Phase 5-2] 루프 끝: jump back + end label */
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
      emit_label(loop_stack[loop_stack_top-1].break_label);
      loop_stack_top--;
      REDUCE("stmt->WHILE '(' expr ')' stmt");
    }
#line 2069 "subc.tab.c"
    break;

  case 44: /* $@9: %empty  */
#line 740 "subc.y"
    {
      /* [Phase 5-3] update expr 결과 버림 (있으면), jump cond, body 라벨 */
      if ((yyvsp[0].expr).type != NULL) {
        emit("shift_sp -1");
      }
      emit("jump %s", loop_stack[loop_stack_top-1].cond_label);
      emit_label(loop_stack[loop_stack_top-1].body_label);
    }
#line 2082 "subc.tab.c"
    break;

  case 45: /* stmt: for_head expr_e $@9 ')' stmt  */
#line 749 "subc.y"
    {
      /* body 끝: jump update, end 라벨 */
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
      emit_label(loop_stack[loop_stack_top-1].break_label);
      loop_stack_top--;
      REDUCE("stmt->FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt");
    }
#line 2094 "subc.tab.c"
    break;

  case 46: /* stmt: BREAK ';'  */
#line 757 "subc.y"
  {
    /* [Phase 5-4] break: 현재 루프의 break_label로 점프 */
    if (loop_stack_top > 0) {
      emit("jump %s", loop_stack[loop_stack_top-1].break_label);
    }
    REDUCE("stmt->BREAK ';'");
  }
#line 2106 "subc.tab.c"
    break;

  case 47: /* stmt: CONTINUE ';'  */
#line 765 "subc.y"
  {
    /* [Phase 5-4] continue: 현재 루프의 continue_label로 점프 */
    if (loop_stack_top > 0) {
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
    }
    REDUCE("stmt->CONTINUE ';'");
  }
#line 2118 "subc.tab.c"
    break;

  case 48: /* if_head: IF '(' expr ')'  */
#line 777 "subc.y"
    {
      char *label = new_label("Lelse_");
      if_label_stack[if_label_top++] = label;
      emit("branch_false %s", label);
    }
#line 2128 "subc.tab.c"
    break;

  case 49: /* $@10: %empty  */
#line 787 "subc.y"
    {
      /* 루프 시작 라벨 생성 및 emit */
      char *begin_label = new_label("Lwhile_");
      char *end_label = new_label("Lend_");
      loop_stack[loop_stack_top].cond_label = begin_label;
      loop_stack[loop_stack_top].continue_label = begin_label;  /* while: continue→조건 */
      loop_stack[loop_stack_top].break_label = end_label;
      loop_stack[loop_stack_top].body_label = NULL;  /* while은 body_label 미사용 */
      loop_stack_top++;
      emit_label(begin_label);
    }
#line 2144 "subc.tab.c"
    break;

  case 50: /* while_head: WHILE '(' $@10 expr ')'  */
#line 799 "subc.y"
    {
      /* 조건 평가 후 branch_false */
      emit("branch_false %s", loop_stack[loop_stack_top-1].break_label);
    }
#line 2153 "subc.tab.c"
    break;

  case 51: /* $@11: %empty  */
#line 808 "subc.y"
    {
      /* init expr 결과 버림 (있으면) */
      if ((yyvsp[-1].expr).type != NULL) {
        emit("shift_sp -1");
      }
      /* 라벨 4개 생성 */
      char *cond_label = new_label("Lcond_");
      char *update_label = new_label("Lupdate_");
      char *body_label = new_label("Lbody_");
      char *end_label = new_label("Lend_");
      loop_stack[loop_stack_top].cond_label = cond_label;
      loop_stack[loop_stack_top].continue_label = update_label;  /* for: continue→update */
      loop_stack[loop_stack_top].break_label = end_label;
      loop_stack[loop_stack_top].body_label = body_label;
      loop_stack_top++;
      /* cond 라벨 emit */
      emit_label(cond_label);
    }
#line 2176 "subc.tab.c"
    break;

  case 52: /* for_head: FOR '(' expr_e ';' $@11 expr_e ';'  */
#line 827 "subc.y"
    {
      /* cond expr 처리 (없으면 push_const 1) */
      if ((yyvsp[-1].expr).type == NULL) {
        emit("push_const 1");
      }
      emit("branch_false %s", loop_stack[loop_stack_top-1].break_label);
      emit("jump %s", loop_stack[loop_stack_top-1].body_label);
      /* update 라벨 emit - 여기서부터 update 코드 */
      emit_label(loop_stack[loop_stack_top-1].continue_label);
    }
#line 2191 "subc.tab.c"
    break;

  case 53: /* expr_e: expr  */
#line 841 "subc.y"
  {
    (yyval.expr) = (yyvsp[0].expr);
    REDUCE("expr_e->expr");
  }
#line 2200 "subc.tab.c"
    break;

  case 54: /* expr_e: %empty  */
#line 846 "subc.y"
  {
    (yyval.expr).type = NULL;
    (yyval.expr).is_lvalue = 0;
    (yyval.expr).is_var = 0;
    (yyval.expr).is_null_const = 0;
    (yyval.expr).next = NULL;
    REDUCE("expr_e->epsilon");
  }
#line 2213 "subc.tab.c"
    break;

  case 55: /* expr: assignment  */
#line 858 "subc.y"
  {
    (yyval.expr) = (yyvsp[0].expr);
    REDUCE("expr->assignment");
  }
#line 2222 "subc.tab.c"
    break;

  case 56: /* assignment: binary  */
#line 866 "subc.y"
  {
    (yyval.expr) = (yyvsp[0].expr);
    REDUCE("assignment->binary");
  }
#line 2231 "subc.tab.c"
    break;

  case 57: /* $@12: %empty  */
#line 871 "subc.y"
    {
      /* [Project 4] Mid-rule: LHS 주소 복제 */
      emit("push_reg sp");
      emit("fetch");
    }
#line 2241 "subc.tab.c"
    break;

  case 58: /* assignment: unary $@12 '=' assignment  */
#line 877 "subc.y"
  {
    /* [Project 4] RHS는 이미 r-value (binary:unary에서 처리됨) */
    emit("assign");
    emit("fetch");  /* assignment result */

    /* Semantic analysis: LHS=$1, RHS=$4 */
    if ((yyvsp[-3].expr).type == NULL || ((yyvsp[0].expr).type == NULL && !(yyvsp[0].expr).is_null_const)) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!(yyvsp[-3].expr).is_lvalue) {
      error_assignable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if ((yyvsp[0].expr).is_null_const) {
      if (!is_pointer_type((yyvsp[-3].expr).type)) {
        error_null();
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
      } else {
        (yyval.expr).type = (yyvsp[-3].expr).type;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
      }
    } else if (!check_same_type((yyvsp[-3].expr).type, (yyvsp[0].expr).type)) {
      error_incompatible();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr).type = (yyvsp[-3].expr).type;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("assignment->unary '=' assignment");
  }
#line 2290 "subc.tab.c"
    break;

  case 59: /* binary: binary RELOP binary  */
#line 925 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!check_same_type((yyvsp[-2].expr).type, (yyvsp[0].expr).type)) {
      error_comparable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) && !is_char_type((yyvsp[-2].expr).type)) {
      error_comparable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 비교 연산 */
      switch ((yyvsp[-1].intVal)) {
        case OP_LT: emit("less"); break;
        case OP_LE: emit("less_equal"); break;
        case OP_GT: emit("greater"); break;
        case OP_GE: emit("greater_equal"); break;
      }
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary RELOP binary");
  }
#line 2328 "subc.tab.c"
    break;

  case 60: /* binary: binary EQUOP binary  */
#line 959 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!check_same_type((yyvsp[-2].expr).type, (yyvsp[0].expr).type)) {
      error_comparable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) && 
               !is_char_type((yyvsp[-2].expr).type) && 
               !is_pointer_type((yyvsp[-2].expr).type)) {
      error_comparable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 동등 비교 */
      switch ((yyvsp[-1].intVal)) {
        case OP_EQ: emit("equal"); break;
        case OP_NE: emit("not_equal"); break;
      }
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary EQUOP binary");
  }
#line 2366 "subc.tab.c"
    break;

  case 61: /* binary: binary '+' binary  */
#line 993 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 덧셈 */
      emit("add");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary '+' binary");
  }
#line 2393 "subc.tab.c"
    break;

  case 62: /* binary: binary '-' binary  */
#line 1016 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 뺄셈 */
      emit("sub");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary '-' binary");
  }
#line 2420 "subc.tab.c"
    break;

  case 63: /* binary: binary '*' binary  */
#line 1039 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 곱셈 */
      emit("mul");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary '*' binary");
  }
#line 2447 "subc.tab.c"
    break;

  case 64: /* binary: binary '/' binary  */
#line 1062 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 나눗셈 */
      emit("div");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary '/' binary");
  }
#line 2474 "subc.tab.c"
    break;

  case 65: /* binary: binary '%' binary  */
#line 1085 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 나머지 */
      emit("mod");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary '%' binary");
  }
#line 2501 "subc.tab.c"
    break;

  case 66: /* binary: binary LOGICAL_AND binary  */
#line 1108 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 논리 AND */
      emit("and");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary LOGICAL_AND binary");
  }
#line 2528 "subc.tab.c"
    break;

  case 67: /* binary: binary LOGICAL_OR binary  */
#line 1131 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL || (yyvsp[0].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-2].expr).type) || !is_int_type((yyvsp[0].expr).type)) {
      error_binary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* [Project 4] 논리 OR */
      emit("or");
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("binary->binary LOGICAL_OR binary");
  }
#line 2555 "subc.tab.c"
    break;

  case 68: /* binary: unary  */
#line 1154 "subc.y"
  {
    /* [Project 4] l-value → r-value 변환
     * need_rvalue=1이고 l-value면 fetch하여 값으로 변환
     */
    (yyval.expr) = (yyvsp[0].expr);
    if (need_rvalue && (yyval.expr).is_lvalue) {
      emit("fetch");
      (yyval.expr).is_lvalue = 0;
    }
    REDUCE("binary->unary");
  }
#line 2571 "subc.tab.c"
    break;

  case 69: /* $@13: %empty  */
#line 1168 "subc.y"
        { SAVE_NEED_RVALUE(); }
#line 2577 "subc.tab.c"
    break;

  case 70: /* unary: '(' $@13 expr ')'  */
#line 1169 "subc.y"
  {
    RESTORE_NEED_RVALUE();
    (yyval.expr) = (yyvsp[-1].expr);  /* $2는 mid-rule, expr은 $3 */
    REDUCE("unary->'(' expr ')'");
  }
#line 2587 "subc.tab.c"
    break;

  case 71: /* unary: INTEGER_CONST  */
#line 1175 "subc.y"
  {
    /* [Project 4] Emit constant value */
    emit("push_const %d", (yyvsp[0].intVal));

    (yyval.expr).type = inttype;
    (yyval.expr).is_lvalue = 0;
    (yyval.expr).is_var = 0;
    (yyval.expr).is_null_const = 0;
    (yyval.expr).next = NULL;
    REDUCE("unary->INTEGER_CONST");
  }
#line 2603 "subc.tab.c"
    break;

  case 72: /* unary: CHAR_CONST  */
#line 1187 "subc.y"
  {
    /* DEBUG: Print CHAR_CONST type */
    /* fprintf(stderr, "DEBUG: CHAR_CONST - chartype=%p (declclass=%d, typeclass=%d)\n",
            (void*)chartype, chartype ? chartype->declclass : -1,
            chartype ? chartype->typeclass : -1); */
    (yyval.expr).type = chartype;
    (yyval.expr).is_lvalue = 0;
    (yyval.expr).is_var = 0;
    (yyval.expr).is_null_const = 0;
    (yyval.expr).next = NULL;
    REDUCE("unary->CHAR_CONST");
  }
#line 2620 "subc.tab.c"
    break;

  case 73: /* unary: STRING  */
#line 1200 "subc.y"
  {
    /* [Project 4] 문자열 리터럴: 테이블에 추가하고 라벨 push */
    char *label = add_string((yyvsp[0].stringVal));
    emit("push_const %s", label);

    (yyval.expr).type = makeptrdecl(chartype);  /* char* 타입 생성 */
    (yyval.expr).is_lvalue = 0;
    (yyval.expr).is_var = 0;
    (yyval.expr).is_null_const = 0;
    (yyval.expr).next = NULL;
    REDUCE("unary->STRING");
  }
#line 2637 "subc.tab.c"
    break;

  case 74: /* unary: ID  */
#line 1213 "subc.y"
  {
    struct decl *d = lookup((yyvsp[0].idptr));
    if (d == NULL) {
      error_undeclared();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
      (yyval.expr).next = NULL;
    } else if (d->declclass == DECL_VAR) {
      /* [Project 4] Emit l-value (address) for variable */
      if (d->is_global) {
        emit("push_const Lglob+%d", d->offset);
      } else {
        /* local variable: fp + offset */
        emit("push_reg fp");
        emit("push_const %d", d->offset);
        emit("add");
      }

      struct decl *t = get_type(d);
      (yyval.expr).type = t;
      (yyval.expr).is_lvalue = 1;

      /* & 연산자는 포인터와 배열에 대해서만 금지 */
      if (is_pointer_type(t) || is_array_type(t)) {
        (yyval.expr).is_var = 0;  /* &p, &arr 금지 */
      } else {
        (yyval.expr).is_var = 1;  /* int, char, struct 허용 */
      }

      (yyval.expr).is_null_const = 0;
      (yyval.expr).next = NULL;
    } else if (d->declclass == DECL_CONST) {
      /* [Project 4] Array: emit base address as r-value */
      if (d->is_global) {
        emit("push_const Lglob+%d", d->offset);
      } else {
        emit("push_reg fp");
        emit("push_const %d", d->offset);
        emit("add");
      }

      (yyval.expr).type = d->type;
      (yyval.expr).is_lvalue = 0;   /* 배열은 lvalue 아님 */
      (yyval.expr).is_var = 0;      /* &arr 불가 */
      (yyval.expr).is_null_const = 0;
      (yyval.expr).next = NULL;
    } else if (d->declclass == DECL_FUNC) {
      /* 함수 이름은 함수 decl 자체를 type으로 - 코드 생성 없음 */
      (yyval.expr).type = d;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
      (yyval.expr).next = NULL;
    } else {
      error_undeclared();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
      (yyval.expr).next = NULL;
    }
    REDUCE("unary->ID");
  }
#line 2707 "subc.tab.c"
    break;

  case 75: /* unary: '-' unary  */
#line 1279 "subc.y"
  {
    if ((yyvsp[0].expr).type == NULL) {
      (yyval.expr) = (yyvsp[0].expr);  /* 에러 전파 */
    } else if (!is_int_or_char_type((yyvsp[0].expr).type)) {
      error_unary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr).type = (yyvsp[0].expr).type;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("unary->'-' unary");
  }
#line 2729 "subc.tab.c"
    break;

  case 76: /* unary: '!' unary  */
#line 1297 "subc.y"
  {
    if ((yyvsp[0].expr).type == NULL) {
      (yyval.expr) = (yyvsp[0].expr);
    } else if (!is_int_type((yyvsp[0].expr).type)) {
      error_unary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr).type = inttype;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("unary->'!' unary");
  }
#line 2751 "subc.tab.c"
    break;

  case 77: /* unary: unary INCOP  */
#line 1315 "subc.y"
  {
    /* [Project 4] Post-increment: result = old value
     * Stack before: [addr]
     * Pattern: dup addr twice, fetch old, add 1, assign, fetch new, sub 1
     */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr] */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr, addr] */
    emit("fetch");      /* [addr, addr, old] */
    emit("push_const 1");
    emit("add");        /* [addr, addr, new] */
    emit("assign");     /* [addr] */
    emit("fetch");      /* [new] */
    emit("push_const 1");
    emit("sub");        /* [old] - result */

    /* Semantic analysis */
    if ((yyvsp[-1].expr).type == NULL) {
      (yyval.expr) = (yyvsp[-1].expr);
    } else if (!(yyvsp[-1].expr).is_lvalue) {
      error_assignable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_or_char_type((yyvsp[-1].expr).type)) {
      error_unary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr).type = (yyvsp[-1].expr).type;
      (yyval.expr).is_lvalue = 0;  /* result is r-value */
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("unary->unary INCOP");
  }
#line 2796 "subc.tab.c"
    break;

  case 78: /* unary: unary DECOP  */
#line 1356 "subc.y"
  {
    /* [Project 4] Post-decrement: result = old value
     * Stack before: [addr]
     * Pattern: dup addr twice, fetch old, sub 1, assign, fetch new, add 1
     */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr] */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr, addr] */
    emit("fetch");      /* [addr, addr, old] */
    emit("push_const 1");
    emit("sub");        /* [addr, addr, new] */
    emit("assign");     /* [addr] */
    emit("fetch");      /* [new] */
    emit("push_const 1");
    emit("add");        /* [old] - result */

    /* Semantic analysis */
    if ((yyvsp[-1].expr).type == NULL) {
      (yyval.expr) = (yyvsp[-1].expr);
    } else if (!(yyvsp[-1].expr).is_lvalue) {
      error_assignable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_or_char_type((yyvsp[-1].expr).type)) {
      error_unary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr).type = (yyvsp[-1].expr).type;
      (yyval.expr).is_lvalue = 0;  /* result is r-value */
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("unary->unary DECOP");
  }
#line 2841 "subc.tab.c"
    break;

  case 79: /* unary: INCOP unary  */
#line 1397 "subc.y"
  {
    if ((yyvsp[0].expr).type == NULL) {
      (yyval.expr) = (yyvsp[0].expr);
    } else if (!(yyvsp[0].expr).is_lvalue) {
      error_assignable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_or_char_type((yyvsp[0].expr).type)) {
      error_unary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr) = (yyvsp[0].expr);  /* 타입 변화 없음 */
    }
    REDUCE("unary->INCOP unary");
  }
#line 2866 "subc.tab.c"
    break;

  case 80: /* unary: DECOP unary  */
#line 1418 "subc.y"
  {
    if ((yyvsp[0].expr).type == NULL) {
      (yyval.expr) = (yyvsp[0].expr);
    } else if (!(yyvsp[0].expr).is_lvalue) {
      error_assignable();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_or_char_type((yyvsp[0].expr).type)) {
      error_unary();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr) = (yyvsp[0].expr);  /* 타입 변화 없음 */
    }
    REDUCE("unary->DECOP unary");
  }
#line 2891 "subc.tab.c"
    break;

  case 81: /* unary: '&' unary  */
#line 1439 "subc.y"
  {
    if ((yyvsp[0].expr).type == NULL) {
      (yyval.expr) = (yyvsp[0].expr);
    } else if (!(yyvsp[0].expr).is_var) {
      error_addressof();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      (yyval.expr).type = makeptrdecl((yyvsp[0].expr).type);
      (yyval.expr).is_lvalue = 0;  /* &a는 rvalue */
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("unary->'&' unary");
  }
#line 2913 "subc.tab.c"
    break;

  case 82: /* unary: '*' unary  */
#line 1457 "subc.y"
  {
    /* [Project 4] 포인터 역참조
     * - operand가 l-value면 fetch로 포인터 값 획득
     * - operand가 r-value면 이미 포인터 값이므로 그대로
     * - 결과는 항상 l-value (포인터 값이 새 주소가 됨)
     * - semantic error 경로에서는 emit 하지 않음
     */
    if ((yyvsp[0].expr).type == NULL) {
      (yyval.expr) = (yyvsp[0].expr);
    } else if (!is_pointer_type((yyvsp[0].expr).type)) {
      error_indirection();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* 정상 경로: 코드 생성 */
      if ((yyvsp[0].expr).is_lvalue) {
        emit("fetch");  /* 포인터 값 획득 */
      }
      /* 결과: 스택에 포인터 값(=주소)이 있고, 이것이 *E의 l-value */

      struct decl *pointee = get_type((yyvsp[0].expr).type)->ptrto;
      (yyval.expr).type = pointee;
      (yyval.expr).is_lvalue = 1;  /* *p는 lvalue */
      (yyval.expr).is_var = 1;     /* &(*p)도 가능 */
      (yyval.expr).is_null_const = 0;
    }
    REDUCE("unary->'*' unary");
  }
#line 2948 "subc.tab.c"
    break;

  case 83: /* unary: unary '[' expr ']'  */
#line 1488 "subc.y"
  {
    /* [Project 4] 배열 접근 코드 생성
     * Stack before: [base_addr, index_value_or_addr]
     * 계산: base + index * element_size
     * Stack after: [element_addr] (l-value)
     * - semantic error 경로에서는 emit 하지 않음
     */
    if ((yyvsp[-3].expr).type == NULL || (yyvsp[-1].expr).type == NULL) {
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_array_type((yyvsp[-3].expr).type)) {
      error_array();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else if (!is_int_type((yyvsp[-1].expr).type)) {
      error_subscript();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* 정상 경로: 코드 생성 */
      struct decl *elem = get_type((yyvsp[-3].expr).type)->elementvar;

      /* elem == NULL 방어 */
      if (!elem) {
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
      } else {
        int elem_size = size_in_words(elem);

        /* index가 l-value면 fetch하여 값으로 변환 */
        if ((yyvsp[-1].expr).is_lvalue) {
          emit("fetch");
        }

        /* element size가 1이 아니면 곱셈 */
        if (elem_size != 1) {
          emit("push_const %d", elem_size);
          emit("mul");
        }

        /* base + offset = element 주소 */
        emit("add");

        /* 결과는 l-value */
        (yyval.expr).type = elem->type;
        (yyval.expr).is_lvalue = 1;  /* arr[i]는 lvalue */
        (yyval.expr).is_var = 1;     /* &arr[i] 가능 */
        (yyval.expr).is_null_const = 0;
      }
    }
    REDUCE("unary->unary '[' expr ']'");
  }
#line 3013 "subc.tab.c"
    break;

  case 84: /* unary: unary '.' ID  */
#line 1549 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL) {
      (yyval.expr) = (yyvsp[-2].expr);
    } else if (!is_struct_type((yyvsp[-2].expr).type)) {
      error_struct();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      /* struct member 찾기 */
      struct ste *field = get_type((yyvsp[-2].expr).type)->fieldlist;
      struct decl *member_decl = NULL;
      while (field != NULL) {
        if (field->name == (yyvsp[0].idptr)) {
          member_decl = field->decl;
          break;
        }
        field = field->prev;
      }
      if (member_decl == NULL) {
        error_member();
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
      } else {
        (yyval.expr).type = member_decl->type;  /* VAR의 type */
        (yyval.expr).is_lvalue = (yyvsp[-2].expr).is_lvalue;  /* struct가 lvalue면 member도 lvalue */
        (yyval.expr).is_var = (yyvsp[-2].expr).is_lvalue;      /* lvalue member는 &가능 */
        (yyval.expr).is_null_const = 0;
      }
    }
    REDUCE("unary->unary '.' ID");
  }
#line 3053 "subc.tab.c"
    break;

  case 85: /* unary: unary STRUCTOP ID  */
#line 1585 "subc.y"
  {
    if ((yyvsp[-2].expr).type == NULL) {
      (yyval.expr) = (yyvsp[-2].expr);
    } else if (!is_pointer_type((yyvsp[-2].expr).type)) {
      error_strurctp();
      (yyval.expr).type = NULL;
      (yyval.expr).is_lvalue = 0;
      (yyval.expr).is_var = 0;
      (yyval.expr).is_null_const = 0;
    } else {
      struct decl *pointee = get_type((yyvsp[-2].expr).type)->ptrto;
      if (!is_struct_type(pointee)) {
        error_strurctp();
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
      } else {
        /* struct member 찾기 */
        struct ste *field = get_type(pointee)->fieldlist;
        struct decl *member_decl = NULL;
        while (field != NULL) {
          if (field->name == (yyvsp[0].idptr)) {
            member_decl = field->decl;
            break;
          }
          field = field->prev;
        }
        if (member_decl == NULL) {
          error_member();
          (yyval.expr).type = NULL;
          (yyval.expr).is_lvalue = 0;
          (yyval.expr).is_var = 0;
          (yyval.expr).is_null_const = 0;
        } else {
          (yyval.expr).type = member_decl->type;  /* VAR의 type */
          (yyval.expr).is_lvalue = 1;  /* p->member는 항상 lvalue */
          (yyval.expr).is_var = 1;      /* &(p->member) 가능 */
          (yyval.expr).is_null_const = 0;
        }
      }
    }
    REDUCE("unary->unary STRUCTOP ID");
  }
#line 3102 "subc.tab.c"
    break;

  case 86: /* unary: ID '(' args_opt ')'  */
#line 1630 "subc.y"
  {
    /* [Project 4] 함수 호출: ID로 직접 호출 */
    struct id *fname = (yyvsp[-3].idptr);
    struct decl *callee = lookup(fname);

    /* 인자 리스트에 에러가 있는지 확인 */
    struct exprinfo *it = (yyvsp[-1].exprlist);
    int has_arg_error = 0;
    while (it != NULL) {
        if (it->type == NULL) {
            has_arg_error = 1;
            break;
        }
        it = it->next;
    }

    if (has_arg_error) {
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
        (yyval.expr).next = NULL;
    } else if (callee == NULL) {
        error_undeclared();
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
        (yyval.expr).next = NULL;
    } else if (callee->declclass != DECL_FUNC) {
        error_function();
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
        (yyval.expr).next = NULL;
    } else if (!check_function_arguments(callee, (yyvsp[-1].exprlist))) {
        error_arguments();
        (yyval.expr).type = NULL;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
        (yyval.expr).next = NULL;
    } else {
        /* 호출 성공 */
        /* [Project 4] built-in 함수 처리 */
        if (strcmp(fname->name, "write_string") == 0) {
            emit("write_string");
        } else if (strcmp(fname->name, "write_int") == 0) {
            emit("write_int");
        } else if (strcmp(fname->name, "write_char") == 0) {
            emit("write_char");
        } else if (strcmp(fname->name, "read_int") == 0) {
            emit("read_int");
        } else if (strcmp(fname->name, "read_char") == 0) {
            emit("read_char");
        } else {
            /* 사용자 정의 함수: caller 템플릿 */
            /* (1) args는 이미 args 규칙에서 push됨 */

            /* 인자 개수 계산 */
            int nargs = 0;
            struct exprinfo *arg_p = (yyvsp[-1].exprlist);
            while (arg_p != NULL) { nargs++; arg_p = arg_p->next; }

            /* (2) return slot 확보 */
            emit("push_const 0");

            /* (3) return address - 고유 라벨 생성 */
            char *ret_label = new_label("Lret_");
            emit("push_const %s", ret_label);

            /* (4) old_fp 저장 */
            emit("push_reg fp");

            /* (5)-(6) fp = sp */
            emit("push_reg sp");
            emit("pop_reg fp");

            /* (7) jump to callee */
            emit("jump %s", fname->name);

            /* (8) 복귀 지점 */
            emit_label(ret_label);

            /* ★ args 정리 정책 ★
             * 스택이 위로 자람 (PUSH: sp++)
             * 복귀 직후 스택: arg0 ... arg_{n-1} return_value <-- sp
             * arg0 위치 = sp - nargs (더 낮은 주소)
             * 비void: return_value를 arg0 위치로 이동 후 shift
             * void: return_slot + args 모두 정리 */
            if (callee->returntype != voidtype) {
                /* 비void 함수: return_value 보존하면서 args 정리 */
                if (nargs > 0) {
                    /* 1. arg0 addr 계산: sp - nargs */
                    emit("push_reg sp");
                    emit("push_const %d", nargs);
                    emit("sub");  /* addr of arg0 = sp - nargs */

                    /* 2. return_value fetch: addr는 현재 sp - 1 */
                    emit("push_reg sp");
                    emit("push_const 1");
                    emit("sub");  /* addr of return_value */
                    emit("fetch");  /* get return_value */

                    /* 3. assign: [addr, value] 순서 */
                    emit("assign");  /* mem[arg0] = return_value */

                    /* 4. sp를 arg0 위치로 이동 (스택 되감기) */
                    emit("shift_sp %d", -nargs);
                }
                /* nargs == 0: return_value가 이미 유일한 값 */
            } else {
                /* void 함수: return_slot + args 모두 정리 (sp 감소) */
                emit("shift_sp %d", -(nargs + 1));
            }
        }

        (yyval.expr).type = callee->returntype;
        (yyval.expr).is_lvalue = 0;
        (yyval.expr).is_var = 0;
        (yyval.expr).is_null_const = 0;
        (yyval.expr).next = NULL;
    }
    REDUCE("unary->ID '(' args_opt ')'");
  }
#line 3233 "subc.tab.c"
    break;

  case 87: /* unary: SYM_NULL  */
#line 1757 "subc.y"
  {
    /* [Project 4] NULL = 0 */
    emit("push_const 0");

    (yyval.expr).type = inttype;  /* NULL은 일단 int로 취급 */
    (yyval.expr).is_lvalue = 0;
    (yyval.expr).is_var = 0;
    (yyval.expr).is_null_const = 1;  /* semantic에서 포인터 대입 허용 */
    (yyval.expr).next = NULL;
    REDUCE("unary->SYM_NULL");
  }
#line 3249 "subc.tab.c"
    break;

  case 88: /* args: expr  */
#line 1772 "subc.y"
  {
      /* 첫 번째 인자: 새 노드를 만들어서 head로 사용 */
      struct exprinfo *node = (struct exprinfo *)malloc(sizeof(struct exprinfo));
      *node = (yyvsp[0].expr);
      node->next = NULL;
      
      (yyval.exprlist) = node;
      REDUCE("args->expr");
  }
#line 3263 "subc.tab.c"
    break;

  case 89: /* args: args ',' expr  */
#line 1782 "subc.y"
  {
      /* 추가 인자: 새 노드를 만들어 끝에 붙이기 (정순 리스트) */
      struct exprinfo *node = (struct exprinfo *)malloc(sizeof(struct exprinfo));
      *node = (yyvsp[0].expr);
      node->next = NULL;
      
      /* 리스트 끝까지 가서 추가 */
      struct exprinfo *p = (yyvsp[-2].exprlist);
      while (p->next != NULL) {
          p = p->next;
      }
      p->next = node;
      
      (yyval.exprlist) = (yyvsp[-2].exprlist);  /* head는 그대로 유지 */

      REDUCE("args->args ',' expr");
  }
#line 3285 "subc.tab.c"
    break;

  case 90: /* args_opt: %empty  */
#line 1803 "subc.y"
  {
      (yyval.exprlist) = NULL;
      REDUCE("args_opt->epsilon");
  }
#line 3294 "subc.tab.c"
    break;

  case 91: /* args_opt: args  */
#line 1808 "subc.y"
  {
      (yyval.exprlist) = (yyvsp[0].exprlist);
      REDUCE("args_opt->args");
  }
#line 3303 "subc.tab.c"
    break;


#line 3307 "subc.tab.c"

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

#line 1814 "subc.y"


/* Epilogue section */
/* add return 0 */
int yyerror (char* s) {
  // fprintf (stderr, "yyerror: %s at line %d, token '%s'\n", s, get_lineno(), yyget_text());
  return 0;
}

/* sometime, it prints only empty lines */
void reduce(char* s) {
  printf("%s\n", s);
}

/* ========================================================================== */
/* [Project 3] Error message printing procedures                              */
/* ========================================================================== */

// Print the preamble of error message.
void error_preamble(void) {
  printf("%s:%d: error: ", filename, get_lineno());
}

void error_undeclared(void) {
  error_preamble();
  printf("use of undeclared identifier\n");
}

void error_redeclaration(void) {
  error_preamble();
  printf("redeclaration\n");
}

void error_assignable(void) {
  error_preamble();
  printf("lvalue is not assignable\n");
}

void error_incompatible(void) {
  error_preamble();
  printf("incompatible types for assignment operation\n");
}

void error_null(void) {
  error_preamble();
  printf("cannot assign 'NULL' to non-pointer type\n");
}

void error_binary(void) {
  error_preamble();
  printf("invalid operands to binary expression\n");
}

void error_unary(void) {
  error_preamble();
  printf("invalid argument type to unary expression\n");
}

void error_comparable(void) {
  error_preamble();
  printf("types are not comparable in binary expression\n");
}

void error_indirection(void) {
  error_preamble();
  printf("indirection requires pointer operand\n");
}

void error_addressof(void) {
  error_preamble();
  printf("cannot take the address of an rvalue\n");
}

void error_struct(void) {
  error_preamble();
  printf("member reference base type is not a struct\n");
}

void error_strurctp(void){
  error_preamble();
  printf("member reference base type is not a struct pointer\n");
}

void error_member(void) {
  error_preamble();
  printf("no such member in struct\n");
}

void error_array(void) {
  error_preamble();
  printf("subscripted value is not an array\n");
}

void error_subscript(void) {
  error_preamble();
  printf("array subscript is not an integer\n");
}

void error_incomplete(void) {
  error_preamble();
  printf("incomplete type\n");
}

void error_return(void) {
  error_preamble();
  printf("incompatible return types\n");
}

/* error_function and error_arguments are defined in symtab.c */
