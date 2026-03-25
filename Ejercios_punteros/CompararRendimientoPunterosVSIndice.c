#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Para la función de cronómetro omp_get_wtime()

// Definimos un tamaño de array muy grande para que la operación tome tiempo medible.
#define TAMANO_ARRAY 100000000

// Versión 1: Usando índices tradicionales
long long sumaConIndices(int arr[], int size) {
    long long suma = 0;
    for (int i = 0; i < size; i++) {
        suma += arr[i];
    }
    return suma;
}

// Versión 2: Usando aritmética de punteros
long long sumaConPunteros(int *arr, int size) {
    long long suma = 0;
    int *fin = arr + size;
    for (int *ptr = arr; ptr < fin; ptr++) {
        suma += *ptr;
    }
    return suma;
}

int main() {
    // Reservar memoria para un array muy grande
    int *mi_array = (int *)malloc(TAMANO_ARRAY * sizeof(int));
    if (mi_array == NULL) return 1;

    // Inicializar el array
    for (int i = 0; i < TAMANO_ARRAY; i++) {
        mi_array[i] = i % 10; // Llenar con valores pequeños 0-9
    }

    double inicio, fin;
    long long resultado;

    // --- Medir tiempo de la versión con índices ---
    inicio = omp_get_wtime();
    resultado = sumaConIndices(mi_array, TAMANO_ARRAY);
    fin = omp_get_wtime();
    printf("Versión con Índices:\n");
    printf("  Resultado: %lld\n", resultado);
    printf("  Tiempo de ejecución: %f segundos\n\n", fin - inicio);

    // --- Medir tiempo de la versión con punteros ---
    inicio = omp_get_wtime();
    resultado = sumaConPunteros(mi_array, TAMANO_ARRAY);
    fin = omp_get_wtime();
    printf("Versión con Punteros:\n");
    printf("  Resultado: %lld\n", resultado);
    printf("  Tiempo de ejecución: %f segundos\n\n", fin - inicio);

    // Liberar memoria
    free(mi_array);

    return 0;
}
  