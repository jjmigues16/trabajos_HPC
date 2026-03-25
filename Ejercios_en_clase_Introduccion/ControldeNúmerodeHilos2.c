#include <stdio.h>
#include <omp.h>

int main() {
    // Obtenemos el número de procesadores (lógicos) disponibles en el sistema.
    int max_procs = omp_get_num_procs();
    printf("Número de procesadores disponibles: %d\n", max_procs);

    // Calculamos la mitad y ajustamos el número de hilos para la próxima región paralela.
    int num_threads_to_use = max_procs / 2;
    omp_set_num_threads(num_threads_to_use);
    printf("Usando %d hilos (la mitad de los disponibles).\n\n", num_threads_to_use);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        // omp_get_num_threads() devuelve el número total de hilos en el equipo actual.
        int total_threads = omp_get_num_threads();

        printf("Soy el hilo %d de %d hilos en total.\n", thread_id, total_threads);
    }

    return 0;
}
