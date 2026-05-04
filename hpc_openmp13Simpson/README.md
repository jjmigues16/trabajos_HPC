Optimización de Integración Numérica con OpenMP
Este proyecto demuestra cómo calcular una integral definida de forma numérica utilizando la Regla de Simpson y cómo optimizar drásticamente el rendimiento del cálculo mediante la paralelización con OpenMP.

Descripción del Problema
El objetivo es calcular la integral definida de la función gaussiana, que no tiene una solución analítica simple:

I
=
∫
−
1
1
e
−
x
2
 
d
x
I=∫ 
−1
1
​
 e 
−x 
2
 
 dx

Para lograr una alta precisión, se utiliza el método de aplicación múltiple, dividiendo el intervalo de integración en una gran cantidad de subintervalos (N > 20,000,000), lo que resulta en un cálculo computacionalmente intensivo.

Metodología
1. Método Numérico: Regla de Simpson
Se implementan las dos variantes principales de la regla para la aproximación:

Regla de Simpson 1/3: Utiliza polinomios de segundo grado.
Regla de Simpson 3/8: Utiliza polinomios de tercer grado.
El código compara una implementación secuencial (un solo bucle for) con una versión optimizada.

2. Optimización: Paralelización con OpenMP
El cuello de botella del rendimiento es el bucle for que itera millones de veces. La optimización principal consiste en paralelizar este bucle para que la carga de trabajo se distribuya entre todos los núcleos disponibles del procesador.

Esto se logra con la directiva de OpenMP:

Copiar
#pragma omp parallel for reduction(+:sum_variable)
#pragma omp parallel for: Divide las iteraciones del bucle entre múltiples hilos de ejecución.
reduction(+:...): Es una cláusula crucial que previene condiciones de carrera. Asegura que cada hilo trabaje en una copia privada de la variable de suma y, al final, combina todos los resultados parciales de forma segura para obtener el total correcto.
Requisitos
Un compilador de C (como GCC).
Soporte para OpenMP (incluido en la mayoría de las versiones modernas de GCC).
Compilación
Para compilar el programa, es esencial incluir las banderas -fopenmp (para habilitar OpenMP) y -lm (para enlazar la librería matemática).

Copiar
gcc -o simpson_calculator tu_archivo.c -fopenmp -lm
Reemplaza tu_archivo.c con el nombre de tu archivo de código fuente.

Uso
Una vez compilado, ejecuta el programa desde la terminal:

Copiar
./simpson_calculator
Ejemplo de Salida
El programa mostrará los resultados de cada método, comparando el rendimiento de la ejecución secuencial y la paralela. El resultado será similar a este:

Copiar
Calculando la integral de e^(-x^2) en el intervalo [-1.0, 1.0]
------------------------------------------------------------------

1. Método de Simpson 1/3 (Simple)
   Resultado: 1.493633
   (Cálculo demasiado rápido para paralelizar de forma significativa)

2. Método de Simpson 3/8 (Simple)
   Resultado: 1.493644
   (Cálculo demasiado rápido para paralelizar de forma significativa)

3. Método de Simpson 1/3 (Múltiple con N=20000000)
   [Secuencial] Resultado: 1.493648, Tiempo: 0.451234 s
   [Paralelo]   Resultado: 1.493648, Tiempo: 0.058123 s
   => Speedup: 7.76x

4. Método de Simpson 3/8 (Múltiple con N=20000001)
   [Secuencial] Resultado: 1.493648, Tiempo: 0.481357 s
   [Paralelo]   Resultado: 1.493648, Tiempo: 0.062458 s
   => Speedup: 7.71x