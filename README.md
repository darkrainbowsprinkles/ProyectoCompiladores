# Proyecto de Compiladores

Este repositorio contiene el desarrollo de un compilador académico para un mini-lenguaje inspirado en C. El trabajo se hizo con Flex y Bison, y hasta este punto el proyecto ya cubre el análisis léxico, el análisis sintáctico, la tabla de símbolos, la generación de código intermedio y una traducción final a un pseudocódigo de ensamblador simple.

La intención del proyecto no es crear un compilador industrial, sino mostrar de forma clara cómo una entrada fuente se va transformando por etapas. Por eso el código genera varios archivos de salida para revisar cada fase por separado.

## Qué hace el compilador

Actualmente el compilador puede:

- Reconocer declaraciones de variables, funciones sin parámetros y estructuras simples.
- Analizar expresiones aritméticas y booleanas.
- Procesar estructuras de control como `if`, `if/else`, `while`, `do-while`, `for` y `switch`.
- Leer e imprimir valores con instrucciones de entrada y salida.
- Registrar identificadores en una tabla de símbolos.
- Generar código intermedio de tres direcciones con temporales y etiquetas.
- Aplicar una optimización básica sobre ese intermedio.
- Producir una versión final en pseudocódigo de ensamblador simple.

## Archivos principales

Todo el código fuente del compilador está dentro de la carpeta `Codigo/`.

- `lexer.l`: reglas léxicas.
- `parser.y`: gramática, acciones semánticas y generación de salidas.
- `input.txt`, `input1.txt`, `input2.txt`, `input3.txt`: programas de prueba.
- `test.c`: archivo de prueba adicional.

Durante la ejecución se generan estos archivos de texto:

- `tokens.txt`
- `table.txt`
- `tabla_simbolos.txt`
- `codigo_intermedio.txt`
- `codigo_intermedio_optimizado.txt`
- `codigo_final.txt`

## Cómo compilar

Desde la carpeta `Codigo/`, el flujo normal es:

```bash
yacc -d parser.y
flex lexer.l
gcc lex.yy.c parser.tab.c -o compilador.exe
```

Si el entorno usa Bison en lugar de `yacc`, el equivalente también funciona:

```bash
bison -d parser.y
flex lexer.l
gcc lex.yy.c parser.tab.c -o compilador.exe
```

## Cómo ejecutar

Una vez compilado, el programa se ejecuta redirigiendo un archivo de entrada:

```bash
compilador.exe < input.txt
```

También se pueden probar los demás archivos de entrada para revisar distintos casos:

- `input1.txt` para precedencia de operadores y condiciones booleanas.
- `input2.txt` para funciones, lectura, impresión y retornos.
- `input3.txt` para ver optimización y eliminación de código muerto.

## Estado actual

En este punto el compilador ya permite seguir el recorrido completo de un programa pequeño: leerlo, validarlo, registrar símbolos, generar intermedio, optimizarlo y producir una salida final más simple. Las pruebas del repositorio se hicieron justamente para revisar cada una de esas etapas.

## Reporte

El reporte del proyecto está en `Reporte_Proyecto.tex`.
