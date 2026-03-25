// Archivo: matmul.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Usa un tamaño grande para que el cálculo tarde un tiempo medible.
#define N 2000

int main() {
    double *A = (double*)malloc(N * N * sizeof(double));
    double *B = (double*)malloc(N * N * sizeof(double));
    double *C = (double*)malloc(N * N * sizeof(double));
    if (!A || !B || !C) return 1;

    #pragma omp parallel for
    for (int i = 0; i < N * N; i++) {
        A[i] = 1.5;
        B[i] = 2.5;
    }

    printf("Calculando multiplicación de matrices %dx%d...\n", N, N);
    double tiempo_inicio = omp_get_wtime();

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
    printf("  -> Tiempo de ejecución: %f segundos\n\n", tiempo_fin - tiempo_inicio);

    free(A); free(B); free(C);
    return 0;
}


/** y en la terminal se escribe lo siguiente: # 1. Compilar el programa
gcc -fopenmp -O2 -o matmul matmul.c

# 2. Ejecutar SIN afinidad (lento)
echo "--- Ejecutando SIN afinidad (comportamiento por defecto) ---"
export OMP_PROC_BIND=false
./matmul

# 3. Ejecutar CON afinidad (rápido)
echo "--- Ejecutando CON afinidad (hilos fijados a núcleos) ---"
export OMP_PLACES=cores
export OMP_PROC_BIND=true
./matmul
*/