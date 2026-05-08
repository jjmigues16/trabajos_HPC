#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Necesario para OpenMP

// --- 1. Parámetros de la simulación ---
const double LX = 1.0;      // Longitud en X
const double LY = 1.0;      // Longitud en Y
const double T = 0.1;       // Tiempo total de la simulación
const double ALPHA = 0.005; // Difusividad térmica

const int NX = 50;          // Puntos en el eje X
const int NY = 50;          // Puntos en el eje Y
const int M = 10000;        // Pasos en el tiempo

int main() {
    // --- 2. Cálculo de deltas y gamma ---
    double dx = LX / NX;
    double dy = LY / NY;
    double dt = T / M;
    double gamma_x = ALPHA * dt / (dx * dx);
    double gamma_y = ALPHA * dt / (dy * dy);

    printf("Iniciando simulación 2D en C con OpenMP\n");
    printf("Parámetros: NX=%d, NY=%d, M=%d, gamma_x=%.4f, gamma_y=%.4f\n", NX, NY, M, gamma_x, gamma_y);

    // Verificación de la condición de estabilidad 2D
    if (gamma_x + gamma_y > 0.5) {
        printf("¡Alerta de estabilidad! gamma_x + gamma_y > 0.5. La simulación puede ser inestable.\n");
        return 1;
    }

    // --- 3. Manejo de memoria y condiciones iniciales ---
    // Aplanamos la rejilla 2D en un arreglo 1D
    int total_points = (NX + 1) * (NY + 1);
    double* u_current = (double*)malloc(total_points * sizeof(double));
    double* u_next = (double*)malloc(total_points * sizeof(double));

    // Condición inicial: Pulso de calor cuadrado en el centro
    for (int k = 0; k <= NY; k++) { // Iterar sobre y
        for (int i = 0; i <= NX; i++) { // Iterar sobre x
            int index = k * (NX + 1) + i;
            if (i >= NX / 4 && i <= 3 * NX / 4 && k >= NY / 4 && k <= 3 * NY / 4) {
                u_current[index] = 100.0;
            } else {
                u_current[index] = 0.0;
            }
        }
    }
    // Copiamos la condición inicial a u_next
    for(int i=0; i<total_points; ++i) u_next[i] = u_current[i];


    // --- 4. Bucle principal de simulación (en el tiempo) ---
    for (int j = 0; j < M; j++) {
        
        // El paralelismo se aplica en el bucle espacial más externo (sobre las filas 'y')
        #pragma omp parallel for
        for (int k = 1; k < NY; k++) { // Iterar sobre y (sin tocar fronteras)
            for (int i = 1; i < NX; i++) { // Iterar sobre x (sin tocar fronteras)
                
                // Índice del punto actual y sus vecinos en el arreglo aplanado
                int current_idx = k * (NX + 1) + i;
                int left_idx = k * (NX + 1) + (i - 1);
                int right_idx = k * (NX + 1) + (i + 1);
                int up_idx = (k + 1) * (NX + 1) + i;
                int down_idx = (k - 1) * (NX + 1) + i;

                // Laplaciano discretizado en 2D
                double laplacian_x = u_current[right_idx] - 2.0 * u_current[current_idx] + u_current[left_idx];
                double laplacian_y = u_current[up_idx] - 2.0 * u_current[current_idx] + u_current[down_idx];

                // Calculamos el siguiente estado basándonos en el actual
                u_next[current_idx] = u_current[current_idx] + gamma_x * laplacian_x + gamma_y * laplacian_y;
            }
        }

        // Intercambiamos los punteros (técnica de "swap" o "ping-pong")
        double* temp = u_current;
        u_current = u_next;
        u_next = temp;
    }

    printf("Simulación completada.\n");

    // --- 5. Guardar resultados en un archivo ---
    FILE *file = fopen("resultados_2d.csv", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo para escribir.\n");
        return 1;
    }

    // Escribimos los datos en formato CSV (x, y, temperatura)
    for (int k = 0; k <= NY; k++) {
        for (int i = 0; i <= NX; i++) {
            fprintf(file, "%f,%f,%f\n", i * dx, k * dy, u_current[k * (NX + 1) + i]);
        }
    }

    fclose(file);
    printf("Resultados guardados en 'resultados_2d.csv'.\n");

    // --- 6. Liberar la memoria ---
    free(u_current);
    free(u_next);

    return 0;
}
