#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Necesario para OpenMP

// --- 1. Parámetros de la simulación ---
const double LX = 1.0;      // Longitud en X
const double LY = 1.0;      // Longitud en Y
const double LZ = 1.0;      // Longitud en Z
const double T = 0.05;      // Tiempo total (reducido por estabilidad)
const double ALPHA = 0.005; // Difusividad térmica

// Usamos menos puntos por eje debido al alto consumo de memoria
const int NX = 30;          // Puntos en el eje X
const int NY = 30;          // Puntos en el eje Y
const int NZ = 30;          // Puntos en el eje Z
const int M = 15000;        // Pasos en el tiempo (debe ser grande)

int main() {
    // --- 2. Cálculo de deltas y gamma ---
    // Asumimos dx = dy = dz para simplificar
    double dx = LX / NX;
    double dy = LY / NY;
    double dz = LZ / NZ;
    double dt = T / M;
    double gamma = ALPHA * dt / (dx * dx); // Asumiendo dx es el mismo para todos

    printf("Iniciando simulación 3D en C con OpenMP\n");
    printf("Parámetros: N=%d, M=%d, gamma=%.4f\n", NX, M, gamma);

    // Verificación de la condición de estabilidad 3D
    if (gamma > 1.0/6.0) {
        printf("¡Alerta de estabilidad! Gamma es > 1/6. La simulación puede ser inestable.\n");
        return 1;
    }

    // --- 3. Manejo de memoria y condiciones iniciales ---
    long int total_points = (NX + 1) * (NY + 1) * (NZ + 1);
    double* u_current = (double*)malloc(total_points * sizeof(double));
    double* u_next = (double*)malloc(total_points * sizeof(double));

    if (u_current == NULL || u_next == NULL) {
        printf("Error al alocar memoria. Reduzca el valor de N.\n");
        return 1;
    }

    // Condición inicial: Cubo de calor en el centro
    for (int l = 0; l <= NZ; l++) { // z
        for (int k = 0; k <= NY; k++) { // y
            for (int i = 0; i <= NX; i++) { // x
                long int index = (long)l * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + i;
                if (i >= NX / 4 && i <= 3 * NX / 4 &&
                    k >= NY / 4 && k <= 3 * NY / 4 &&
                    l >= NZ / 4 && l <= 3 * NZ / 4) {
                    u_current[index] = 100.0;
                } else {
                    u_current[index] = 0.0;
                }
            }
        }
    }
    // Copiamos la condición inicial a u_next
    for(long int i=0; i<total_points; ++i) u_next[i] = u_current[i];


    // --- 4. Bucle principal de simulación (en el tiempo) ---
    for (int j = 0; j < M; j++) {
        
        // Paralelizamos el bucle más externo del espacio (eje z)
        #pragma omp parallel for
        for (int l = 1; l < NZ; l++) { // z
            for (int k = 1; k < NY; k++) { // y
                for (int i = 1; i < NX; i++) { // x
                    
                    // Índice del punto actual y sus 6 vecinos
                    long int current_idx = (long)l * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + i;
                    long int right_idx   = (long)l * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + (i + 1);
                    long int left_idx    = (long)l * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + (i - 1);
                    long int up_idx      = (long)l * (NX + 1) * (NY + 1) + (long)(k + 1) * (NX + 1) + i;
                    long int down_idx    = (long)l * (NX + 1) * (NY + 1) + (long)(k - 1) * (NX + 1) + i;
                    long int front_idx   = (long)(l + 1) * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + i;
                    long int back_idx    = (long)(l - 1) * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + i;

                    // Laplaciano discretizado en 3D
                    double laplacian = (u_current[right_idx] - 2.0 * u_current[current_idx] + u_current[left_idx]) +
                                       (u_current[up_idx]   - 2.0 * u_current[current_idx] + u_current[down_idx]) +
                                       (u_current[front_idx]- 2.0 * u_current[current_idx] + u_current[back_idx]);

                    // Calculamos el siguiente estado
                    u_next[current_idx] = u_current[current_idx] + gamma * laplacian;
                }
            }
        }

        // Intercambiamos los punteros
        double* temp = u_current;
        u_current = u_next;
        u_next = temp;
    }

    printf("Simulación completada.\n");

    // --- 5. Guardar resultados en un archivo ---
    FILE *file = fopen("resultados_3d.csv", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo para escribir.\n");
        return 1;
    }

    // Escribimos los datos en formato CSV (x, y, z, temperatura)
    for (int l = 0; l <= NZ; l++) {
        for (int k = 0; k <= NY; k++) {
            for (int i = 0; i <= NX; i++) {
                long int index = (long)l * (NX + 1) * (NY + 1) + (long)k * (NX + 1) + i;
                fprintf(file, "%f,%f,%f,%f\n", i * dx, k * dy, l * dz, u_current[index]);
            }
        }
    }

    fclose(file);
    printf("Resultados guardados en 'resultados_3d.csv'.\n");

    // --- 6. Liberar la memoria ---
    free(u_current);
    free(u_next);

    return 0;
}
