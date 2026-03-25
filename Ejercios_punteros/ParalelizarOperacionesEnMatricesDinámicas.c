#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int filas = 5;
    int columnas = 10;
    const int VALOR_A_MULTIPLICAR = 3;
    int **matriz;

    // 1. Asignación de memoria para la matriz dinámica
    matriz = (int **)malloc(filas * sizeof(int *));
    for (int i = 0; i < filas; i++) {
        matriz[i] = (int *)malloc(columnas * sizeof(int));
    }

    // Inicialización de la matriz con valores
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            // Usando punteros: *(*(matriz + i) + j) es equivalente a matriz[i][j]
            *(*(matriz + i) + j) = i * columnas + j;
        }
    }
    
    printf("Operación: Multiplicar cada elemento por %d.\n", VALOR_A_MULTIPLICAR);
    double start_time = omp_get_wtime();

    // 2. Paralelización de la operación con OpenMP
    // La directiva se aplica al bucle externo. OpenMP dividirá las filas (iteraciones de 'i')
    // entre los hilos disponibles.
    #pragma omp parallel for
    for (int i = 0; i < filas; i++) {
        // Cada hilo trabaja en una o más filas completas.
        // printf("Hilo %d trabajando en la fila %d\n", omp_get_thread_num(), i); // Descomentar para ver la distribución
        for (int j = 0; j < columnas; j++) {
            // Accedemos y modificamos usando aritmética de punteros
            *(*(matriz + i) + j) *= VALOR_A_MULTIPLICAR;
        }
    }
    
    double end_time = omp_get_wtime();
    printf("La operación paralela tomó %f segundos.\n", end_time - start_time);

    // (Opcional) Imprimir la matriz para verificar
    printf("\nMatriz resultante (primeros 5x5 elementos):\n");
    for (int i = 0; i < 5 && i < filas; i++) {
        for (int j = 0; j < 5 && j < columnas; j++) {
            printf("%4d ", *(*(matriz + i) + j));
        }
        printf("\n");
    }

    // 3. Liberación de memoria
    for (int i = 0; i < filas; i++) {
        free(matriz[i]); // Libera cada fila
    }
    free(matriz); // Libera el puntero a las filas

    return 0;
}
