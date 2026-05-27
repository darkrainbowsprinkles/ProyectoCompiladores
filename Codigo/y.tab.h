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
     IDENTIFIER = 258,
     INT = 259,
     FLOAT = 260,
     CHAR = 261,
     FOR = 262,
     IF = 263,
     ELSE = 264,
     WHILE = 265,
     DO = 266,
     SWITCH = 267,
     BREAK = 268,
     CASE = 269,
     DEFAULT = 270,
     STRUCT = 271,
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
     COLON = 288,
     BOOL = 289,
     TRUE_LITERAL = 290,
     FALSE_LITERAL = 291,
     AND = 292,
     OR = 293,
     NOT = 294,
     VOID = 295,
     RETURN = 296,
     PRINT = 297,
     READ = 298
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define INT 259
#define FLOAT 260
#define CHAR 261
#define FOR 262
#define IF 263
#define ELSE 264
#define WHILE 265
#define DO 266
#define SWITCH 267
#define BREAK 268
#define CASE 269
#define DEFAULT 270
#define STRUCT 271
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
#define BOOL 289
#define TRUE_LITERAL 290
#define FALSE_LITERAL 291
#define AND 292
#define OR 293
#define NOT 294
#define VOID 295
#define RETURN 296
#define PRINT 297
#define READ 298



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 51 "parser.y"

    char* string_val; 


/* Line 2058 of yacc.c  */
#line 148 "y.tab.h"
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

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
