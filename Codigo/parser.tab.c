/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

extern void readInputFile(int argc, char *argv[]);
extern FILE *yyin;
extern void writeTokenTypes();
extern void writeTokenTable();
extern void clearMemoryAllocation();
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

typedef struct
{
    char *place;
    char *code;
} ExprAttr;

typedef struct
{
    char *code;
} CodeAttr;

typedef struct
{
    char *initCode;
    char *conditionCode;
    char *conditionPlace;
    char *updateCode;
} ForAttr;

typedef struct
{
    char *conditionCode;
    char *conditionPlace;
    char *thenCode;
} IfAttr;

typedef struct {
    char name[50];
    char type[20];
    char category[20]; 
} Symbol;

Symbol symbolTable[200];
int symbolCount = 0;
static int tempCounter = 0;
static int labelCounter = 0;
static char *generatedCode = NULL;
static char *optimizedCode = NULL;

typedef struct
{
    char name[64];
    char value[64];
    int valid;
} ConstantEntry;

static ConstantEntry constantTable[256];
static int constantCount = 0;

char *dupText(const char *text);
char *formatText(const char *format, ...);
char *joinTexts(int count, ...);
char *nextLineToken(char **cursor);
void clearConstants();
void clearAllConstants();
const char *lookupConstantValue(const char *name);
void setConstantValue(const char *name, const char *value);
int isConstantAtom(const char *text);
int evaluateUnaryNot(const char *text, char *buffer, size_t size);
int evaluateBinary(const char *leftText, const char *op, const char *rightText, char *buffer, size_t size);
char *optimizeIntermediateCodeText(const char *source);
char *generateFinalCodeText(const char *source);

void clearConstants()
{
    constantCount = 0;
    for(int i = 0; i < 256; i++)
    {
        constantTable[i].name[0] = '\0';
        constantTable[i].value[0] = '\0';
        constantTable[i].valid = 0;
    }
}

int isIdentifierName(const char *text)
{
    if(text == NULL || text[0] == '\0')
    {
        return 0;
    }

    if(!(isalpha((unsigned char)text[0]) || text[0] == '_'))
    {
        return 0;
    }

    for(int i = 1; text[i] != '\0'; i++)
    {
        if(!(isalnum((unsigned char)text[i]) || text[i] == '_'))
        {
            return 0;
        }
    }

    return 1;
}

int isIntegerText(const char *text)
{
    if(text == NULL || *text == '\0')
    {
        return 0;
    }

    int index = 0;
    if(text[index] == '-' || text[index] == '+')
    {
        index++;
    }

    if(text[index] == '\0')
    {
        return 0;
    }

    for(; text[index] != '\0'; index++)
    {
        if(!isdigit((unsigned char)text[index]))
        {
            return 0;
        }
    }

    return 1;
}

int isFloatText(const char *text)
{
    if(text == NULL || *text == '\0')
    {
        return 0;
    }

    char *end = NULL;
    strtod(text, &end);
    return end != NULL && *end == '\0' && strchr(text, '.') != NULL;
}

int isBooleanText(const char *text)
{
    return text != NULL && (strcmp(text, "true") == 0 || strcmp(text, "false") == 0 || strcmp(text, "0") == 0 || strcmp(text, "1") == 0);
}

double numericValue(const char *text)
{
    if(text == NULL)
    {
        return 0.0;
    }

    if(strcmp(text, "true") == 0)
    {
        return 1.0;
    }

    if(strcmp(text, "false") == 0)
    {
        return 0.0;
    }

    return strtod(text, NULL);
}

void formatNumberValue(double value, char *buffer, size_t size)
{
    long long asInteger = (long long)value;
    if(value == (double)asInteger)
    {
        snprintf(buffer, size, "%lld", asInteger);
    }
    else
    {
        snprintf(buffer, size, "%g", value);
    }
}

int isConstantAtom(const char *text)
{
    return isIntegerText(text) || isFloatText(text) || isBooleanText(text);
}

const char *lookupConstantValue(const char *name)
{
    if(name == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < constantCount; i++)
    {
        if(constantTable[i].valid && strcmp(constantTable[i].name, name) == 0)
        {
            return constantTable[i].value;
        }
    }

    return NULL;
}

void setConstantValue(const char *name, const char *value)
{
    if(name == NULL)
    {
        return;
    }

    for(int i = 0; i < constantCount; i++)
    {
        if(strcmp(constantTable[i].name, name) == 0)
        {
            if(value == NULL)
            {
                constantTable[i].valid = 0;
                constantTable[i].name[0] = '\0';
                constantTable[i].value[0] = '\0';
            }
            else
            {
                constantTable[i].valid = 1;
                snprintf(constantTable[i].value, sizeof(constantTable[i].value), "%s", value);
            }
            return;
        }
    }

    if(value == NULL)
    {
        return;
    }

    if(constantCount >= 256)
    {
        return;
    }

    snprintf(constantTable[constantCount].name, sizeof(constantTable[constantCount].name), "%s", name);
    snprintf(constantTable[constantCount].value, sizeof(constantTable[constantCount].value), "%s", value);
    constantTable[constantCount].valid = 1;
    constantCount++;
}

void clearAllConstants()
{
    for(int i = 0; i < constantCount; i++)
    {
        constantTable[i].valid = 0;
    }
}

int isReservedWordLine(const char *line)
{
    return strncmp(line, "function ", 9) == 0 ||
           strncmp(line, "end function ", 14) == 0 ||
           strncmp(line, "struct ", 8) == 0 ||
           strncmp(line, "switch ", 7) == 0 ||
           strncmp(line, "case ", 5) == 0 ||
           strncmp(line, "default:", 8) == 0 ||
           strncmp(line, "field ", 6) == 0 ||
           strncmp(line, "decl ", 5) == 0;
}

char *nextLineToken(char **cursor)
{
    if(cursor == NULL || *cursor == NULL || **cursor == '\0')
    {
        return NULL;
    }

    char *start = *cursor;
    char *newline = strchr(start, '\n');

    if(newline != NULL)
    {
        *newline = '\0';
        *cursor = newline + 1;
    }
    else
    {
        *cursor = start + strlen(start);
    }

    return start;
}

char *replaceIdentifierWithConstant(const char *text)
{
    const char *constant = lookupConstantValue(text);
    if(constant != NULL)
    {
        return dupText(constant);
    }

    return dupText(text);
}

int evaluateBinary(const char *leftText, const char *op, const char *rightText, char *buffer, size_t size)
{
    double left = numericValue(leftText);
    double right = numericValue(rightText);
    double result = 0.0;
    int booleanResult = 0;

    if(strcmp(op, "+") == 0)
    {
        result = left + right;
        formatNumberValue(result, buffer, size);
        return 1;
    }

    if(strcmp(op, "-") == 0)
    {
        result = left - right;
        formatNumberValue(result, buffer, size);
        return 1;
    }

    if(strcmp(op, "*") == 0)
    {
        result = left * right;
        formatNumberValue(result, buffer, size);
        return 1;
    }

    if(strcmp(op, "/") == 0)
    {
        if(right == 0.0)
        {
            return 0;
        }

        result = left / right;
        formatNumberValue(result, buffer, size);
        return 1;
    }

    if(strcmp(op, "<") == 0)
    {
        booleanResult = left < right;
    }
    else if(strcmp(op, ">") == 0)
    {
        booleanResult = left > right;
    }
    else if(strcmp(op, "<=") == 0)
    {
        booleanResult = left <= right;
    }
    else if(strcmp(op, ">=") == 0)
    {
        booleanResult = left >= right;
    }
    else if(strcmp(op, "==") == 0)
    {
        booleanResult = left == right;
    }
    else if(strcmp(op, "&&") == 0)
    {
        booleanResult = (left != 0.0) && (right != 0.0);
    }
    else if(strcmp(op, "||") == 0)
    {
        booleanResult = (left != 0.0) || (right != 0.0);
    }
    else
    {
        return 0;
    }

    snprintf(buffer, size, "%d", booleanResult ? 1 : 0);
    return 1;
}

int evaluateUnaryNot(const char *text, char *buffer, size_t size)
{
    if(!isConstantAtom(text))
    {
        return 0;
    }

    double value = numericValue(text);
    snprintf(buffer, size, "%d", value == 0.0 ? 1 : 0);
    return 1;
}

char *optimizeIntermediateCodeText(const char *source)
{
    if(source == NULL || source[0] == '\0')
    {
        return dupText("");
    }

    clearConstants();

    char *working = dupText(source);
    size_t capacity = strlen(source) * 2 + 256;
    char *result = (char *)malloc(capacity);
    if(result == NULL)
    {
        printf("Error de memoria al optimizar codigo intermedio\n");
        exit(1);
    }

    result[0] = '\0';

    int skippingDeadCode = 0;
    char *cursor = working;
    char *line = nextLineToken(&cursor);

    while(line != NULL)
    {
        char *trimmed = line;
        while(*trimmed != '\0' && isspace((unsigned char)*trimmed))
        {
            trimmed++;
        }

        size_t trimmedLength = strlen(trimmed);
        while(trimmedLength > 0 && isspace((unsigned char)trimmed[trimmedLength - 1]))
        {
            trimmed[trimmedLength - 1] = '\0';
            trimmedLength--;
        }

        if(trimmed[0] == '\0')
        {
            line = nextLineToken(&cursor);
            continue;
        }

        if(skippingDeadCode)
        {
            if(strchr(trimmed, ':') != NULL)
            {
                skippingDeadCode = 0;
            }
            else
            {
                line = nextLineToken(&cursor);
                continue;
            }
        }

        if(strncmp(trimmed, "return", 6) == 0)
        {
            if(strlen(result) + strlen(trimmed) + 2 >= capacity)
            {
                capacity *= 2;
                result = (char *)realloc(result, capacity);
            }

            strcat(result, trimmed);
            strcat(result, "\n");
            skippingDeadCode = 1;
            line = nextLineToken(&cursor);
            continue;
        }

        if(strcmp(trimmed, "break") == 0)
        {
            if(strlen(result) + strlen(trimmed) + 2 >= capacity)
            {
                capacity *= 2;
                result = (char *)realloc(result, capacity);
            }

            strcat(result, trimmed);
            strcat(result, "\n");
            skippingDeadCode = 1;
            line = nextLineToken(&cursor);
            continue;
        }

        if(strncmp(trimmed, "read ", 5) == 0)
        {
            char variable[64];
            if(sscanf(trimmed, "read %63s", variable) == 1)
            {
                setConstantValue(variable, NULL);
            }
        }

        if(strncmp(trimmed, "goto ", 5) == 0 || strncmp(trimmed, "ifFalse ", 8) == 0 || strncmp(trimmed, "if ", 3) == 0)
        {
            char keyword[16];
            char operand[64];
            char label[64];
            if(sscanf(trimmed, "%15s %63s goto %63s", keyword, operand, label) == 3)
            {
                const char *constant = lookupConstantValue(operand);
                const char *resolvedOperand = constant != NULL ? constant : operand;
                if(isConstantAtom(resolvedOperand))
                {
                    if(strcmp(keyword, "ifFalse") == 0)
                    {
                        if(strcmp(resolvedOperand, "0") == 0 || strcmp(resolvedOperand, "false") == 0)
                        {
                            snprintf(trimmed, 128, "goto %s", label);
                        }
                        else
                        {
                            trimmed[0] = '\0';
                        }
                    }
                    else if(strcmp(keyword, "if") == 0)
                    {
                        if(strcmp(resolvedOperand, "0") == 0 || strcmp(resolvedOperand, "false") == 0)
                        {
                            trimmed[0] = '\0';
                        }
                        else
                        {
                            snprintf(trimmed, 128, "goto %s", label);
                        }
                    }
                }
            }
        }

        if(trimmed[0] != '\0' && !isReservedWordLine(trimmed) && strchr(trimmed, ':') == NULL)
        {
            char lhs[64];
            char rhs1[64];
            char op[8];
            char rhs2[64];
            char callName[64];

            if(sscanf(trimmed, "%63s = call %63s", lhs, callName) == 2)
            {
                setConstantValue(lhs, NULL);
            }
            else if(sscanf(trimmed, "%63s = ! %63s", lhs, rhs1) == 2)
            {
                const char *constant = lookupConstantValue(rhs1);
                char folded[64];
                if(constant != NULL && evaluateUnaryNot(constant, folded, sizeof(folded)))
                {
                    snprintf(trimmed, 128, "%s = %s", lhs, folded);
                    setConstantValue(lhs, folded);
                }
                else
                {
                    snprintf(trimmed, 128, "%s = ! %s", lhs, constant != NULL ? constant : rhs1);
                    setConstantValue(lhs, NULL);
                }
            }
            else if(sscanf(trimmed, "%63s = %63s %7s %63s", lhs, rhs1, op, rhs2) == 4)
            {
                const char *leftConstant = lookupConstantValue(rhs1);
                const char *rightConstant = lookupConstantValue(rhs2);
                char folded[64];

                if(leftConstant != NULL || isConstantAtom(rhs1))
                {
                    rhs1[0] = '\0';
                }

                if(rightConstant != NULL || isConstantAtom(rhs2))
                {
                    rhs2[0] = '\0';
                }

                const char *leftValue = leftConstant != NULL ? leftConstant : rhs1;
                const char *rightValue = rightConstant != NULL ? rightConstant : rhs2;

                if(evaluateBinary(leftValue, op, rightValue, folded, sizeof(folded)))
                {
                    snprintf(trimmed, 128, "%s = %s", lhs, folded);
                    setConstantValue(lhs, folded);
                }
                else
                {
                    snprintf(trimmed, 128, "%s = %s %s %s", lhs, leftValue, op, rightValue);
                    setConstantValue(lhs, NULL);
                }
            }
            else if(sscanf(trimmed, "%63s = %63s", lhs, rhs1) == 2)
            {
                const char *constant = lookupConstantValue(rhs1);
                if(constant != NULL)
                {
                    snprintf(trimmed, 128, "%s = %s", lhs, constant);
                    setConstantValue(lhs, constant);
                }
                else if(isConstantAtom(rhs1))
                {
                    setConstantValue(lhs, rhs1);
                }
                else if(strcmp(lhs, rhs1) == 0)
                {
                    trimmed[0] = '\0';
                }
                else
                {
                    setConstantValue(lhs, NULL);
                }
            }
            else if(strncmp(trimmed, "function ", 9) == 0 || strncmp(trimmed, "end function ", 14) == 0)
            {
                clearAllConstants();
            }
        }

        if(trimmed[0] != '\0')
        {
            size_t lineLength = strlen(trimmed);
            if(strlen(result) + lineLength + 2 >= capacity)
            {
                while(strlen(result) + lineLength + 2 >= capacity)
                {
                    capacity *= 2;
                }
                result = (char *)realloc(result, capacity);
                if(result == NULL)
                {
                    printf("Error de memoria al ampliar codigo optimizado\n");
                    exit(1);
                }
            }

            strcat(result, trimmed);
            strcat(result, "\n");
        }

        if(strncmp(trimmed, "goto ", 5) == 0)
        {
            skippingDeadCode = 1;
        }

        line = nextLineToken(&cursor);
    }

    free(working);
    return result;
}

int isArithmeticOperator(const char *op)
{
    return strcmp(op, "+") == 0 ||
           strcmp(op, "-") == 0 ||
           strcmp(op, "*") == 0 ||
           strcmp(op, "/") == 0;
}

int isComparisonOperator(const char *op)
{
    return strcmp(op, "<") == 0 ||
           strcmp(op, ">") == 0 ||
           strcmp(op, "<=") == 0 ||
           strcmp(op, ">=") == 0 ||
           strcmp(op, "==") == 0;
}

int isLogicalOperator(const char *op)
{
    return strcmp(op, "&&") == 0 || strcmp(op, "||") == 0;
}

int isLabelLine(const char *line)
{
    size_t length;

    if(line == NULL)
    {
        return 0;
    }

    length = strlen(line);
    return length > 1 && line[0] == 'L' && line[length - 1] == ':';
}

int labelIsReferenced(const char *source, const char *label)
{
    char *pattern;
    int referenced;

    if(source == NULL || label == NULL)
    {
        return 0;
    }

    pattern = formatText("goto %s", label);
    referenced = strstr(source, pattern) != NULL;
    free(pattern);
    return referenced;
}

const char *assemblyMnemonicForOperator(const char *op)
{
    if(strcmp(op, "+") == 0)
    {
        return "ADD";
    }
    if(strcmp(op, "-") == 0)
    {
        return "SUB";
    }
    if(strcmp(op, "*") == 0)
    {
        return "MUL";
    }
    if(strcmp(op, "/") == 0)
    {
        return "DIV";
    }
    if(strcmp(op, "<") == 0)
    {
        return "LT";
    }
    if(strcmp(op, ">") == 0)
    {
        return "GT";
    }
    if(strcmp(op, "<=") == 0)
    {
        return "LE";
    }
    if(strcmp(op, ">=") == 0)
    {
        return "GE";
    }
    if(strcmp(op, "==") == 0)
    {
        return "EQ";
    }
    if(strcmp(op, "&&") == 0)
    {
        return "AND";
    }
    if(strcmp(op, "||") == 0)
    {
        return "OR";
    }

    return "OP";
}

char *generateFinalCodeText(const char *source)
{
    if(source == NULL || source[0] == '\0')
    {
        return dupText("");
    }

    char *working = dupText(source);
    size_t capacity = strlen(source) * 3 + 512;
    char *result = (char *)malloc(capacity);
    if(result == NULL)
    {
        printf("Error de memoria al generar codigo final\n");
        exit(1);
    }

    result[0] = '\0';

    char *cursor = working;
    char *line = nextLineToken(&cursor);
    char pendingCallTemp[64];
    int hasPendingCallTemp = 0;

    pendingCallTemp[0] = '\0';

    while(line != NULL)
    {
        char *trimmed = line;
        while(*trimmed != '\0' && isspace((unsigned char)*trimmed))
        {
            trimmed++;
        }

        size_t trimmedLength = strlen(trimmed);
        while(trimmedLength > 0 && isspace((unsigned char)trimmed[trimmedLength - 1]))
        {
            trimmed[trimmedLength - 1] = '\0';
            trimmedLength--;
        }

        if(trimmed[0] == '\0')
        {
            line = nextLineToken(&cursor);
            continue;
        }

        if(isLabelLine(trimmed))
        {
            char labelName[64];

            if(sscanf(trimmed, "%63[^:]:", labelName) == 1 && !labelIsReferenced(source, labelName))
            {
                line = nextLineToken(&cursor);
                continue;
            }
        }

        char assembledLine[256];
        assembledLine[0] = '\0';

        char name[64];
        char type[64];
        char arg1[64];
        char arg2[64];
        char arg3[64];

        if(sscanf(trimmed, "function %63s %63s begin", name, type) == 2)
        {
            snprintf(assembledLine, sizeof(assembledLine), "%s PROC ; %s\n", name, type);
        }
        else if(sscanf(trimmed, "end function %63s", name) == 1)
        {
            snprintf(assembledLine, sizeof(assembledLine), "%s ENDP\n", name);
        }
        else if(strchr(trimmed, ':') != NULL && trimmed[strlen(trimmed) - 1] == ':')
        {
            snprintf(assembledLine, sizeof(assembledLine), "%s\n", trimmed);
        }
        else if(sscanf(trimmed, "goto %63s", name) == 1)
        {
            snprintf(assembledLine, sizeof(assembledLine), "JMP %s\n", name);
        }
        else if(sscanf(trimmed, "ifFalse %63s goto %63s", arg1, arg2) == 2)
        {
            snprintf(assembledLine, sizeof(assembledLine), "JZ %s, %s\n", arg1, arg2);
        }
        else if(sscanf(trimmed, "if %63s goto %63s", arg1, arg2) == 2)
        {
            snprintf(assembledLine, sizeof(assembledLine), "JNZ %s, %s\n", arg1, arg2);
        }
        else if(sscanf(trimmed, "print %63s", arg1) == 1)
        {
            snprintf(assembledLine, sizeof(assembledLine), "PRINT %s\n", arg1);
        }
        else if(sscanf(trimmed, "read %63s", arg1) == 1)
        {
            snprintf(assembledLine, sizeof(assembledLine), "READ %s\n", arg1);
        }
        else if(sscanf(trimmed, "return %63s", arg1) == 1)
        {
            snprintf(assembledLine, sizeof(assembledLine), "RET %s\n", arg1);
        }
        else if(strcmp(trimmed, "return") == 0)
        {
            snprintf(assembledLine, sizeof(assembledLine), "RET\n");
        }
        else if(sscanf(trimmed, "%63s = call %63s", arg1, arg2) == 2)
        {
            snprintf(assembledLine, sizeof(assembledLine), "CALL %s\n", arg2);
            snprintf(pendingCallTemp, sizeof(pendingCallTemp), "%s", arg1);
            hasPendingCallTemp = 1;
        }
        else if(sscanf(trimmed, "%63s = ! %63s", arg1, arg2) == 2)
        {
            snprintf(assembledLine, sizeof(assembledLine), "NOT %s, %s\n", arg1, arg2);
        }
        else if(sscanf(trimmed, "%63s = %63s %63s %63s", arg1, arg2, arg3, name) == 4)
        {
            if(isArithmeticOperator(arg3) || isComparisonOperator(arg3) || isLogicalOperator(arg3))
            {
                snprintf(assembledLine, sizeof(assembledLine), "%s %s, %s, %s\n", assemblyMnemonicForOperator(arg3), arg1, arg2, name);
            }
            else
            {
                snprintf(assembledLine, sizeof(assembledLine), "MOV %s, %s\n", arg1, arg2);
            }
        }
        else if(sscanf(trimmed, "%63s = %63s", arg1, arg2) == 2)
        {
            if(hasPendingCallTemp && strcmp(arg2, pendingCallTemp) == 0)
            {
                snprintf(assembledLine, sizeof(assembledLine), "MOV %s, R0\n", arg1);
                hasPendingCallTemp = 0;
                pendingCallTemp[0] = '\0';
            }
            else
            {
                snprintf(assembledLine, sizeof(assembledLine), "MOV %s, %s\n", arg1, arg2);
                hasPendingCallTemp = 0;
                pendingCallTemp[0] = '\0';
            }
        }
        else
        {
            hasPendingCallTemp = 0;
            pendingCallTemp[0] = '\0';
        }

        if(assembledLine[0] != '\0')
        {
            size_t needed = strlen(result) + strlen(assembledLine) + 1;
            if(needed >= capacity)
            {
                while(needed >= capacity)
                {
                    capacity *= 2;
                }

                result = (char *)realloc(result, capacity);
                if(result == NULL)
                {
                    printf("Error de memoria al ampliar codigo final\n");
                    exit(1);
                }
            }

            strcat(result, assembledLine);
        }

        line = nextLineToken(&cursor);
    }

    free(working);
    return result;
}

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

char *dupText(const char *text)
{
    if(text == NULL)
    {
        return strdup("");
    }

    return strdup(text);
}

char *formatText(const char *format, ...)
{
    va_list args;
    va_list copy;
    int size;
    char *buffer;

    va_start(args, format);
    va_copy(copy, args);
    size = vsnprintf(NULL, 0, format, copy);
    va_end(copy);

    buffer = (char *)malloc((size_t)size + 1);
    if(buffer == NULL)
    {
        printf("Error de memoria al generar codigo intermedio\n");
        exit(1);
    }

    vsnprintf(buffer, (size_t)size + 1, format, args);
    va_end(args);

    return buffer;
}

char *joinTexts(int count, ...)
{
    va_list args;
    size_t total = 1;

    va_start(args, count);
    for(int i = 0; i < count; i++)
    {
        char *part = va_arg(args, char *);
        if(part != NULL)
        {
            total += strlen(part);
        }
    }
    va_end(args);

    char *result = (char *)malloc(total);
    if(result == NULL)
    {
        printf("Error de memoria al concatenar codigo intermedio\n");
        exit(1);
    }

    result[0] = '\0';

    va_start(args, count);
    for(int i = 0; i < count; i++)
    {
        char *part = va_arg(args, char *);
        if(part != NULL)
        {
            strcat(result, part);
        }
    }
    va_end(args);

    return result;
}

char *newTemp()
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "t%d", ++tempCounter);
    return dupText(buffer);
}

char *newLabel()
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "L%d", ++labelCounter);
    return dupText(buffer);
}

ExprAttr *makeExprAttr(char *place, char *code)
{
    ExprAttr *attr = (ExprAttr *)malloc(sizeof(ExprAttr));
    if(attr == NULL)
    {
        printf("Error de memoria al crear atributo de expresion\n");
        exit(1);
    }

    attr->place = dupText(place);
    attr->code = dupText(code);
    return attr;
}

CodeAttr *makeCodeAttr(char *code)
{
    CodeAttr *attr = (CodeAttr *)malloc(sizeof(CodeAttr));
    if(attr == NULL)
    {
        printf("Error de memoria al crear atributo de codigo\n");
        exit(1);
    }

    attr->code = dupText(code);
    return attr;
}

ForAttr *makeForAttr(char *initCode, char *conditionCode, char *conditionPlace, char *updateCode)
{
    ForAttr *attr = (ForAttr *)malloc(sizeof(ForAttr));
    if(attr == NULL)
    {
        printf("Error de memoria al crear atributo de for\n");
        exit(1);
    }

    attr->initCode = dupText(initCode);
    attr->conditionCode = dupText(conditionCode);
    attr->conditionPlace = dupText(conditionPlace);
    attr->updateCode = dupText(updateCode);
    return attr;
}

IfAttr *makeIfAttr(char *conditionCode, char *conditionPlace, char *thenCode)
{
    IfAttr *attr = (IfAttr *)malloc(sizeof(IfAttr));
    if(attr == NULL)
    {
        printf("Error de memoria al crear atributo de if\n");
        exit(1);
    }

    attr->conditionCode = dupText(conditionCode);
    attr->conditionPlace = dupText(conditionPlace);
    attr->thenCode = dupText(thenCode);
    return attr;
}

void writeIntermediateCode()
{
    FILE *file = fopen("output/codigo_intermedio.txt", "w");

    if(file == NULL)
    {
        printf("Error: No se pudo crear el archivo de codigo intermedio.\n");
        return;
    }

    fprintf(file, "%s", generatedCode != NULL ? generatedCode : "");
    fclose(file);

    printf("\n[Info] Codigo intermedio guardado en 'output/codigo_intermedio.txt'\n");
}

void writeOptimizedIntermediateCode()
{
    optimizedCode = optimizeIntermediateCodeText(generatedCode);

    FILE *file = fopen("output/codigo_intermedio_optimizado.txt", "w");

    if(file == NULL)
    {
        printf("Error: No se pudo crear el archivo de codigo intermedio optimizado.\n");
        return;
    }

    fprintf(file, "%s", optimizedCode != NULL ? optimizedCode : "");
    fclose(file);

    printf("[Info] Codigo intermedio optimizado guardado en 'output/codigo_intermedio_optimizado.txt'\n");
}

void writeFinalCode()
{
    char *source = optimizedCode != NULL ? optimizedCode : generatedCode;
    char *finalCode = generateFinalCodeText(source);

    FILE *file = fopen("output/codigo_final.txt", "w");

    if(file == NULL)
    {
        printf("Error: No se pudo crear el archivo de codigo final.\n");
        free(finalCode);
        return;
    }

    fprintf(file, "%s", finalCode != NULL ? finalCode : "");
    fclose(file);
    free(finalCode);

    printf("[Info] Codigo final guardado en 'output/codigo_final.txt'\n");
}

/* Line 371 of yacc.c  */
#line 1225 "parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
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
     STRING_LITERAL = 261,
     CHAR_LITERAL = 262,
     COMPARISON = 263,
     ASSIGNATION = 264,
     PLUS = 265,
     MINUS = 266,
     MULTIPLY = 267,
     DIVIDE = 268,
     INCREMENT = 269,
     DECREMENT = 270,
     AND = 271,
     OR = 272,
     NOT = 273,
     TRUE_LITERAL = 274,
     FALSE_LITERAL = 275,
     INT = 276,
     FLOAT = 277,
     CHAR = 278,
     FOR = 279,
     IF = 280,
     ELSE = 281,
     WHILE = 282,
     DO = 283,
     SWITCH = 284,
     BREAK = 285,
     CASE = 286,
     DEFAULT = 287,
     STRUCT = 288,
     OPEN_PARENTHESIS = 289,
     CLOSE_PARENTHESIS = 290,
     OPEN_BRACE = 291,
     CLOSE_BRACE = 292,
     SEMICOLON = 293,
     COLON = 294,
     BOOL = 295,
     VOID = 296,
     RETURN = 297,
     PRINT = 298,
     READ = 299,
     LOWER_THAN_ELSE = 300
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 1158 "parser.y"

    char* string_val;
    ExprAttr* expr;
    CodeAttr* code;
    ForAttr* forpart;
    IfAttr* ifpart;


/* Line 387 of yacc.c  */
#line 1322 "parser.tab.c"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 1350 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   380

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNRULES -- Number of states.  */
#define YYNSTATES  179

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    12,    16,    19,    25,
      31,    33,    35,    37,    40,    43,    45,    48,    51,    53,
      56,    61,    65,    71,    77,    83,    89,    91,    95,    99,
     105,   111,   120,   128,   129,   132,   140,   146,   147,   150,
     153,   155,   159,   162,   168,   174,   176,   178,   180,   183,
     186,   188,   191,   197,   198,   203,   207,   209,   213,   217,
     221,   225,   229,   233,   237,   240,   244,   246,   248,   250,
     252,   254,   256,   258,   260,   262,   264,   266,   269,   272
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    -1,    48,    47,    -1,    49,    38,    -1,
      50,    -1,    42,    64,    38,    -1,    42,    38,    -1,    43,
      34,    64,    35,    38,    -1,    44,    34,     3,    35,    38,
      -1,    51,    -1,    53,    -1,    55,    -1,    55,    38,    -1,
      56,    38,    -1,    57,    -1,    61,    38,    -1,    30,    38,
      -1,    64,    -1,    65,     3,    -1,    65,     3,     9,    64,
      -1,     3,     9,    64,    -1,    65,     3,    34,    35,    63,
      -1,    41,     3,    34,    35,    63,    -1,    24,    34,    52,
      35,    63,    -1,    49,    38,    64,    38,    64,    -1,    54,
      -1,    54,    26,    53,    -1,    54,    26,    63,    -1,    25,
      34,    64,    35,    63,    -1,    27,    34,    64,    35,    63,
      -1,    28,    36,    47,    37,    27,    34,    64,    35,    -1,
      29,    34,    64,    35,    36,    58,    37,    -1,    -1,    48,
      58,    -1,    31,    64,    39,    59,    30,    38,    58,    -1,
      32,    39,    59,    30,    38,    -1,    -1,    60,    59,    -1,
      49,    38,    -1,    50,    -1,    42,    64,    38,    -1,    42,
      38,    -1,    43,    34,    64,    35,    38,    -1,    44,    34,
       3,    35,    38,    -1,    51,    -1,    53,    -1,    55,    -1,
      55,    38,    -1,    56,    38,    -1,    57,    -1,    61,    38,
      -1,    33,     3,    36,    62,    37,    -1,    -1,    65,     3,
      38,    62,    -1,    36,    47,    37,    -1,    66,    -1,    64,
      10,    64,    -1,    64,    11,    64,    -1,    64,    12,    64,
      -1,    64,    13,    64,    -1,    64,     8,    64,    -1,    64,
      16,    64,    -1,    64,    17,    64,    -1,    18,    64,    -1,
      34,    64,    35,    -1,    21,    -1,    22,    -1,    23,    -1,
      40,    -1,     3,    -1,     4,    -1,     6,    -1,     5,    -1,
       7,    -1,    19,    -1,    20,    -1,     3,    14,    -1,     3,
      15,    -1,     3,    34,    35,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1224,  1224,  1228,  1236,  1240,  1244,  1249,  1253,  1258,
    1262,  1266,  1270,  1274,  1278,  1282,  1286,  1290,  1296,  1300,
    1305,  1311,  1318,  1327,  1338,  1354,  1360,  1370,  1383,  1398,
    1404,  1419,  1432,  1444,  1447,  1452,  1461,  1469,  1472,  1479,
    1483,  1487,  1492,  1496,  1501,  1505,  1509,  1513,  1517,  1521,
    1525,  1529,  1535,  1547,  1550,  1560,  1566,  1570,  1579,  1588,
    1597,  1606,  1615,  1624,  1633,  1641,  1647,  1648,  1649,  1650,
    1653,  1657,  1661,  1665,  1669,  1673,  1677,  1681,  1689,  1697
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INT_NUMBER",
  "FLOAT_NUMBER", "STRING_LITERAL", "CHAR_LITERAL", "COMPARISON",
  "ASSIGNATION", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "INCREMENT",
  "DECREMENT", "AND", "OR", "NOT", "TRUE_LITERAL", "FALSE_LITERAL", "INT",
  "FLOAT", "CHAR", "FOR", "IF", "ELSE", "WHILE", "DO", "SWITCH", "BREAK",
  "CASE", "DEFAULT", "STRUCT", "OPEN_PARENTHESIS", "CLOSE_PARENTHESIS",
  "OPEN_BRACE", "CLOSE_BRACE", "SEMICOLON", "COLON", "BOOL", "VOID",
  "RETURN", "PRINT", "READ", "LOWER_THAN_ELSE", "$accept", "program",
  "statement", "declaration", "function_declaration", "for_statement",
  "for_content", "if_statement", "if_content", "while_statement",
  "do_while_statement", "switch_statement", "switch_content",
  "switch_case_body", "non_break_statement", "struct_statement",
  "struct_content", "action_braces", "operation", "type", "expression", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    49,    49,
      49,    49,    50,    50,    51,    52,    53,    53,    53,    54,
      55,    56,    57,    58,    58,    58,    58,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    62,    62,    63,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    65,    65,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     3,     2,     5,     5,
       1,     1,     1,     2,     2,     1,     2,     2,     1,     2,
       4,     3,     5,     5,     5,     5,     1,     3,     3,     5,
       5,     8,     7,     0,     2,     7,     5,     0,     2,     2,
       1,     3,     2,     5,     5,     1,     1,     1,     2,     2,
       1,     2,     5,     0,     4,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    70,    71,    73,    72,    74,     0,    75,    76,    66,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,     0,     2,     0,     5,    10,
      11,    26,    12,     0,    15,     0,    18,     0,    56,     0,
      77,    78,     0,    70,    64,     0,     0,     0,     2,     0,
      17,     0,     0,     0,     7,     0,     0,     0,     1,     3,
       4,     0,    13,    14,    16,     0,     0,     0,     0,     0,
       0,     0,    19,    21,    79,     0,     0,     0,     0,     0,
       0,     0,    53,    65,     0,     6,     0,     0,     2,    27,
      28,    61,    57,    58,    59,    60,    62,    63,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,     0,     0,    24,    29,    30,     0,
      33,    52,     0,    23,     8,     9,    55,    22,     0,     0,
       0,     0,    33,     0,    53,    25,     0,     0,    37,    34,
      32,    54,    31,    37,     0,     0,     0,     0,    40,    45,
      46,    47,     0,    50,     0,    37,     0,     0,    42,     0,
       0,     0,    39,    48,    49,     0,    38,    51,     0,    41,
       0,     0,    36,    33,     0,     0,    35,    43,    44
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    25,    26,    27,    28,    29,    76,    30,    31,    32,
      33,    34,   133,   154,   155,    35,   107,    90,    36,    37,
      38
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -127
static const yytype_int16 yypact[] =
{
     160,    84,  -127,  -127,  -127,  -127,     5,  -127,  -127,  -127,
    -127,  -127,   -12,    11,    12,    -8,    14,     4,    48,     5,
    -127,    81,   224,    52,    53,    88,   160,    58,  -127,  -127,
    -127,    79,    69,    75,  -127,    78,   350,   114,  -127,     5,
    -127,  -127,    86,     0,  -127,    49,     5,     5,   160,     5,
    -127,    83,   263,   103,  -127,    19,     5,   136,  -127,  -127,
    -127,   -18,  -127,  -127,  -127,     5,     5,     5,     5,     5,
       5,     5,    10,   350,  -127,   102,   106,   140,   273,   284,
     107,   294,    80,  -127,   110,  -127,   304,   138,   160,  -127,
    -127,   116,     8,     8,  -127,  -127,   367,   360,     5,   139,
       5,   141,   166,   141,   141,   149,   150,   154,   189,   141,
     167,   168,   170,   350,   141,    98,  -127,  -127,  -127,   174,
     128,  -127,   171,  -127,  -127,  -127,  -127,  -127,     5,     5,
       5,   179,   128,   185,    80,   350,   314,    65,   192,  -127,
    -127,  -127,  -127,   192,   234,   211,   212,   186,  -127,  -127,
    -127,   209,   210,  -127,   193,   192,   213,   219,  -127,   253,
       5,   254,  -127,  -127,  -127,   218,  -127,  -127,   221,  -127,
     324,   225,  -127,   128,   239,   240,  -127,  -127,  -127
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -127,   -22,  -115,   -43,   -64,   -13,  -127,   -58,  -127,   112,
     144,   190,  -126,  -106,  -127,   200,   159,   243,    -6,   -44,
    -127
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      44,    77,    75,    89,    59,   132,   139,    13,    43,     2,
       3,     4,     5,    52,    40,    41,    55,   132,    88,    98,
      68,    69,    45,     6,     7,     8,    80,    65,    48,    66,
      67,    68,    69,    73,    42,    70,    71,   157,   108,    19,
      78,    79,    50,    81,    99,    46,    47,   176,    49,   166,
      86,    51,     1,     2,     3,     4,     5,    85,   132,    91,
      92,    93,    94,    95,    96,    97,   112,     6,     7,     8,
       9,    10,    11,    65,   148,    66,    67,    68,    69,   148,
     150,    70,    71,    19,    53,   150,    56,    57,    58,    20,
     108,   148,   113,    39,   115,   147,    60,   150,    40,    41,
     147,     9,    10,    11,   143,    61,    65,    62,    66,    67,
      68,    69,   147,    63,    70,    71,    64,    72,    42,    82,
      20,    74,   135,   136,   137,   149,    66,    67,    68,    69,
     149,     1,     2,     3,     4,     5,   128,    84,   159,    87,
     100,   101,   149,   102,   105,   109,     6,     7,     8,     9,
      10,    11,    12,    13,   170,    14,    15,    16,    17,   130,
     131,    18,    19,     1,     2,     3,     4,     5,    20,    21,
      22,    23,    24,   111,   114,    98,   119,    88,     6,     7,
       8,     9,    10,    11,    12,    13,   120,    14,    15,    16,
      17,   121,   122,    18,    19,     1,     2,     3,     4,     5,
      20,    21,    22,    23,    24,   124,   125,   126,   129,   134,
       6,     7,     8,     9,    10,    11,    12,    13,   138,    14,
      15,    16,   140,   165,   162,    18,    19,    43,     2,     3,
       4,     5,    20,    21,   144,   145,   146,    43,     2,     3,
       4,     5,     6,     7,     8,   160,   161,   163,   164,   168,
     151,   167,     6,     7,     8,   151,   172,   171,    19,   173,
     175,    65,    54,    66,    67,    68,    69,   151,    19,    70,
      71,    65,   158,    66,    67,    68,    69,   177,   178,    70,
      71,    65,   152,    66,    67,    68,    69,   152,     0,    70,
      71,   169,    65,   141,    66,    67,    68,    69,    83,   152,
      70,    71,    65,     0,    66,    67,    68,    69,   103,     0,
      70,    71,    65,     0,    66,    67,    68,    69,     0,   104,
      70,    71,    65,     0,    66,    67,    68,    69,   153,   106,
      70,    71,    65,   153,    66,    67,    68,    69,   156,   110,
      70,    71,     0,   156,   116,   153,   117,   118,     0,   142,
       0,     0,   123,     0,     0,   156,     0,   127,    65,   174,
      66,    67,    68,    69,     0,     0,    70,    71,    65,     0,
      66,    67,    68,    69,     0,    65,    70,    66,    67,    68,
      69
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-127)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       6,    45,    45,    61,    26,   120,   132,    25,     3,     4,
       5,     6,     7,    19,    14,    15,    22,   132,    36,     9,
      12,    13,    34,    18,    19,    20,    48,     8,    36,    10,
      11,    12,    13,    39,    34,    16,    17,   143,    82,    34,
      46,    47,    38,    49,    34,    34,    34,   173,    34,   155,
      56,     3,     3,     4,     5,     6,     7,    38,   173,    65,
      66,    67,    68,    69,    70,    71,    88,    18,    19,    20,
      21,    22,    23,     8,   138,    10,    11,    12,    13,   143,
     138,    16,    17,    34,     3,   143,    34,    34,     0,    40,
     134,   155,    98,     9,   100,   138,    38,   155,    14,    15,
     143,    21,    22,    23,    39,    26,     8,    38,    10,    11,
      12,    13,   155,    38,    16,    17,    38,     3,    34,    36,
      40,    35,   128,   129,   130,   138,    10,    11,    12,    13,
     143,     3,     4,     5,     6,     7,    38,    34,   144,     3,
      38,    35,   155,     3,    37,    35,    18,    19,    20,    21,
      22,    23,    24,    25,   160,    27,    28,    29,    30,    31,
      32,    33,    34,     3,     4,     5,     6,     7,    40,    41,
      42,    43,    44,    35,    35,     9,    27,    36,    18,    19,
      20,    21,    22,    23,    24,    25,    36,    27,    28,    29,
      30,    37,     3,    33,    34,     3,     4,     5,     6,     7,
      40,    41,    42,    43,    44,    38,    38,    37,    34,    38,
      18,    19,    20,    21,    22,    23,    24,    25,    39,    27,
      28,    29,    37,    30,    38,    33,    34,     3,     4,     5,
       6,     7,    40,    41,    42,    43,    44,     3,     4,     5,
       6,     7,    18,    19,    20,    34,    34,    38,    38,    30,
     138,    38,    18,    19,    20,   143,    38,     3,    34,    38,
      35,     8,    38,    10,    11,    12,    13,   155,    34,    16,
      17,     8,    38,    10,    11,    12,    13,    38,    38,    16,
      17,     8,   138,    10,    11,    12,    13,   143,    -1,    16,
      17,    38,     8,   134,    10,    11,    12,    13,    35,   155,
      16,    17,     8,    -1,    10,    11,    12,    13,    35,    -1,
      16,    17,     8,    -1,    10,    11,    12,    13,    -1,    35,
      16,    17,     8,    -1,    10,    11,    12,    13,   138,    35,
      16,    17,     8,   143,    10,    11,    12,    13,   138,    35,
      16,    17,    -1,   143,   101,   155,   103,   104,    -1,    35,
      -1,    -1,   109,    -1,    -1,   155,    -1,   114,     8,    35,
      10,    11,    12,    13,    -1,    -1,    16,    17,     8,    -1,
      10,    11,    12,    13,    -1,     8,    16,    10,    11,    12,
      13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    18,    19,    20,    21,
      22,    23,    24,    25,    27,    28,    29,    30,    33,    34,
      40,    41,    42,    43,    44,    47,    48,    49,    50,    51,
      53,    54,    55,    56,    57,    61,    64,    65,    66,     9,
      14,    15,    34,     3,    64,    34,    34,    34,    36,    34,
      38,     3,    64,     3,    38,    64,    34,    34,     0,    47,
      38,    26,    38,    38,    38,     8,    10,    11,    12,    13,
      16,    17,     3,    64,    35,    49,    52,    65,    64,    64,
      47,    64,    36,    35,    34,    38,    64,     3,    36,    53,
      63,    64,    64,    64,    64,    64,    64,    64,     9,    34,
      38,    35,     3,    35,    35,    37,    35,    62,    65,    35,
      35,    35,    47,    64,    35,    64,    63,    63,    63,    27,
      36,    37,     3,    63,    38,    38,    37,    63,    38,    34,
      31,    32,    48,    58,    38,    64,    64,    64,    39,    58,
      37,    62,    35,    39,    42,    43,    44,    49,    50,    51,
      53,    55,    56,    57,    59,    60,    61,    59,    38,    64,
      34,    34,    38,    38,    38,    30,    59,    38,    30,    38,
      64,     3,    38,    38,    35,    35,    58,    38,    38
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 1224 "parser.y"
    {
        (yyval.code) = makeCodeAttr("");
        generatedCode = dupText("");
    }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 1229 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(1) - (2)].code)->code, (yyvsp[(2) - (2)].code)->code);
        (yyval.code) = makeCodeAttr(code);
        generatedCode = dupText(code);
    }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 1237 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 1241 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 1245 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(2) - (3)].expr)->code, formatText("return %s\n", (yyvsp[(2) - (3)].expr)->place));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 1250 "parser.y"
    {
        (yyval.code) = makeCodeAttr("return\n");
    }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 1254 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(3) - (5)].expr)->code, formatText("print %s\n", (yyvsp[(3) - (5)].expr)->place));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 1259 "parser.y"
    {
        (yyval.code) = makeCodeAttr(formatText("read %s\n", (yyvsp[(3) - (5)].string_val)));
    }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 1263 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 1267 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 1271 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 1275 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 1279 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 1283 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 1287 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 1291 "parser.y"
    {
        (yyval.code) = makeCodeAttr("break\n");
    }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 1297 "parser.y"
    {
        (yyval.code) = makeCodeAttr((yyvsp[(1) - (1)].expr)->code);
    }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 1301 "parser.y"
    {
        addSymbol((yyvsp[(2) - (2)].string_val), (yyvsp[(1) - (2)].string_val), "variable");
        (yyval.code) = makeCodeAttr(formatText("decl %s %s\n", (yyvsp[(1) - (2)].string_val), (yyvsp[(2) - (2)].string_val)));
    }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 1306 "parser.y"
    {
        addSymbol((yyvsp[(2) - (4)].string_val), (yyvsp[(1) - (4)].string_val), "variable");
        char *code = joinTexts(2, (yyvsp[(4) - (4)].expr)->code, formatText("%s = %s\n", (yyvsp[(2) - (4)].string_val), (yyvsp[(4) - (4)].expr)->place));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 1312 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(3) - (3)].expr)->code, formatText("%s = %s\n", (yyvsp[(1) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 1319 "parser.y"
    {
        addSymbol((yyvsp[(2) - (5)].string_val), (yyvsp[(1) - (5)].string_val), "function");
        char *code = joinTexts(3,
                               formatText("function %s %s begin\n", (yyvsp[(2) - (5)].string_val), (yyvsp[(1) - (5)].string_val)),
                               (yyvsp[(5) - (5)].code)->code,
                               formatText("end function %s\n", (yyvsp[(2) - (5)].string_val)));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 1328 "parser.y"
    {
        addSymbol((yyvsp[(2) - (5)].string_val), "void", "function");
        char *code = joinTexts(3,
                               formatText("function %s void begin\n", (yyvsp[(2) - (5)].string_val)),
                               (yyvsp[(5) - (5)].code)->code,
                               formatText("end function %s\n", (yyvsp[(2) - (5)].string_val)));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 1339 "parser.y"
    {
        char *startLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(7,
                               (yyvsp[(3) - (5)].forpart)->initCode,
                               formatText("%s:\n", startLabel),
                               (yyvsp[(3) - (5)].forpart)->conditionCode,
                               formatText("ifFalse %s goto %s\n", (yyvsp[(3) - (5)].forpart)->conditionPlace, endLabel),
                               (yyvsp[(5) - (5)].code)->code,
                               (yyvsp[(3) - (5)].forpart)->updateCode,
                               formatText("goto %s\n%s:\n", startLabel, endLabel));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 1355 "parser.y"
    {
        (yyval.forpart) = makeForAttr((yyvsp[(1) - (5)].code)->code, (yyvsp[(3) - (5)].expr)->code, (yyvsp[(3) - (5)].expr)->place, (yyvsp[(5) - (5)].expr)->code);
    }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 1361 "parser.y"
    {
        char *falseLabel = newLabel();
        char *code = joinTexts(4,
                               (yyvsp[(1) - (1)].ifpart)->conditionCode,
                               formatText("ifFalse %s goto %s\n", (yyvsp[(1) - (1)].ifpart)->conditionPlace, falseLabel),
                               (yyvsp[(1) - (1)].ifpart)->thenCode,
                               formatText("%s:\n", falseLabel));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 1371 "parser.y"
    {
        char *falseLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(6,
                               (yyvsp[(1) - (3)].ifpart)->conditionCode,
                               formatText("ifFalse %s goto %s\n", (yyvsp[(1) - (3)].ifpart)->conditionPlace, falseLabel),
                               (yyvsp[(1) - (3)].ifpart)->thenCode,
                               formatText("goto %s\n%s:\n", endLabel, falseLabel),
                               (yyvsp[(3) - (3)].code)->code,
                               formatText("%s:\n", endLabel));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 1384 "parser.y"
    {
        char *falseLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(6,
                               (yyvsp[(1) - (3)].ifpart)->conditionCode,
                               formatText("ifFalse %s goto %s\n", (yyvsp[(1) - (3)].ifpart)->conditionPlace, falseLabel),
                               (yyvsp[(1) - (3)].ifpart)->thenCode,
                               formatText("goto %s\n%s:\n", endLabel, falseLabel),
                               (yyvsp[(3) - (3)].code)->code,
                               formatText("%s:\n", endLabel));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 1399 "parser.y"
    {
        (yyval.ifpart) = makeIfAttr((yyvsp[(3) - (5)].expr)->code, (yyvsp[(3) - (5)].expr)->place, (yyvsp[(5) - (5)].code)->code);
    }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 1405 "parser.y"
    {
        char *startLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(6,
                               formatText("%s:\n", startLabel),
                               (yyvsp[(3) - (5)].expr)->code,
                               formatText("ifFalse %s goto %s\n", (yyvsp[(3) - (5)].expr)->place, endLabel),
                               (yyvsp[(5) - (5)].code)->code,
                               formatText("goto %s\n", startLabel),
                               formatText("%s:\n", endLabel));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 1420 "parser.y"
    {
        char *startLabel = newLabel();
        char *code = joinTexts(5,
                               formatText("%s:\n", startLabel),
                               (yyvsp[(3) - (8)].code)->code,
                               (yyvsp[(7) - (8)].expr)->code,
                               formatText("if %s goto %s\n", (yyvsp[(7) - (8)].expr)->place, startLabel),
                               "");
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 1433 "parser.y"
    {
        char *code = joinTexts(4,
                               (yyvsp[(3) - (7)].expr)->code,
                               formatText("switch %s begin\n", (yyvsp[(3) - (7)].expr)->place),
                               (yyvsp[(6) - (7)].code)->code,
                               "switch end\n");
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 1444 "parser.y"
    {
        (yyval.code) = makeCodeAttr("");
    }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 1448 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(1) - (2)].code)->code, (yyvsp[(2) - (2)].code)->code);
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 1453 "parser.y"
    {
        char *code = joinTexts(4,
                               formatText("case %s:\n", (yyvsp[(2) - (7)].expr)->place),
                               (yyvsp[(2) - (7)].expr)->code,
                               (yyvsp[(4) - (7)].code)->code,
                               (yyvsp[(7) - (7)].code)->code);
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 1462 "parser.y"
    {
        char *code = joinTexts(2, "default:\n", (yyvsp[(3) - (5)].code)->code);
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 1469 "parser.y"
    {
        (yyval.code) = makeCodeAttr("");
    }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 1473 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(1) - (2)].code)->code, (yyvsp[(2) - (2)].code)->code);
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 1480 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 1484 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 1488 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(2) - (3)].expr)->code, formatText("return %s\n", (yyvsp[(2) - (3)].expr)->place));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 1493 "parser.y"
    {
        (yyval.code) = makeCodeAttr("return\n");
    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 1497 "parser.y"
    {
        char *code = joinTexts(2, (yyvsp[(3) - (5)].expr)->code, formatText("print %s\n", (yyvsp[(3) - (5)].expr)->place));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 1502 "parser.y"
    {
        (yyval.code) = makeCodeAttr(formatText("read %s\n", (yyvsp[(3) - (5)].string_val)));
    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 1506 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 1510 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 1514 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 1518 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 1522 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 1526 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (1)].code);
    }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 1530 "parser.y"
    {
        (yyval.code) = (yyvsp[(1) - (2)].code);
    }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 1536 "parser.y"
    {
        addSymbol((yyvsp[(2) - (5)].string_val), "struct", "structure");
        char *code = joinTexts(3,
                               formatText("struct %s begin\n", (yyvsp[(2) - (5)].string_val)),
                               (yyvsp[(4) - (5)].code)->code,
                               formatText("struct %s end\n", (yyvsp[(2) - (5)].string_val)));
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 1547 "parser.y"
    {
        (yyval.code) = makeCodeAttr("");
    }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 1551 "parser.y"
    {
        char *code = joinTexts(3,
                               formatText("field %s %s\n", (yyvsp[(1) - (4)].string_val), (yyvsp[(2) - (4)].string_val)),
                               (yyvsp[(4) - (4)].code)->code,
                               "");
        (yyval.code) = makeCodeAttr(code);
    }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 1561 "parser.y"
    {
        (yyval.code) = (yyvsp[(2) - (3)].code);
    }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 1567 "parser.y"
    {
        (yyval.expr) = (yyvsp[(1) - (1)].expr);
    }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 1571 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 1580 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 1589 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 1598 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 1607 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 1616 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 1625 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               (yyvsp[(1) - (3)].expr)->code,
                               (yyvsp[(3) - (3)].expr)->code,
                               formatText("%s = %s %s %s\n", temp, (yyvsp[(1) - (3)].expr)->place, (yyvsp[(2) - (3)].string_val), (yyvsp[(3) - (3)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 1634 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(2,
                               (yyvsp[(2) - (2)].expr)->code,
                               formatText("%s = %s %s\n", temp, (yyvsp[(1) - (2)].string_val), (yyvsp[(2) - (2)].expr)->place));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 1642 "parser.y"
    {
        (yyval.expr) = (yyvsp[(2) - (3)].expr);
    }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 1647 "parser.y"
    { (yyval.string_val) = strdup("int"); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 1648 "parser.y"
    { (yyval.string_val) = strdup("float"); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 1649 "parser.y"
    { (yyval.string_val) = strdup("char"); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 1650 "parser.y"
    { (yyval.string_val) = strdup("bool"); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 1654 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 1658 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 1662 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 1666 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 1670 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 1674 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 1678 "parser.y"
    {
        (yyval.expr) = makeExprAttr((yyvsp[(1) - (1)].string_val), "");
    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 1682 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(2,
                               formatText("%s = %s + 1\n", temp, (yyvsp[(1) - (2)].string_val)),
                               formatText("%s = %s\n", (yyvsp[(1) - (2)].string_val), temp));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 1690 "parser.y"
    {
        char *temp = newTemp();
        char *code = joinTexts(2,
                               formatText("%s = %s - 1\n", temp, (yyvsp[(1) - (2)].string_val)),
                               formatText("%s = %s\n", (yyvsp[(1) - (2)].string_val), temp));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 1698 "parser.y"
    {
        char *temp = newTemp();
        char *code = formatText("%s = call %s\n", temp, (yyvsp[(1) - (3)].string_val));
        (yyval.expr) = makeExprAttr(temp, code);
    }
    break;


/* Line 1792 of yacc.c  */
#line 3489 "parser.tab.c"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1703 "parser.y"


extern char *yytext; 

void yyerror(char *msg)
{
    printf("\n[!] Error sintactico cerca del token: '%s'\n", yytext);
}

void writeSymbolTableToFile() 
{
    FILE *file = fopen("output/tabla_simbolos.txt", "w");

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
    printf("\n[Info] Tabla de simbolos guardada en 'output/tabla_simbolos.txt'\n");
}

int main(int argc, char *argv[]) {

    readInputFile(argc, argv);

    allTokens = (TokenData **)malloc(MAX_TOKENS * sizeof(TokenData *));

    if(yyparse() == 0) {
        printf("Analisis sintactico correcto\n");

        writeSymbolTableToFile();
        writeIntermediateCode();
        writeOptimizedIntermediateCode();
        writeFinalCode();
        writeTokenTypes();
        writeTokenTable();
        clearMemoryAllocation();
    }

    fclose(yyin);

    return 0;
}