#include <stdio.h>
#include <omp.h>

#define TAREAS 10000

void realizarTareaPequena(int i) {
    for(int j=0; j < 1000; j++) { volatile double d = j*i; }
}

int main() {
    double inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: Un solo bloque paralelo ---
    // Se elige este patrón porque evita el altísimo coste de crear y destruir
    // el equipo de hilos repetidamente.
    // ¿Por qué es mejor?
    // El enfoque ineficiente (crear un '#pragma omp parallel' dentro de un bucle for)
    // paga el sobrecosto de la creación de hilos en CADA iteración.
    // Este enfoque óptimo crea el equipo de hilos UNA SOLA VEZ. Luego, la directiva
    // '#pragma omp for' simplemente distribuye las iteraciones del bucle entre los
    // hilos que ya existen, lo cual es extremadamente rápido.
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < TAREAS; i++) {
            realizarTareaPequena(i);
        }
    }
    
    double fin = omp_get_wtime();
    printf("Un solo bloque 'parallel' con 'for': %f s\n", fin - inicio);

    return 0;
}
