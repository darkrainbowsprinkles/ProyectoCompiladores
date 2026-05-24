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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
     INT = 258,
     FLOAT = 259,
     CHAR = 260,
     FOR = 261,
     IF = 262,
     ELSE = 263,
     WHILE = 264,
     DO = 265,
     SWITCH = 266,
     BREAK = 267,
     CASE = 268,
     DEFAULT = 269,
     STRUCT = 270,
     IDENTIFIER = 271,
     INT_NUMBER = 272,
     FLOAT_NUMBER = 273,
     CHAR_LITERAL = 274,
     COMPARISON = 275,
     ASSIGNATION = 276,
     PLUS = 277,
     MINUS = 278,
     MULTIPLY = 279,
     DIVIDE = 280,
     INCREMENT = 281,
     DECREMENT = 282,
     OPEN_PARENTHESIS = 283,
     CLOSE_PARENTHESIS = 284,
     OPEN_BRACE = 285,
     CLOSE_BRACE = 286,
     SEMICOLON = 287,
     COLON = 288
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define CHAR 260
#define FOR 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define DO 265
#define SWITCH 266
#define BREAK 267
#define CASE 268
#define DEFAULT 269
#define STRUCT 270
#define IDENTIFIER 271
#define INT_NUMBER 272
#define FLOAT_NUMBER 273
#define CHAR_LITERAL 274
#define COMPARISON 275
#define ASSIGNATION 276
#define PLUS 277
#define MINUS 278
#define MULTIPLY 279
#define DIVIDE 280
#define INCREMENT 281
#define DECREMENT 282
#define OPEN_PARENTHESIS 283
#define CLOSE_PARENTHESIS 284
#define OPEN_BRACE 285
#define CLOSE_BRACE 286
#define SEMICOLON 287
#define COLON 288



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
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

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
