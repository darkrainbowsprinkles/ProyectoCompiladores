# Compilador Académico para Mini-C (Flex & Bison)

Este repositorio contiene el desarrollo de un **compilador académico** completo para un mini-lenguaje inspirado en la sintaxis de C, diseñado e implementado utilizando las herramientas clásicas de automatización de compiladores: **Flex** (analizador léxico) y **Bison/Yacc** (analizador sintáctico LALR). 

El objetivo principal de este proyecto es didáctico: ilustrar de forma transparente el flujo de transformación de un código fuente a través de todas las etapas clásicas de la ingeniería de compiladores, desde el flujo de caracteres hasta la emisión de código final.

---

## Arquitectura y Capacidades del Compilador

El sistema procesa el código fuente de manera secuencial y genera salidas independientes para auditoría de cada fase en la carpeta `Codigo/output/`.

### 1. Fase Front-End (Análisis Léxico, Sintáctico y Semántico)
* **Análisis Léxico (`lexer.l`):** Reconocimiento de palabras clave, operadores aritméticos/lógicos, delimitadores e identificadores. Genera `tabla_tokens.txt`.
* **Análisis Sintáctico (`parser.y`):** Implementación de una gramática libre de contexto mediante algoritmos de parsing SLR/LALR. 
* **Gestión de Símbolos:** Validación semántica y registro de tipos, ámbitos (scopes) y variables. Produce `tabla_variables.txt` y `tabla_simbolos.txt`.

### 2. Estructuras de Control y Expresiones Soportadas
* **Declaraciones:** Variables primitivas, funciones sin parámetros (y sus retornos) junto con estructuras de datos simples.
* **Expresiones:** Evaluación y precedencia estricta de expresiones aritméticas y booleanas complejas.
* **Flujo de Control:** Soporte completo para bifurcaciones y ciclos:
  * `if` / `if-else`
  * `while` y `do-while`
  * Ciclos `for`
  * Estructuras de selección múltiple `switch`
* **I/O Básica:** Instrucciones nativas para la lectura e impresión de valores en consola.

### 3. Fase Back-End (Generación y Optimización de Código)
* **Código de Tres Direcciones (TAC):** Traducción de la jerarquía sintáctica a un código intermedio lineal basado en variables temporales y etiquetas de salto. Genera `codigo_intermedio.txt`.
* **Optimización Básica:** Módulo de optimización sobre el TAC para la simplificación de expresiones algebraicas y la eliminación de código muerto. Produce `codigo_intermedio_optimizado.txt`.
* **Síntesis de Código Final:** Generación de una versión equivalente en un pseudocódigo de ensamblador simplificado de bajo nivel. Genera `codigo_final.txt`.

---

## Estructura del Repositorio

~~~bash
├── Codigo/
│   ├── lexer.l                         # Especificación léxica (Flex)
│   ├── parser.y                        # Especificación sintáctica y semántica (Bison)
│   ├── input.txt                       # Prueba general de estructuras de control
│   ├── input1.txt                      # Prueba de precedencia y álgebra booleana
│   ├── input2.txt                      # Prueba de funciones, I/O y retornos
│   ├── input3.txt                      # Prueba de optimización de código intermedio
│   ├── test.c                          # Archivo de pruebas misceláneas
│   └── output/                         # Artefactos generados tras la compilación
│       ├── tabla_tokens.txt
│       ├── tabla_variables.txt
│       ├── tabla_simbolos.txt
│       ├── codigo_intermedio.txt
│       ├── codigo_intermedio_optimizado.txt
│       └── codigo_final.txt
├── Reporte_Proyecto.tex                # Documentación formal del proyecto en LaTeX
└── README.md                           # Presentación del repositorio
~~~

---

## Construcción y Compilación

Para compilar el proyecto desde la raíz de la carpeta `Codigo/`, asegúrate de contar con las herramientas del entorno de desarrollo de GNU (`flex`, `bison` o `yacc`, y `gcc`).

### Opción A: Utilizando Bison (Recomendado)
~~~bash
bison -d parser.y
flex lexer.l
gcc lex.yy.c parser.tab.c -o compilador.exe
~~~

### Opción B: Utilizando Yacc clásico
~~~bash
yacc -d parser.y
flex lexer.l
gcc lex.yy.c parser.tab.c -o compilador.exe
~~~

*El flag `-d` es crítico en ambos casos para generar la cabecera (`.h`) que comunica los tokens mapeados desde el parser hacia el lexer.*

---

## Ejecución y Pruebas del Sistema

El ejecutable opera mediante la redirección del flujo de entrada estándar. Puedes evaluar el comportamiento de los diferentes módulos del compilador procesando los scripts de prueba provistos:

~~~bash
# Ejecución del caso de prueba base
./compilador.exe < input.txt

# Pruebas específicas por escenario:
./compilador.exe < input1.txt   # Evalúa la lógica booleana y prioridad de operadores
./compilador.exe < input2.txt   # Evalúa el comportamiento de funciones y subrutinas
./compilador.exe < input3.txt   # Muestra la depuración de código muerto y optimización TAC
~~~

Tras la ejecución de cualquiera de los comandos anteriores, inspecciona la carpeta `output/` para analizar de manera secuencial los reportes de salida correspondientes a la ejecución semántica de tu código fuente.

---

## Documentación Formal

Para una revisión en profundidad acerca de las decisiones de diseño sintáctico, el diseño de la tabla de símbolos mediante estructuras de datos eficientes, la especificación de la gramática libre de contexto y los criterios de traducción intermedio/final, por favor consulta el reporte técnico escrito en LaTeX ubicado en el archivo raíz:
> `Reporte_Proyecto.tex`
