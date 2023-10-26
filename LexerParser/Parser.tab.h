/* A Bison parser, made by GNU Bison 3.8.  */

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "./Parser.y"


#line 52 "Parser.tab.h"

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
    STR_INT = 259,                 /* STR_INT  */
    STR_FLOAT = 260,               /* STR_FLOAT  */
    STR_VARCHAR = 261,             /* STR_VARCHAR  */
    SAFE = 262,                    /* SAFE  */
    QUIT = 263,                    /* QUIT  */
    CREATE = 264,                  /* CREATE  */
    DROP = 265,                    /* DROP  */
    SHOW = 266,                    /* SHOW  */
    USE = 267,                     /* USE  */
    DESC = 268,                    /* DESC  */
    INSERT = 269,                  /* INSERT  */
    DELETE = 270,                  /* DELETE  */
    UPDATE = 271,                  /* UPDATE  */
    SELECT = 272,                  /* SELECT  */
    ALTER = 273,                   /* ALTER  */
    ADD = 274,                     /* ADD  */
    INTO = 275,                    /* INTO  */
    VALUES = 276,                  /* VALUES  */
    FROM = 277,                    /* FROM  */
    WHERE = 278,                   /* WHERE  */
    SET = 279,                     /* SET  */
    NOT = 280,                     /* NOT  */
    DEFAULT = 281,                 /* DEFAULT  */
    AND = 282,                     /* AND  */
    DATABASES = 283,               /* DATABASES  */
    DATABASE = 284,                /* DATABASE  */
    TABLES = 285,                  /* TABLES  */
    TABLE = 286,                   /* TABLE  */
    INDEX = 287,                   /* INDEX  */
    PRIMARY = 288,                 /* PRIMARY  */
    FOREIGN = 289,                 /* FOREIGN  */
    KEY = 290,                     /* KEY  */
    CONSTRAINT = 291,              /* CONSTRAINT  */
    REFERENCES = 292,              /* REFERENCES  */
    T_INT = 293,                   /* T_INT  */
    T_FLOAT = 294,                 /* T_FLOAT  */
    T_VARCHAR = 295,               /* T_VARCHAR  */
    STR_NULL = 296                 /* STR_NULL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "./Parser.y"

    char *str;
    Column *column;
    PK *pk;
    FK *fk;
    std::vector<FK> *fkList;
    ColumnType type;
    bool boolean;
    std::vector<Column> *columnList;
    std::vector<Expr> *exprList;
    std::vector<std::vector<Expr>> *valLists;
    Expr *expr;
    CmpOp op;

#line 125 "Parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
