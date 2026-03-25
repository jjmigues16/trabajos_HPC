#include <stdio.h>
#include <omp.h>

#define N 40

void procesar(int i) {
    if (i < 10) { // Las primeras 10 iteraciones son "pesadas"
        volatile double d = 0;
        for(int k=0; k<200000000; ++k) d += 1.0;
    } else { // El resto son ligeras
        volatile double d = 0;
        for(int k=0; k<1000; ++k) d += 1.0;
    }
}

int main() {
    double inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: 'schedule(dynamic)' ---
    // Se elige 'dynamic' porque el trabajo del bucle está desbalanceado.
    // ¿Por qué es mejor?
    // 'schedule(static)' (el default) dividiría las 40 iteraciones en bloques contiguos.
    // Con 4 hilos, el Hilo 0 recibiría las iteraciones 0-9 (todas pesadas) y tardaría
    // muchísimo más que los demás, creando un cuello de botella.
    // 'schedule(dynamic)' asigna las iteraciones una por una (o en pequeños chunks) a
    // medida que los hilos quedan libres. Así, las tareas pesadas se distribuyen entre
    // todos los hilos, y ninguno se queda inactivo.
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < N; i++) {
        procesar(i);
    }
    
    double fin = omp_get_wtime();
    printf("schedule(dynamic): %f s\n", fin - inicio);

    return 0;
}
