#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

// 1. Reservar memoria para el arreglo de punteros a filas (nuestro matriz en el diagrama)

double** crearMatriz(int filas, int cols) {
   
    double** matriz = (double**)malloc(filas * sizeof(double*)); // Se pide  espacio para filas punteros de tipo double

    // 2.Valida que memoria si se haaya podido asignar para el arreglo de punteros.

    if (matriz == NULL) { 
        printf("Error: No se pudo asignar memoria para las filas.\n");
        return NULL; // Si Falló, no pude continuar.
    }

    
    // 3. Bucle para reservar memoria para cada fila individual.

    for (int z = 0; z < filas; z++) {
        
        matriz[z] = (double*)malloc(cols * sizeof(double));// Para cada puntero en nuestro arreglo, le asignamos un bloque de memoria
        // para 'cols' números de tipo 'double'.


        // 4. VALIDACIÓN DENTRO DEL BUCLE: Verificar si la fila se pudo crear.

        if (matriz[z] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d.\n", z);
            
            for (int j = 0; j < z; j++) {
                free(matriz[j]); // Libera las filas creadas exitosamente antes del error.
            }
            free(matriz); // Libera el arreglo de punteros.
            return NULL;
        }
    }

    // 5. Si todo salió bien, retornamos el puntero a nuestra nueva matriz.
    return matriz;
}


void liberarMatriz(double** m, int filas) {
    if (m == NULL) {
        return; // No hay nada que liberar.
    }
    //PROCESO DE LIBERACIÓN DE MEMORIA:
    // 1. Primero, liberamos la memoria de cada fila individual.
    // Este es el orden INVERSO a la creación.
    for (int z = 0; z < filas; z++) {
        free(m[z]); // Libera el bloque de memoria de la fila 'z'.
    }

    // 2. Una vez que todas las filas están libres, liberamos el arreglo de punteros.
    free(m);
}


void inicializar(double** m, int filas, int cols, double valor_base) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            // Esta es la notación de aritmética de punteros requerida.
            // Es equivalente a m[i][j]
            *(*(m + i) + j) = valor_base + (double)i * cols + (double)j;
        }
    }
}

/**
 * @brief Imprime una matriz en la consola con un título.
 */
void imprimirMatriz(double** m, int filas, int cols, const char* nombre) {
    printf("\n%s (%dx%d):\n", nombre, filas, cols);
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            // Usamos la notación de corchetes aquí por simplicidad.
            printf("%8.2f ", m[i][j]);
        }
        printf("\n"); // Salto de línea al final de cada fila.
    }
}


void sumarMatrices(double** A, double** B, double** C, int filas, int cols) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            *(*(C + i) + j) = *(*(A + i) + j) + *(*(B + i) + j);
        }
    }
}


void multiplicarMatrices(double** A, double** B, double** C, int n) {
    // Recorremos las filas de C (y de A)
    for (int i = 0; i < n; i++) {
        // Recorremos las columnas de C (y de B)
        for (int j = 0; j < n; j++) {
            // Inicializamos el valor de la celda C[i][j] a 0.0
            C[i][j] = 0.0;
            // Bucle interno para calcular el producto punto
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    
    int n = (argc > 1) ? atoi(argv[1]) : 1000;

    // Variables para medir el tiempo
    clock_t start, end;
    double cpu_time_used_sum, cpu_time_used_mul;

    printf("Creando matrices A, B y C de tamaño %dx%d...\n", n, n);
    double** A = crearMatriz(n, n);
    double** B = crearMatriz(n, n);
    double** C = crearMatriz(n, n);

    if (A == NULL || B == NULL || C == NULL) {
        printf("Error en la creación de una o más matrices.\n");
        liberarMatriz(A, n);
        liberarMatriz(B, n);
        liberarMatriz(C, n);
        return 1;
    }

    inicializar(A, n, n, 0.0);
    inicializar(B, n, n, 100.0);

    // --- Medición de la Suma 
    printf("\nCalculando Suma (C = A + B) para N=%d \n", n);
    start = clock();
    sumarMatrices(A, B, C, n, n);
    end = clock();
    cpu_time_used_sum = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de CPU para la suma: %f segundos: \n", cpu_time_used_sum);

    //  Medición de la Multiplicación 
    printf("\nCalculando Multiplicacion (C = A * B) para N=%d \n", n);
    start = clock();
    multiplicarMatrices(A, B, C, n);
    end = clock();
    cpu_time_used_mul = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de CPU para la multiplicacion: %f segundos: \n", cpu_time_used_mul);

    
   
    if (n <=4)  {
        imprimirMatriz(A, n, n, "Matriz A");
        imprimirMatriz(B, n, n, "Matriz B");
        sumarMatrices(A, B, C, n, n);
        imprimirMatriz(C, n, n, "Resultado Suma (C)");
        multiplicarMatrices(A, B, C, n);
        imprimirMatriz(C, n, n, "Resultado Multiplicacion (C)");
    }

    printf("\nLiberando memoria de A, B y C...\n");
    liberarMatriz(A, n);
    liberarMatriz(B, n);
    liberarMatriz(C, n);
    printf("Memoria liberada.\n");

    return 0;
}
