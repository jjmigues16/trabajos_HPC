#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define NUM_PUNTOS 10000000

int main() {
    long long puntos_en_circulo = 0;
    
    double tiempo_inicio = omp_get_wtime();

    // --- SOLUCIÓN ÓPTIMA: 'reduction' para contar ---
    // Se elige 'reduction' por la misma razón que en el ejercicio 1: permite un conteo
    // paralelo sin cuellos de botella. Cada hilo cuenta en su propia variable privada.
    //
    // NOTA IMPORTANTE sobre rand(): La función 'rand()' no es segura para hilos (thread-safe)
    // porque usa un estado global. En su lugar, usamos 'rand_r()', que es reentrante y segura.
    // Cada hilo necesita su propia semilla; aquí usamos un truco con la variable de bucle 'i'
    // para darle a cada hilo una secuencia pseudo-aleatoria diferente.
    #pragma omp parallel for reduction(+:puntos_en_circulo)
    for (long long i = 0; i < NUM_PUNTOS; i++) {
        unsigned int semilla = (unsigned int)i; // Semilla diferente para cada iteración
        double x = (double)rand_r(&semilla) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&semilla) / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            puntos_en_circulo++;
        }
    }

    double pi_estimado = 4.0 * puntos_en_circulo / NUM_PUNTOS;
    double tiempo_fin = omp_get_wtime();

    printf("Pi estimado: %f\n", pi_estimado);
    printf("Tiempo de cálculo: %f segundos\n", tiempo_fin - tiempo_inicio);

    return 0;
}
