#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000

int main() {
    int *array = (int*)malloc(N * sizeof(int));
    for(int i=0; i<N; i++) array[i] = 1;

    long long total = 0;
    double inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: 'reduction' ---
    // Se elige 'reduction' porque es órdenes de magnitud más rápido que 'critical' para esta tarea.
    // ¿Por qué es mejor?
    // 'critical' crea un cuello de botella: obliga a todos los hilos a ponerse en fila para
    // actualizar la variable 'total' uno por uno. Esto serializa el código y anula la ventaja
    // del paralelismo.
    // 'reduction', en cambio, permite que cada hilo sume en su propia copia privada a máxima
    // velocidad. La sincronización ocurre una sola vez al final, de forma muy eficiente.
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < N; i++) {
        total += array[i];
    }
    
    double fin = omp_get_wtime();
    printf("Con 'reduction':\n  Total: %lld\n  Tiempo: %f s\n", total, fin - inicio);

    free(array);
    return 0;
}
