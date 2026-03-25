#include <stdio.h>
#include <math.h>

#define N 10
#define MAX_ITER 100
#define TOLERANCIA 1e-6

int main() {
    double A[N][N], b[N], x[N];
    // (Inicializar A, b, y x con ceros)
    for(int i=0; i<N; ++i) {
        b[i] = i+1;
        x[i] = 0;
        for(int j=0; j<N; ++j) {
            A[i][j] = (i==j) ? N : 1.0;
        }
    }

    int iter;
    for (iter = 0; iter < MAX_ITER; iter++) {
        double max_error = 0.0;

        // --- SOLUCIÓN CORRECTA: Bucle Secuencial ---
        // NO se puede usar '#pragma omp parallel for' aquí.
        // ¿Por qué?
        // El método de Gauss-Seidel tiene una DEPENDENCIA DE DATOS A TRAVÉS DEL BUCLE.
        // El cálculo de 'x[i]' en esta misma iteración del bucle 'iter'
        // utiliza los valores de 'x[j]' donde j < i, que fueron calculados
        // en esta MISMA iteración 'iter'.
        // Por ejemplo, para calcular x[1], se necesita el x[0] recién calculado.
        // Paralelizar este bucle rompería esa dependencia y daría un resultado incorrecto.
        // La paralelización de Gauss-Seidel requiere algoritmos más avanzados (ej. coloración).
        for (int i = 0; i < N; i++) {
            double sigma = 0.0;
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    sigma += A[i][j] * x[j];
                }
            }
            double x_viejo = x[i];
            x[i] = (b[i] - sigma) / A[i][i];
            if (fabs(x[i] - x_viejo) > max_error) {
                max_error = fabs(x[i] - x_viejo);
            }
        }

        if (max_error < TOLERANCIA) break;
    }

    printf("Convergencia en %d iteraciones.\n", iter);
    return 0;
}
