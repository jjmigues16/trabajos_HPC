#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000 // Usar un tamaño grande para ver el efecto

int main() {
    double *A = (double*)malloc(N * N * sizeof(double));
    double *B = (double*)malloc(N * N * sizeof(double));
    double *C = (double*)malloc(N * N * sizeof(double));
    
    // (Inicialización de matrices A y B...)
    for(int i=0; i<N*N; ++i) { A[i] = 1.0; B[i] = 1.0; }

    double tiempo_inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: Uso de 'collapse' ---
    // Se elige 'collapse(2)' porque es ideal para paralelizar bucles anidados perfectamente.
    // ¿Por qué es mejor?
    // 1. MAYOR ESPACIO DE ITERACIÓN: En lugar de solo paralelizar las N iteraciones del bucle 'i',
    //    'collapse(2)' le dice a OpenMP que trate los bucles 'i' y 'j' como un solo gran bucle
    //    con N*N iteraciones.
    // 2. MEJOR BALANCEO DE CARGA: Al tener un número mucho mayor de iteraciones para distribuir,
    //    el planificador de OpenMP puede repartir el trabajo de manera más equitativa entre los
    //    hilos, especialmente si N no es un múltiplo perfecto del número de hilos.
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double suma = 0.0;
            for (int k = 0; k < N; k++) {
                suma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = suma;
        }
    }
    
    double tiempo_fin = omp_get_wtime();
    printf("Tiempo con 'collapse(2)': %f segundos\n", tiempo_fin - tiempo_inicio);

    free(A); free(B); free(C);
    return 0;
}
