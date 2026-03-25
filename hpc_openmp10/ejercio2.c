#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 10000000 // 10 millones para el desafío

int main() {
    float *vector = (float*)malloc(N * sizeof(float));
    if (!vector) return 1;

    for(int i = 0; i < N; i++) {
        vector[i] = (float)rand() / (float)(RAND_MAX / 10.0);
    }

    double suma_cuadrados = 0.0;
    double tiempo_inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: Combinación de directivas ---
    // Se elige esta combinación porque ataca la optimización en múltiples niveles.
    // 1. 'parallel for': Distribuye las iteraciones del bucle entre los diferentes núcleos de la CPU.
    // 2. 'simd': Instruye al compilador para que use instrucciones vectoriales del CPU (SIMD: Single
    //    Instruction, Multiple Data). Esto permite que una sola instrucción realice la operación
    //    (vector[i] * vector[i]) sobre múltiples elementos del array a la vez DENTRO de cada hilo.
    // 3. 'reduction(+:...)': Como en el ejercicio anterior, realiza la suma de forma paralela y segura.
    #pragma omp parallel for simd reduction(+:suma_cuadrados)
    for(int i = 0; i < N; i++) {
        suma_cuadrados += vector[i] * vector[i];
    }
    
    double norma = sqrt(suma_cuadrados);
    double tiempo_fin = omp_get_wtime();

    printf("La norma del vector es: %f\n", norma);
    printf("Tiempo de cálculo: %f segundos\n", tiempo_fin - tiempo_inicio);

    free(vector);
    return 0;
}
