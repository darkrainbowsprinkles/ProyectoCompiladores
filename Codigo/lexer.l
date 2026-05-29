%{
#include <stdio.h>
#include <string.h>

typedef struct ExprAttr ExprAttr;
typedef struct CodeAttr CodeAttr;
typedef struct ForAttr ForAttr;
typedef struct IfAttr IfAttr;

#include <stdio.h>
#include "y.tab.h"

#define MAX_TOKENS 1000

typedef struct
{
    char *token;
    char *type;
} TokenData;

TokenData **allTokens;
int tokenCount = 0;

void registerToken(char *value, char *type);
void writeTokenTypes();
void writeTokenTable();
void writeTypeInTable(FILE *file, char *type);
%}


digit [0-9]
letter [a-zA-Z]
character [^']
comparison <|>|<=|>=|==
emptySpace [ \t\n\r]

%%
"int"                               { registerToken(yytext, "Palabras Reservadas"); return INT; }
"float"                             { registerToken(yytext, "Palabras Reservadas"); return FLOAT; }
"char"                              { registerToken(yytext, "Palabras Reservadas"); return CHAR; }
"bool"                              { registerToken(yytext, "Palabras Reservadas"); return BOOL; }
"true"                              { registerToken(yytext, "Palabras Reservadas"); yylval.string_val = strdup(yytext); return TRUE_LITERAL; }
"false"                             { registerToken(yytext, "Palabras Reservadas"); yylval.string_val = strdup(yytext); return FALSE_LITERAL; }
"for"                               { registerToken(yytext, "Palabras Reservadas"); return FOR; }
"if"                                { registerToken(yytext, "Palabras Reservadas"); return IF; }
"else"                              { registerToken(yytext, "Palabras Reservadas"); return ELSE; }
"while"                             { registerToken(yytext, "Palabras Reservadas"); return WHILE; }
"do"                                { registerToken(yytext, "Palabras Reservadas"); return DO; }
"switch"                            { registerToken(yytext, "Palabras Reservadas"); return SWITCH; }
"struct"                            { registerToken(yytext, "Palabras Reservadas"); return STRUCT; }
"case"                              { registerToken(yytext, "Palabras Reservadas"); return CASE; }
"break"                             { registerToken(yytext, "Palabras Reservadas"); return BREAK; }
"default"                           { registerToken(yytext, "Palabras Reservadas"); return DEFAULT; }
"void"                              { registerToken(yytext, "Palabras Reservadas"); return VOID; }
"return"                            { registerToken(yytext, "Palabras Reservadas"); return RETURN; }
"print"                             { registerToken(yytext, "Palabras Reservadas"); return PRINT; }
"read"                              { registerToken(yytext, "Palabras Reservadas"); return READ; }

{letter}+(_|{letter}|{digit})* { 
    registerToken(yytext, "Variables"); 
    yylval.string_val = strdup(yytext); 
    return IDENTIFIER; 
}

{digit}+                            { registerToken(yytext, "Numeros"); yylval.string_val = strdup(yytext); return INT_NUMBER; }
{digit}+\.{digit}+                  { registerToken(yytext, "Numeros"); yylval.string_val = strdup(yytext); return FLOAT_NUMBER; }
{comparison}                        { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return COMPARISON; }    
{emptySpace}+                       { /* Ignorar espacios */ }
'{character}?'                      { registerToken(yytext, "Cadenas"); yylval.string_val = strdup(yytext); return CHAR_LITERAL; }
"="                                 { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return ASSIGNATION; }
"+"                                 { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return PLUS; }
"-"                                 { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return MINUS; }
"*"                                 { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return MULTIPLY; }
"//".*                              { /* Ignorar comentarios de una linea */ }
"/"                                 { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return DIVIDE; }
"++"                                { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return INCREMENT; }
"--"                                { registerToken(yytext, "Operadores"); yylval.string_val = strdup(yytext); return DECREMENT; }
"&&"                                { registerToken(yytext, "Operadores Logicos"); yylval.string_val = strdup(yytext); return AND; }
"||"                                { registerToken(yytext, "Operadores Logicos"); yylval.string_val = strdup(yytext); return OR; }
"!"                                 { registerToken(yytext, "Operadores Logicos"); yylval.string_val = strdup(yytext); return NOT; }
"("                                 { registerToken(yytext, "Simbolos Especiales"); return OPEN_PARENTHESIS; }
")"                                 { registerToken(yytext, "Simbolos Especiales"); return CLOSE_PARENTHESIS; }
"{"                                 { registerToken(yytext, "Simbolos Especiales"); return OPEN_BRACE; }
"}"                                 { registerToken(yytext, "Simbolos Especiales"); return CLOSE_BRACE; }
";"                                 { registerToken(yytext, "Simbolos Especiales"); return SEMICOLON; }
":"                                 { registerToken(yytext, "Simbolos Especiales"); return COLON; }
.                                   { registerToken(yytext, "Errores"); printf("Error lexico\n"); }
%%

void registerToken(char *value, char *type)
{
    if(tokenCount >= MAX_TOKENS)
    {
        printf("Numero maximo de Tokens alcanzado\n");
        exit(1);
    }

    TokenData *newToken = malloc(sizeof(TokenData));
    newToken -> token = strdup(value);
    newToken -> type = strdup(type);
    allTokens[tokenCount] = newToken;
    tokenCount++;
}

void writeTokenTypes()
{
    FILE *file = fopen("output/tokens.txt", "w");

    if(file == NULL)
    {
        printf("Error al abrir el archivo de escritura para tokens\n");
        exit(1);
    }

    fprintf(file, "\t\t\tToken\t\t\tClase\n\n");

    for(int i = 0; i < tokenCount; i++)
    {
        char *currentToken = allTokens[i] -> token;
        char *currentType = allTokens[i] -> type;

        fprintf(file, "%d\t\t\t%s\t\t\t%s\n", i, currentToken, currentType);
    }

    fclose(file);
}

void writeTokenTable()
{
    FILE *file = fopen("output/table.txt", "w");

    if(file == NULL)
    {
        printf("Error al abrir el archivo de escritura para tokens\n");
        exit(1);
    }

    writeTypeInTable(file, "Variables");
    writeTypeInTable(file, "Cadenas");
    writeTypeInTable(file, "Errores");

    fclose(file);
}

void writeTypeInTable(FILE *file, char *type)
{
    fprintf(file, "Tabla %s:\n", type);

    int count = 0;

    for(int i = 0; i < tokenCount; i++)
    {
        char *currentType = allTokens[i] -> type;
        int success = strcmp(currentType, type);

        if(success == 0)
        {
            char *currentToken = allTokens[i] -> token;
            fprintf(file, "%d\t\t\t%s\n", count, currentToken);
            count++;
        }
    }

    fprintf(file, "\n");
}

void clearMemoryAllocation()
{
    for(int i = 0; i < tokenCount; i++)
    {
        free(allTokens[i]);
    }

    free(allTokens);
}

void readInputFile(int argc, char *argv[])
{
    if(argc > 1)
    {
        yyin = fopen(argv[1], "rt");
    }
    else
    {
        printf("Error al abrir el archivo de input\n");
        exit(1);
    }
}

int yywrap() 
{ 
    return 1; 
}

/* int main(int argc, char *argv[])
{
    readInputFile(argc, argv);

    allTokens = (TokenData **)malloc(MAX_TOKENS * sizeof(TokenData *));

    if(yyparse() == 0)
    {
        printf("Analisis sintactico correcto\n");
    }

    writeTokenTypes();
    writeTokenTable();
    clearMemoryAllocation();

    fclose(yyin);

    return 0;
} */