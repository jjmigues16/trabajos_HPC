#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main() {
    float *v1 = (float*)malloc(N * sizeof(float));
    float *v2 = (float*)malloc(N * sizeof(float));
    if (!v1 || !v2) return 1;

    for(int i = 0; i < N; i++) { v1[i] = 1.0f; v2[i] = 2.0f; }

    double producto_punto = 0.0;
    double tiempo_inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: 'parallel for' + 'simd' + 'reduction' ---
    // Se elige esta combinación para maximizar el rendimiento.
    // 'parallel for' -> Divide el trabajo entre los núcleos.
    // 'simd' -> Usa las unidades vectoriales dentro de cada núcleo.
    // 'reduction' -> Realiza la suma de forma segura y escalable.
    // Es la receta completa para optimizar bucles de cálculo intensivo como este.
    #pragma omp parallel for simd reduction(+:producto_punto)
    for(int i = 0; i < N; i++) {
        producto_punto += v1[i] * v2[i];
    }

    double tiempo_fin = omp_get_wtime();
    printf("Producto punto: %f\n", producto_punto);
    printf("Tiempo: %f segundos\n", tiempo_fin - tiempo_inicio);

    free(v1); free(v2);
    return 0;
}
