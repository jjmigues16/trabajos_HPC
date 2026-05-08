#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Necesario para OpenMP

// --- 1. Parámetros de la simulación ---
const double L = 1.0;       // Longitud de la barra (metros)
const double T = 0.5;       // Tiempo total de la simulación (segundos)
const double ALPHA = 0.005; // Difusividad térmica del material

const int N = 100;          // Número de puntos en el espacio
const int M = 5000;         // Número de pasos en el tiempo

int main() {
    // --- 2. Cálculo de los deltas y gamma ---
    double dx = L / N;
    double dt = T / M;
    double gamma = ALPHA * dt / (dx * dx);

    printf("Iniciando simulación 1D en C con OpenMP\n");
    printf("Parámetros: N=%d, M=%d, gamma=%.4f\n", N, M, gamma);

    // Verificación de la condición de estabilidad
    if (gamma > 0.5) {
        printf("¡Alerta de estabilidad! Gamma es > 0.5. La simulación puede ser inestable.\n");
        return 1; // Salir con error
    }

    // --- 3. Manejo de memoria y condiciones iniciales ---
    // Usamos un "doble buffer": uno para el tiempo actual, otro para el siguiente
    double* u_current = (double*)malloc((N + 1) * sizeof(double));
    double* u_next = (double*)malloc((N + 1) * sizeof(double));

    // Condición inicial (t=0): Pulso de calor en el centro
    for (int i = 0; i <= N; i++) {
        if (i >= N / 4 && i <= 3 * N / 4) {
            u_current[i] = 100.0;
        } else {
            u_current[i] = 0.0;
        }
    }
    // Copiamos la condición inicial a u_next para tener un estado base
    for (int i = 0; i <= N; i++) {
        u_next[i] = u_current[i];
    }


    // --- 4. Bucle principal de simulación (en el tiempo) ---
    for (int j = 0; j < M; j++) {
        
        // El paralelismo se aplica en el bucle espacial (el más costoso)
        #pragma omp parallel for
        for (int i = 1; i < N; i++) {
            // Calculamos el siguiente estado basándonos en el actual
            u_next[i] = u_current[i] + gamma * (u_current[i + 1] - 2.0 * u_current[i] + u_current[i - 1]);
        }

        // Intercambiamos los punteros (técnica de "swap" o "ping-pong")
        // Esto es mucho más eficiente que copiar todos los datos del arreglo.
        double* temp = u_current;
        u_current = u_next;
        u_next = temp;
    }

    printf("Simulación completada.\n");

    // --- 5. Guardar resultados en un archivo ---
    FILE *file = fopen("resultados_1d.csv", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo para escribir.\n");
        return 1;
    }

    // Escribimos los datos en formato CSV (posición, temperatura)
    for (int i = 0; i <= N; i++) {
        fprintf(file, "%f,%f\n", i * dx, u_current[i]);
    }

    fclose(file);
    printf("Resultados guardados en 'resultados_1d.csv'.\n");

    // --- 6. Liberar la memoria ---
    free(u_current);
    free(u_next);

    return 0;
}
