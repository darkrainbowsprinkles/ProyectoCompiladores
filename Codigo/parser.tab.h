#ifndef YY_YY_PARSER_TAB_H_INCLUDED
#define YY_YY_PARSER_TAB_H_INCLUDED

#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

#ifndef YYSTYPE
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1

typedef struct ExprAttr ExprAttr;
typedef struct CodeAttr CodeAttr;
typedef struct ForAttr ForAttr;
typedef struct IfAttr IfAttr;

typedef union YYSTYPE
{
    char* string_val;
    ExprAttr* expr;
    CodeAttr* code;
    ForAttr* forpart;
    IfAttr* ifpart;
} YYSTYPE;
#endif

extern YYSTYPE yylval;

#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype
{
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
    READ = 298
};
#endif

#define IDENTIFIER 258
#define INT_NUMBER 259
#define FLOAT_NUMBER 260
#define CHAR_LITERAL 261
#define COMPARISON 262
#define ASSIGNATION 263
#define PLUS 264
#define MINUS 265
#define MULTIPLY 266
#define DIVIDE 267
#define INCREMENT 268
#define DECREMENT 269
#define AND 270
#define OR 271
#define NOT 272
#define TRUE_LITERAL 273
#define FALSE_LITERAL 274
#define INT 275
#define FLOAT 276
#define CHAR 277
#define FOR 278
#define IF 279
#define ELSE 280
#define WHILE 281
#define DO 282
#define SWITCH 283
#define BREAK 284
#define CASE 285
#define DEFAULT 286
#define STRUCT 287
#define OPEN_PARENTHESIS 288
#define CLOSE_PARENTHESIS 289
#define OPEN_BRACE 290
#define CLOSE_BRACE 291
#define SEMICOLON 292
#define COLON 293
#define BOOL 294
#define VOID 295
#define RETURN 296
#define PRINT 297
#define READ 298

#endif /* YY_YY_PARSER_TAB_H_INCLUDED */
