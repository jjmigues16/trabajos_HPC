#include <stdio.h>
#include <omp.h>

int main() {
    // Usamos 2 hilos, uno para cada sección.
    omp_set_num_threads(2);

    // #pragma omp parallel sections: Crea una región donde diferentes secciones
    // de código son distribuidas entre los hilos.
    #pragma omp parallel sections
    {
        // #pragma omp section: Define un bloque de código que será ejecutado por un hilo.
        #pragma omp section
        {
            printf("Hilo %d calculando tabla del 2:\n", omp_get_thread_num());
            for (int i = 1; i <= 10; i++) {
                printf("2 x %d = %d\n", i, 2 * i);
            }
        }

        // #pragma omp section: Define otro bloque que será ejecutado por otro hilo.
        #pragma omp section
        {
            printf("Hilo %d calculando tabla del 3:\n", omp_get_thread_num());
            for (int i = 1; i <= 10; i++) {
                printf("3 x %d = %d\n", i, 3 * i);
            }
        }
    }

    return 0;
}
