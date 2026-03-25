#include <stdio.h>
#include <omp.h>

int main() {
    // 'total' se declara fuera de la región paralela, por lo que es 'shared' (compartida) por defecto.
    int total = 0;

    // Creamos una región con 4 hilos.
    #pragma omp parallel num_threads(4)
    {
        // 'mi_numero' se declara dentro de la región, por lo que es 'private' (privada) para cada hilo.
        // Cada hilo tendrá su propia copia de 'mi_numero'.
        int mi_numero = omp_get_thread_num() + 1; // Hilo 0 -> 1, Hilo 1 -> 2, etc.

        // Para evitar una condición de carrera al actualizar 'total', usamos una directiva 'atomic'.
        // Esto garantiza que la operación de suma se complete sin interrupciones.
        #pragma omp atomic
        total += mi_numero;
    }

    printf("El valor total (suma de 1+2+3+4) es: %d\n", total);

    return 0;
}
