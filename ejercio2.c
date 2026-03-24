#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 Reserva memoria para una matriz de 'filas' x 'cols'.
 Devuelve un puntero a la matriz o NULL si falla.
 */
double** crearMatriz(int filas, int cols) {
    double** matriz = (double**)malloc(filas * sizeof(double*));
    if (matriz == NULL) {
        printf("Error: No se pudo asignar memoria para las filas.\n");
        return NULL;
    }

    for (int i = 0; i < filas; i++) {
        matriz[i] = (double*)malloc(cols * sizeof(double));
        if (matriz[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d.\n", i);
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}

/**
 * Libera la memoria de una matriz creada con crearMatriz.
 */
void liberarMatriz(double** m, int filas) {
    if (m == NULL) {
        return;
    }
    for (int i = 0; i < filas; i++) {
        free(m[i]);
    }
    free(m);
}

/**
 *  Inicializa una matriz con valores de ejemplo.
 */
void inicializar(double** m, int filas, int cols, double valor) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            m[i][j] = valor;
        }
    }
}



/**
  Transpone una matriz M en una matriz T.
   M Matriz original de entrada.
   T Matriz de salida donde se almacenará la transpuesta.
  filas Número de filas de la matriz original M.
  cols Número de columnas de la matriz original M.


   Multiplica dos matrices A y B usando la estrategia de transposición
   para optimizar el acceso a caché. El resultado se guarda en C.
   Matriz A (factor izquierdo).
   Matriz B (factor derecho).
   Matriz C donde se guarda el resultado.
   Dimensión de las matrices cuadradas.
 */
void multiplicarMatricesTranspuesta(double** A, double** B, double** C, int n) {
    // 1. Crear matriz temporal para la transpuesta de B
    double** Bt = crearMatriz(n, n);
    if (Bt == NULL) {
        printf("Error: No se pudo crear la matriz transpuesta temporal.\n");
        return;
    }

    // 2. Transponer B en Bt
    transponerMatriz(B, Bt, n, n);

    // 3. Realizar la multiplicación con acceso optimizado
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double suma = 0.0;
            for (int k = 0; k < n; k++) {
                suma += A[i][k] * Bt[j][k];
            }
            C[i][j] = suma;
        }
    }

    // 4. Liberar la memoria de la matriz temporal
    liberarMatriz(Bt, n);
}

int main(int argc, char *argv[]) {
    // Usar N=1000 por defecto o el valor pasado como argumento
    int n = (argc > 1) ? atoi(argv[1]) : 1000;

    // Variables para medir el tiempo
    clock_t start, end;
    double time_total, time_mul1, time_mul2;

    start = clock(); // Inicia el cronómetro general

    printf("Creando matrices A, B y C de tamaño %dx%d...\n", n, n);
    double** A = crearMatriz(n, n);
    double** B = crearMatriz(n, n);
    double** C = crearMatriz(n, n);

    if (A == NULL || B == NULL || C == NULL) {
        printf("Error en la creación de una o más matrices. Abortando.\n");
        liberarMatriz(A, n);
        liberarMatriz(B, n);
        liberarMatriz(C, n);
        return 1;
    }

    printf("Inicializando matrices A y B...\n");
    inicializar(A, n, n, 1.0); // Matriz A llena de unos
    inicializar(B, n, n, 2.0); // Matriz B llena de doses

    // Medición Versión 1: Multiplicación Ingenua 
    printf("\n Medición Versión Original (Ingenua) \n");
    printf("Calculando C = A * B para N=%d...\n", n);
    clock_t start_mul1 = clock();
    multiplicarMatrices(A, B, C, n);
    clock_t end_mul1 = clock();
    time_mul1 = ((double) (end_mul1 - start_mul1)) / CLOCKS_PER_SEC;
    printf("Tiempo de CPU para la multiplicacion original: %f segundos\n", time_mul1);

    // Medición Versión 2: Multiplicación con Transpuesta 
    printf("\n Medición Versión Optimizada (Transpuesta) \n");
    printf("Calculando C = A * B (con B transpuesta) para N=%d...\n", n);
    clock_t start_mul2 = clock();
    multiplicarMatricesTranspuesta(A, B, C, n);
    clock_t end_mul2 = clock();
    time_mul2 = ((double) (end_mul2 - start_mul2)) / CLOCKS_PER_SEC;
    printf("Tiempo de CPU para la multiplicacion optimizada: %f segundos\n", time_mul2);

    //  Comparación de Resultados 
    printf("\n===========================================\n");
    printf("Comparacion para N=%d:\n", n);
    printf("Version Original:   %f segundos\n", time_mul1);
    printf("Version Optimizada: %f segundos\n", time_mul2);
    if (time_mul2 > 0) { // Evitar división por cero
        printf("Mejora: %.2fx mas rapido\n", time_mul1 / time_mul2);
    }
    printf("===========================================\n");

    printf("\nLiberando memoria de A, B y C...\n");
    liberarMatriz(A, n);
    liberarMatriz(B, n);
    liberarMatriz(C, n);
    printf("Memoria liberada.\n");

    end = clock(); // Detiene el cronómetro general
    time_total = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTiempo total de ejecucion del programa: %f segundos\n", time_total);

    return 0;
}
