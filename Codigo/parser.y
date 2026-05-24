%{
#include <stdio.h>
void yyerror(char *msg);
int yylex();
%}

%token INT
%token FLOAT
%token CHAR
%token FOR
%token IF
%token ELSE
%token WHILE
%token DO
%token SWITCH
%token BREAK
%token CASE
%token DEFAULT
%token STRUCT
%token IDENTIFIER
%token INT_NUMBER
%token FLOAT_NUMBER
%token CHAR_LITERAL
%token COMPARISON
%token ASSIGNATION
%token PLUS
%token MINUS
%token MULTIPLY 
%token DIVIDE
%token INCREMENT
%token DECREMENT
%token OPEN_PARENTHESIS
%token CLOSE_PARENTHESIS
%token OPEN_BRACE
%token CLOSE_BRACE
%token SEMICOLON
%token COLON
%start program

%%
program:

    | statement program;

statement:
    declaration SEMICOLON
    | for_statement
    | if_statement
    | while_statement
    | do_while_statement SEMICOLON;
    | switch_statement
    | struct_statement SEMICOLON;

declaration: 
    operation
    | type IDENTIFIER
    | type IDENTIFIER ASSIGNATION operation
    | IDENTIFIER ASSIGNATION operation;

for_statement: 
    FOR OPEN_PARENTHESIS for_content CLOSE_PARENTHESIS action_braces;

for_content:
    declaration SEMICOLON condition SEMICOLON operation;

if_statement:
    if_content
    | if_content ELSE if_statement
    | if_content ELSE action_braces;

if_content:
    IF OPEN_PARENTHESIS condition CLOSE_PARENTHESIS action_braces

while_statement:
    WHILE OPEN_PARENTHESIS condition CLOSE_PARENTHESIS action_braces;

do_while_statement:
    DO OPEN_BRACE program CLOSE_BRACE WHILE OPEN_PARENTHESIS condition CLOSE_PARENTHESIS;

switch_statement:
    SWITCH OPEN_PARENTHESIS operation CLOSE_PARENTHESIS OPEN_BRACE switch_content CLOSE_BRACE;

switch_content:
    
    | statement switch_content
    | CASE operation COLON program BREAK SEMICOLON switch_content
    | DEFAULT COLON program BREAK SEMICOLON;

struct_statement:
    STRUCT IDENTIFIER OPEN_BRACE struct_content CLOSE_BRACE;

struct_content:

    | type IDENTIFIER SEMICOLON struct_content;

action_braces:
    OPEN_BRACE program CLOSE_BRACE

condition: 
    operation
    | operation COMPARISON operation;

operation: 
    expression
    | expression PLUS operation
    | expression MINUS operation
    | expression MULTIPLY operation
    | expression DIVIDE operation;

type:
    INT
    | FLOAT
    | CHAR

expression: 
    IDENTIFIER 
    | INT_NUMBER 
    | FLOAT_NUMBER 
    | CHAR_LITERAL
    | IDENTIFIER INCREMENT
    | IDENTIFIER DECREMENT;
%%

void yyerror(char *msg)
{
    printf("Error sintactico\n");
}