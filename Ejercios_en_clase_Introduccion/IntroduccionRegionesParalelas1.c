#include <stdio.h>
#include <omp.h>

int main() {
    // Establecemos el número de hilos que queremos usar en la región paralela.
    omp_set_num_threads(4);

    // Con #pragma omp parallel, creamos una región paralela.
    // El código dentro de las llaves {} será ejecutado por cada uno de los 4 hilos.
    #pragma omp parallel
    {
        // Obtenemos el ID del hilo actual. Cada hilo tendrá un número único.
        int thread_id = omp_get_thread_num();
        
        // Cada hilo imprime su propio mensaje.
        printf("Hola desde el hilo %d\n", thread_id);
    }

    return 0;
}
