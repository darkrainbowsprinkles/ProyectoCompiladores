%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void readInputFile(int argc, char *argv[]);
extern FILE *yyin;
extern void writeTokenTypes();
extern void writeTokenTable();
void yyerror(char *msg);
int yylex();
extern char *yytext;

typedef struct
{
    char *token;
    char *type;
} TokenData;

extern TokenData **allTokens;
#define MAX_TOKENS 1000

typedef struct {
    char name[50];
    char type[20];
    char category[20]; 
} Symbol;

Symbol symbolTable[200];
int symbolCount = 0;

void addSymbol(char* name, char* type, char* category) 
{
    for(int i = 0; i < symbolCount; i++) 
    {
        if(strcmp(symbolTable[i].name, name) == 0) 
        {
            printf("[Error Semantico] La variable/funcion '%s' ya fue declarada.\n", name);
            return;
        }
    }

    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    strcpy(symbolTable[symbolCount].category, category);
    symbolCount++;
    printf("[Tabla de Simbolos] Registrado: %s (%s) como %s\n", name, type, category);
}
%}

%union {
    char* string_val; 
}

%type <string_val> type
%token <string_val> IDENTIFIER

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
%token BOOL TRUE_LITERAL FALSE_LITERAL
%token AND OR NOT
%token VOID RETURN PRINT READ
%left OR
%left AND
%left COMPARISON
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right NOT

%%
program:

    | statement program;

statement:
    declaration SEMICOLON
    | function_declaration   
    | RETURN operation SEMICOLON 
    | RETURN SEMICOLON       
    | PRINT OPEN_PARENTHESIS operation CLOSE_PARENTHESIS SEMICOLON 
    | READ OPEN_PARENTHESIS IDENTIFIER CLOSE_PARENTHESIS SEMICOLON 
    | for_statement
    | if_statement
    | while_statement
    | do_while_statement SEMICOLON
    | switch_statement
    | struct_statement SEMICOLON
    | BREAK SEMICOLON;

declaration: 
    operation
    | type IDENTIFIER { 
        addSymbol($2, $1, "variable"); 
    }
    | type IDENTIFIER ASSIGNATION operation { 
        addSymbol($2, $1, "variable"); 
    }
    | IDENTIFIER ASSIGNATION operation;

function_declaration:
    type IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS action_braces
    | VOID IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS action_braces;

for_statement: 
    FOR OPEN_PARENTHESIS for_content CLOSE_PARENTHESIS action_braces;

for_content:
    declaration SEMICOLON operation SEMICOLON operation;

if_statement:
    if_content
    | if_content ELSE if_statement
    | if_content ELSE action_braces;

if_content:
    IF OPEN_PARENTHESIS operation CLOSE_PARENTHESIS action_braces;

while_statement:
    WHILE OPEN_PARENTHESIS operation CLOSE_PARENTHESIS action_braces;

do_while_statement:
    DO OPEN_BRACE program CLOSE_BRACE WHILE OPEN_PARENTHESIS operation CLOSE_PARENTHESIS;

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
    OPEN_BRACE program CLOSE_BRACE;

operation: 
    expression
    | operation PLUS operation
    | operation MINUS operation
    | operation MULTIPLY operation
    | operation DIVIDE operation
    | operation COMPARISON operation 
    | operation AND operation
    | operation OR operation
    | NOT operation
    | OPEN_PARENTHESIS operation CLOSE_PARENTHESIS;

type:
    INT     { $$ = strdup("int"); }
    | FLOAT { $$ = strdup("float"); }
    | CHAR  { $$ = strdup("char"); }
    | BOOL  { $$ = strdup("bool"); };

expression: 
    IDENTIFIER 
    | INT_NUMBER 
    | FLOAT_NUMBER 
    | CHAR_LITERAL
    | TRUE_LITERAL
    | FALSE_LITERAL
    | IDENTIFIER INCREMENT
    | IDENTIFIER DECREMENT
    | IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS; 
%%

extern char *yytext; 

void yyerror(char *msg)
{
    printf("\n[!] Error sintactico cerca del token: '%s'\n", yytext);
}

void writeSymbolTableToFile() 
{
    FILE *file = fopen("tabla_simbolos.txt", "w");

    if(file == NULL) 
    {
        printf("Error: No se pudo crear el archivo de tabla de simbolos.\n");
        return;
    }

    fprintf(file, "NOMBRE\t\tTIPO\t\tCATEGORIA\n");
    fprintf(file, "------------------------------------------\n");

    for(int i = 0; i < symbolCount; i++) 
    {
        fprintf(file, "%s\t\t%s\t\t%s\n", 
                symbolTable[i].name, 
                symbolTable[i].type, 
                symbolTable[i].category);
    }
    
    fclose(file);
    printf("\n[Info] Tabla de simbolos guardada en 'tabla_simbolos.txt'\n");
}

int main(int argc, char *argv[]) {

    readInputFile(argc, argv);

    allTokens = (TokenData **)malloc(MAX_TOKENS * sizeof(TokenData *));

    if(yyparse() == 0) {
        printf("Analisis sintactico correcto\n");

        writeSymbolTableToFile();
        writeTokenTypes();
        writeTokenTable();
    }

    fclose(yyin);

    return 0;
}