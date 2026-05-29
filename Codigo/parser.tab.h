/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     INT_NUMBER = 259,
     FLOAT_NUMBER = 260,
     CHAR_LITERAL = 261,
     COMPARISON = 262,
     ASSIGNATION = 263,
     PLUS = 264,
     MINUS = 265,
     MULTIPLY = 266,
     DIVIDE = 267,
     INCREMENT = 268,
     DECREMENT = 269,
     AND = 270,
     OR = 271,
     NOT = 272,
     TRUE_LITERAL = 273,
     FALSE_LITERAL = 274,
     INT = 275,
     FLOAT = 276,
     CHAR = 277,
     FOR = 278,
     IF = 279,
     ELSE = 280,
     WHILE = 281,
     DO = 282,
     SWITCH = 283,
     BREAK = 284,
     CASE = 285,
     DEFAULT = 286,
     STRUCT = 287,
     OPEN_PARENTHESIS = 288,
     CLOSE_PARENTHESIS = 289,
     OPEN_BRACE = 290,
     CLOSE_BRACE = 291,
     SEMICOLON = 292,
     COLON = 293,
     BOOL = 294,
     VOID = 295,
     RETURN = 296,
     PRINT = 297,
     READ = 298,
     LOWER_THAN_ELSE = 299
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 1158 "parser.y"

    char* string_val;
    ExprAttr* expr;
    CodeAttr* code;
    ForAttr* forpart;
    IfAttr* ifpart;


/* Line 2058 of yacc.c  */
#line 110 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
