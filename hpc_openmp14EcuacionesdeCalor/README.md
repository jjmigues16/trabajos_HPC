Simulaciones de la Ecuación del Calor en C con OpenMP
Este repositorio contiene una serie de programas en C para simular la Ecuación del Calor en 1, 2 y 3 dimensiones. Las simulaciones utilizan el método de diferencias finitas para resolver la ecuación de forma numérica y están optimizadas con OpenMP para aprovechar la computación paralela en procesadores multinúcleo.

1. Fundamento Teórico: La Ecuación del Calor
La Ecuación del Calor es una ecuación diferencial parcial que describe cómo se distribuye el calor (o varía la temperatura) en una región a lo largo del tiempo. Su forma general es:

∂
u
∂
t
=
α
∇
2
u
∂t
∂u
​
 =α∇ 
2
 u
Donde:

u
(
x
,
y
,
z
,
t
)
u(x,y,z,t): Es la temperatura en una posición y tiempo determinados.
t
t: Es el tiempo.
α
α: Es la difusividad térmica del material.
∇
2
∇ 
2
 : Es el operador Laplaciano, que representa la difusión del calor en el espacio.
2. Método de Solución: Diferencias Finitas
Para resolver esta ecuación en un computador, se utiliza el Método de Diferencias Finitas (FDM). Este método consiste en:

Discretizar el espacio: Se divide el dominio físico (una línea, una placa o un cubo) en una rejilla de puntos.
Discretizar el tiempo: Se avanza la simulación en pequeños pasos de tiempo 
Δ
t
Δt.
Aproximar las derivadas: Las derivadas parciales de la ecuación se reemplazan por aproximaciones algebraicas que relacionan el valor de la temperatura en un punto con el de sus vecinos.
Para evitar condiciones de carrera y asegurar la correcta lectura de datos en la paralelización, se utiliza una técnica de doble buffer (u_current y u_next), donde el estado futuro se calcula a partir del estado actual y luego se intercambian los roles de los arreglos.

3. Implementaciones Disponibles
El proyecto incluye tres implementaciones progresivas:

calor_1d.c: Simulación en una barra unidimensional.
calor_2d.c: Simulación en una placa bidimensional.
calor_3d.c: Simulación en un cubo tridimensional.
Todos los programas inicializan el sistema con una región de alta temperatura en el centro y condiciones de frontera de temperatura cero (condiciones de Dirichlet).

4. Cómo Compilar y Ejecutar
Prerrequisitos
Un compilador de C que soporte OpenMP (ej. gcc).
(Opcional) Python con las librerías pandas y matplotlib para la visualización de los resultados.
Paso 1: Compilación
Abre una terminal en la carpeta del proyecto y utiliza los siguientes comandos para compilar cada programa. La bandera -fopenmp es esencial para habilitar la paralelización.

Copiar
# Compilar la versión 1D
gcc -o calor_1d calor_1d.c -fopenmp -lm

# Compilar la versión 2D
gcc -o calor_2d calor_2d.c -fopenmp -lm

# Compilar la versión 3D
gcc -o calor_3d calor_3d.c -fopenmp -lm
Paso 2: Ejecución
Ejecuta los binarios compilados. Cada programa realizará los cálculos y guardará el estado final de la temperatura en un archivo .csv.

Copiar
# Ejecutar la simulación 1D
./calor_1d
# Generará: resultados_1d.csv

# Ejecutar la simulación 2D
./calor_2d
# Generará: resultados_2d.csv

# Ejecutar la simulación 3D
./calor_3d
# Generará: resultados_3d.csv
Paso 3: Visualización (Opcional)
Los scripts de Python (visualizar_2d.py, etc.) pueden ser usados para leer los archivos .csv y generar gráficos de los resultados.

Para 1D y 2D, se generan gráficos de línea y mapas de calor, respectivamente.
Para 3D, la visualización es más compleja. Los datos en resultados_3d.csv pueden ser analizados con herramientas más avanzadas (como Paraview) o se pueden adaptar los scripts de Python para mostrar "rebanadas" (slices) 2D del cubo 3D.
5. Conceptos Clave Demostrados
Modelado Físico: Aplicación de una ecuación diferencial parcial a un problema del mundo real.
Métodos Numéricos: Implementación del método de diferencias finitas.
Computación de Alto Rendimiento (HPC): Uso de C para cálculos intensivos.
Programación Paralela: Optimización de bucles con directivas OpenMP para acelerar la ejecución.
Manejo de Memoria en C: Asignación dinámica (malloc) y liberación (free) de memoria, y uso de la técnica de "pointer swapping" para eficiencia.
Flujo de Trabajo Científico: Separación de la computación (en C) y la visualización (en Python).