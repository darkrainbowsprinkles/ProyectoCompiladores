%{
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
    FILE *file = fopen("codigo_intermedio.txt", "w");

    if(file == NULL)
    {
        printf("Error: No se pudo crear el archivo de codigo intermedio.\n");
        return;
    }

    fprintf(file, "%s", generatedCode != NULL ? generatedCode : "");
    fclose(file);

    printf("\n[Info] Codigo intermedio guardado en 'codigo_intermedio.txt'\n");
}

void writeOptimizedIntermediateCode()
{
    optimizedCode = optimizeIntermediateCodeText(generatedCode);

    FILE *file = fopen("codigo_intermedio_optimizado.txt", "w");

    if(file == NULL)
    {
        printf("Error: No se pudo crear el archivo de codigo intermedio optimizado.\n");
        return;
    }

    fprintf(file, "%s", optimizedCode != NULL ? optimizedCode : "");
    fclose(file);

    printf("[Info] Codigo intermedio optimizado guardado en 'codigo_intermedio_optimizado.txt'\n");
}

void writeFinalCode()
{
    char *source = optimizedCode != NULL ? optimizedCode : generatedCode;
    char *finalCode = generateFinalCodeText(source);

    FILE *file = fopen("codigo_final.txt", "w");

    if(file == NULL)
    {
        printf("Error: No se pudo crear el archivo de codigo final.\n");
        free(finalCode);
        return;
    }

    fprintf(file, "%s", finalCode != NULL ? finalCode : "");
    fclose(file);
    free(finalCode);

    printf("[Info] Codigo final guardado en 'codigo_final.txt'\n");
}
%}

%union {
    char* string_val;
    ExprAttr* expr;
    CodeAttr* code;
    ForAttr* forpart;
    IfAttr* ifpart;
}

%type <string_val> type
%type <expr> operation expression
%type <code> program statement declaration function_declaration for_statement if_statement while_statement do_while_statement switch_statement struct_statement action_braces switch_content struct_content
%type <forpart> for_content
%type <ifpart> if_content
%token <string_val> IDENTIFIER

%token <string_val> INT_NUMBER
%token <string_val> FLOAT_NUMBER
%token <string_val> CHAR_LITERAL
%token <string_val> COMPARISON
%token <string_val> ASSIGNATION
%token <string_val> PLUS
%token <string_val> MINUS
%token <string_val> MULTIPLY 
%token <string_val> DIVIDE
%token <string_val> INCREMENT
%token <string_val> DECREMENT
%token <string_val> AND
%token <string_val> OR
%token <string_val> NOT
%token <string_val> TRUE_LITERAL
%token <string_val> FALSE_LITERAL

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
%token OPEN_PARENTHESIS
%token CLOSE_PARENTHESIS
%token OPEN_BRACE
%token CLOSE_BRACE
%token SEMICOLON
%token COLON
%token BOOL
%token VOID RETURN PRINT READ
%left OR
%left AND
%left COMPARISON
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right NOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
program:
    /* empty */
    {
        $$ = makeCodeAttr("");
        generatedCode = dupText("");
    }
    | statement program
    {
        char *code = joinTexts(2, $1->code, $2->code);
        $$ = makeCodeAttr(code);
        generatedCode = dupText(code);
    };

statement:
    declaration SEMICOLON
    {
        $$ = $1;
    }
    | function_declaration
    {
        $$ = $1;
    }
    | RETURN operation SEMICOLON
    {
        char *code = joinTexts(2, $2->code, formatText("return %s\n", $2->place));
        $$ = makeCodeAttr(code);
    }
    | RETURN SEMICOLON
    {
        $$ = makeCodeAttr("return\n");
    }
    | PRINT OPEN_PARENTHESIS operation CLOSE_PARENTHESIS SEMICOLON
    {
        char *code = joinTexts(2, $3->code, formatText("print %s\n", $3->place));
        $$ = makeCodeAttr(code);
    }
    | READ OPEN_PARENTHESIS IDENTIFIER CLOSE_PARENTHESIS SEMICOLON
    {
        $$ = makeCodeAttr(formatText("read %s\n", $3));
    }
    | for_statement
    {
        $$ = $1;
    }
    | if_statement
    {
        $$ = $1;
    }
    | while_statement
    {
        $$ = $1;
    }
    | while_statement SEMICOLON
    {
        $$ = $1;
    }
    | do_while_statement SEMICOLON
    {
        $$ = $1;
    }
    | switch_statement
    {
        $$ = $1;
    }
    | struct_statement SEMICOLON
    {
        $$ = $1;
    }
    | BREAK SEMICOLON
    {
        $$ = makeCodeAttr("break\n");
    };

declaration: 
    operation
    {
        $$ = makeCodeAttr($1->code);
    }
    | type IDENTIFIER
    {
        addSymbol($2, $1, "variable");
        $$ = makeCodeAttr(formatText("decl %s %s\n", $1, $2));
    }
    | type IDENTIFIER ASSIGNATION operation
    {
        addSymbol($2, $1, "variable");
        char *code = joinTexts(2, $4->code, formatText("%s = %s\n", $2, $4->place));
        $$ = makeCodeAttr(code);
    }
    | IDENTIFIER ASSIGNATION operation
    {
        char *code = joinTexts(2, $3->code, formatText("%s = %s\n", $1, $3->place));
        $$ = makeCodeAttr(code);
    };

function_declaration:
    type IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS action_braces
    {
        addSymbol($2, $1, "function");
        char *code = joinTexts(3,
                               formatText("function %s %s begin\n", $2, $1),
                               $5->code,
                               formatText("end function %s\n", $2));
        $$ = makeCodeAttr(code);
    }
    | VOID IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS action_braces
    {
        addSymbol($2, "void", "function");
        char *code = joinTexts(3,
                               formatText("function %s void begin\n", $2),
                               $5->code,
                               formatText("end function %s\n", $2));
        $$ = makeCodeAttr(code);
    };

for_statement: 
    FOR OPEN_PARENTHESIS for_content CLOSE_PARENTHESIS action_braces
    {
        char *startLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(7,
                               $3->initCode,
                               formatText("%s:\n", startLabel),
                               $3->conditionCode,
                               formatText("ifFalse %s goto %s\n", $3->conditionPlace, endLabel),
                               $5->code,
                               $3->updateCode,
                               formatText("goto %s\n%s:\n", startLabel, endLabel));
        $$ = makeCodeAttr(code);
    };

for_content:
    declaration SEMICOLON operation SEMICOLON operation
    {
        $$ = makeForAttr($1->code, $3->code, $3->place, $5->code);
    };

if_statement:
    if_content %prec LOWER_THAN_ELSE
    {
        char *falseLabel = newLabel();
        char *code = joinTexts(4,
                               $1->conditionCode,
                               formatText("ifFalse %s goto %s\n", $1->conditionPlace, falseLabel),
                               $1->thenCode,
                               formatText("%s:\n", falseLabel));
        $$ = makeCodeAttr(code);
    }
    | if_content ELSE if_statement
    {
        char *falseLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(6,
                               $1->conditionCode,
                               formatText("ifFalse %s goto %s\n", $1->conditionPlace, falseLabel),
                               $1->thenCode,
                               formatText("goto %s\n%s:\n", endLabel, falseLabel),
                               $3->code,
                               formatText("%s:\n", endLabel));
        $$ = makeCodeAttr(code);
    }
    | if_content ELSE action_braces
    {
        char *falseLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(6,
                               $1->conditionCode,
                               formatText("ifFalse %s goto %s\n", $1->conditionPlace, falseLabel),
                               $1->thenCode,
                               formatText("goto %s\n%s:\n", endLabel, falseLabel),
                               $3->code,
                               formatText("%s:\n", endLabel));
        $$ = makeCodeAttr(code);
    };

if_content:
    IF OPEN_PARENTHESIS operation CLOSE_PARENTHESIS action_braces
    {
        $$ = makeIfAttr($3->code, $3->place, $5->code);
    };

while_statement:
    WHILE OPEN_PARENTHESIS operation CLOSE_PARENTHESIS action_braces
    {
        char *startLabel = newLabel();
        char *endLabel = newLabel();
        char *code = joinTexts(6,
                               formatText("%s:\n", startLabel),
                               $3->code,
                               formatText("ifFalse %s goto %s\n", $3->place, endLabel),
                               $5->code,
                               formatText("goto %s\n", startLabel),
                               formatText("%s:\n", endLabel));
        $$ = makeCodeAttr(code);
    };

do_while_statement:
    DO OPEN_BRACE program CLOSE_BRACE WHILE OPEN_PARENTHESIS operation CLOSE_PARENTHESIS
    {
        char *startLabel = newLabel();
        char *code = joinTexts(5,
                               formatText("%s:\n", startLabel),
                               $3->code,
                               $7->code,
                               formatText("if %s goto %s\n", $7->place, startLabel),
                               "");
        $$ = makeCodeAttr(code);
    };

switch_statement:
    SWITCH OPEN_PARENTHESIS operation CLOSE_PARENTHESIS OPEN_BRACE switch_content CLOSE_BRACE
    {
        char *code = joinTexts(4,
                               $3->code,
                               formatText("switch %s begin\n", $3->place),
                               $6->code,
                               "switch end\n");
        $$ = makeCodeAttr(code);
    };

switch_content:
    /* empty */
    {
        $$ = makeCodeAttr("");
    }
    | statement switch_content
    {
        char *code = joinTexts(2, $1->code, $2->code);
        $$ = makeCodeAttr(code);
    }
    | CASE operation COLON program BREAK SEMICOLON switch_content
    {
        char *code = joinTexts(4,
                               formatText("case %s:\n", $2->place),
                               $2->code,
                               $4->code,
                               $7->code);
        $$ = makeCodeAttr(code);
    }
    | DEFAULT COLON program BREAK SEMICOLON
    {
        char *code = joinTexts(2, "default:\n", $3->code);
        $$ = makeCodeAttr(code);
    };

struct_statement:
    STRUCT IDENTIFIER OPEN_BRACE struct_content CLOSE_BRACE
    {
        addSymbol($2, "struct", "structure");
        char *code = joinTexts(3,
                               formatText("struct %s begin\n", $2),
                               $4->code,
                               formatText("struct %s end\n", $2));
        $$ = makeCodeAttr(code);
    };

struct_content:
    /* empty */
    {
        $$ = makeCodeAttr("");
    }
    | type IDENTIFIER SEMICOLON struct_content
    {
        char *code = joinTexts(3,
                               formatText("field %s %s\n", $1, $2),
                               $4->code,
                               "");
        $$ = makeCodeAttr(code);
    };

action_braces:
    OPEN_BRACE program CLOSE_BRACE
    {
        $$ = $2;
    };

operation: 
    expression
    {
        $$ = $1;
    }
    | operation PLUS operation
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | operation MINUS operation
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | operation MULTIPLY operation
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | operation DIVIDE operation
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | operation COMPARISON operation 
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | operation AND operation
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | operation OR operation
    {
        char *temp = newTemp();
        char *code = joinTexts(3,
                               $1->code,
                               $3->code,
                               formatText("%s = %s %s %s\n", temp, $1->place, $2, $3->place));
        $$ = makeExprAttr(temp, code);
    }
    | NOT operation
    {
        char *temp = newTemp();
        char *code = joinTexts(2,
                               $2->code,
                               formatText("%s = %s %s\n", temp, $1, $2->place));
        $$ = makeExprAttr(temp, code);
    }
    | OPEN_PARENTHESIS operation CLOSE_PARENTHESIS
    {
        $$ = $2;
    };

type:
    INT     { $$ = strdup("int"); }
    | FLOAT { $$ = strdup("float"); }
    | CHAR  { $$ = strdup("char"); }
    | BOOL  { $$ = strdup("bool"); };

expression: 
    IDENTIFIER 
    {
        $$ = makeExprAttr($1, "");
    }
    | INT_NUMBER 
    {
        $$ = makeExprAttr($1, "");
    }
    | FLOAT_NUMBER 
    {
        $$ = makeExprAttr($1, "");
    }
    | CHAR_LITERAL
    {
        $$ = makeExprAttr($1, "");
    }
    | TRUE_LITERAL
    {
        $$ = makeExprAttr($1, "");
    }
    | FALSE_LITERAL
    {
        $$ = makeExprAttr($1, "");
    }
    | IDENTIFIER INCREMENT
    {
        char *temp = newTemp();
        char *code = joinTexts(2,
                               formatText("%s = %s + 1\n", temp, $1),
                               formatText("%s = %s\n", $1, temp));
        $$ = makeExprAttr(temp, code);
    }
    | IDENTIFIER DECREMENT
    {
        char *temp = newTemp();
        char *code = joinTexts(2,
                               formatText("%s = %s - 1\n", temp, $1),
                               formatText("%s = %s\n", $1, temp));
        $$ = makeExprAttr(temp, code);
    }
    | IDENTIFIER OPEN_PARENTHESIS CLOSE_PARENTHESIS
    {
        char *temp = newTemp();
        char *code = formatText("%s = call %s\n", temp, $1);
        $$ = makeExprAttr(temp, code);
    }; 
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