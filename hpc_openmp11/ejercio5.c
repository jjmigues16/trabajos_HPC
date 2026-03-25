#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000
#define M 10000

int main() {
    double *matriz = (double*)malloc(N * M * sizeof(double));
    double inicio, fin;

    inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: Acceso por Filas (Contiguo) ---
    // Se elige este orden de bucles porque respeta cómo C almacena los datos en memoria.
    // ¿Por qué es mejor?
    // 1. ORDEN DE MEMORIA: C almacena las matrices en "orden de fila principal" (row-major).
    //    Esto significa que los elementos de una misma fila (matriz[i][0], matriz[i][1], ...)
    //    están uno al lado del otro en la memoria RAM.
    // 2. LOCALIDAD ESPACIAL Y CACHÉ: Al recorrer el bucle interno 'j', accedemos a posiciones
    //    de memoria contiguas. Cuando la CPU carga el primer elemento en su caché, también
    //    carga los elementos adyacentes. Los accesos siguientes son "cache hits" (aciertos),
    //    que son extremadamente rápidos.
    // 3. El acceso por columnas (invertir los bucles i y j) provocaría saltos enormes en la
    //    memoria, resultando en "cache misses" (fallos) constantes y un rendimiento pésimo.
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {       // Bucle externo sobre filas
        for (int j = 0; j < M; j++) {   // Bucle interno sobre columnas
            matriz[i * M + j] += 1.0;
        }
    }
    
    fin = omp_get_wtime();
    printf("Acceso por filas (contiguo y rápido): %f s\n", fin - inicio);

    free(matriz);
    return 0;
}
