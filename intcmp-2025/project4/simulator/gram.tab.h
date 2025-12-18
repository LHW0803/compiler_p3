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

#ifndef YY_YY_GRAM_TAB_H_INCLUDED
# define YY_YY_GRAM_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT_CONST = 258,               /* INT_CONST  */
    STRING_CONST = 259,            /* STRING_CONST  */
    NEGATE = 260,                  /* NEGATE  */
    NOT = 261,                     /* NOT  */
    ABS = 262,                     /* ABS  */
    ADD = 263,                     /* ADD  */
    SUB = 264,                     /* SUB  */
    MUL = 265,                     /* MUL  */
    DIV = 266,                     /* DIV  */
    MOD = 267,                     /* MOD  */
    AND = 268,                     /* AND  */
    OR = 269,                      /* OR  */
    EQUAL = 270,                   /* EQUAL  */
    NOT_EQUAL = 271,               /* NOT_EQUAL  */
    GREATER = 272,                 /* GREATER  */
    GREATER_EQUAL = 273,           /* GREATER_EQUAL  */
    LESS = 274,                    /* LESS  */
    LESS_EQUAL = 275,              /* LESS_EQUAL  */
    JUMP = 276,                    /* JUMP  */
    BRANCH_TRUE = 277,             /* BRANCH_TRUE  */
    BRANCH_FALSE = 278,            /* BRANCH_FALSE  */
    EXIT = 279,                    /* EXIT  */
    PUSH_CONST = 280,              /* PUSH_CONST  */
    PUSH_REG = 281,                /* PUSH_REG  */
    POP_REG = 282,                 /* POP_REG  */
    SHIFT_SP = 283,                /* SHIFT_SP  */
    ASSIGN = 284,                  /* ASSIGN  */
    FETCH = 285,                   /* FETCH  */
    READ_INT = 286,                /* READ_INT  */
    READ_CHAR = 287,               /* READ_CHAR  */
    WRITE_INT = 288,               /* WRITE_INT  */
    WRITE_CHAR = 289,              /* WRITE_CHAR  */
    WRITE_STRING = 290,            /* WRITE_STRING  */
    SP = 291,                      /* SP  */
    FP = 292,                      /* FP  */
    PC = 293,                      /* PC  */
    ID = 294,                      /* ID  */
    DATA = 295,                    /* DATA  */
    STRING = 296,                  /* STRING  */
    NEW_LINE = 297                 /* NEW_LINE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 72 "gram.y"

	int intVal;
	char* stringVal;
	id_ptr id;

	struct instr_node* instr;
	struct operand* operand;

#line 115 "gram.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_GRAM_TAB_H_INCLUDED  */
