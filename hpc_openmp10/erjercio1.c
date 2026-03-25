#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000

int main() {
    double *vector = (double*)malloc(N * sizeof(double));
    if (!vector) return 1;

    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 10;
    }

    double suma_total = 0.0;
    double suma_cuadrada = 0.0; // Para el desafío adicional

    double tiempo_inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: Uso de 'reduction' ---
    // Se elige 'reduction' porque es la forma más eficiente y escalable de combinar valores en paralelo.
    // ¿Por qué es mejor?
    // 1. CERO CUELLOS DE BOTELLA: A diferencia de 'critical' o 'atomic' que fuerzan a los hilos a esperar,
    //    'reduction' crea una copia privada de 'suma_total' y 'suma_cuadrada' para cada hilo.
    // 2. TRABAJO 100% PARALELO: Cada hilo trabaja libremente en su copia local sin interferir con los demás.
    // 3. SINCRONIZACIÓN AUTOMÁTICA: Al final del bucle, OpenMP combina (suma) de forma segura los valores
    //    de todas las copias privadas en la variable original.
    #pragma omp parallel for reduction(+:suma_total, +:suma_cuadrada)
    for (int i = 0; i < N; i++) {
        suma_total += vector[i];
        suma_cuadrada += vector[i] * vector[i];
    }
    
    double tiempo_fin = omp_get_wtime();

    printf("Solución óptima con 'reduction':\n");
    printf("   Suma total: %f\n", suma_total);
    printf("   Suma de cuadrados: %f\n", suma_cuadrada);
    printf("   Tiempo: %f segundos\n", tiempo_fin - tiempo_inicio);

    free(vector);
    return 0;
}
