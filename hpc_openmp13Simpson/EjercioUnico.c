#include <stdio.h>
#include <math.h>
#include <omp.h>

// La función a integrar: f(x) = e^(-x^2)
double f(double x) {
    return exp(-x * x);
}

int main() {
    // Límites de integración
    const double a = -1.0;
    const double b = 1.0;

    // Número de subintervalos para los métodos de aplicación múltiple
    // Debe ser un número grande para que la paralelización sea efectiva.
    // N_13 debe ser par. N_38 debe ser múltiplo de 3.
    const int N_13 = 20000000;
    const int N_38 = 20000001; 

    double integral;
    double t_sec, t_par; // Tiempos de ejecución

    printf("Calculando la integral de e^(-x^2) en el intervalo [%.1f, %.1f]\n", a, b);
    printf("------------------------------------------------------------------\n\n");

    // --- 1. Método de Simpson 1/3 de aplicación simple ---
    printf("1. Método de Simpson 1/3 (Simple)\n");
    double h_13_s = (b - a) / 2.0;
    integral = (h_13_s / 3.0) * (f(a) + 4.0 * f(a + h_13_s) + f(b));
    printf("   Resultado: %f\n", integral);
    printf("   (Cálculo demasiado rápido para paralelizar de forma significativa)\n\n");

    // --- 2. Método de Simpson 3/8 de aplicación simple ---
    printf("2. Método de Simpson 3/8 (Simple)\n");
    double h_38_s = (b - a) / 3.0;
    integral = (3.0 * h_38_s / 8.0) * (f(a) + 3.0 * f(a + h_38_s) + 3.0 * f(a + 2 * h_38_s) + f(b));
    printf("   Resultado: %f\n", integral);
    printf("   (Cálculo demasiado rápido para paralelizar de forma significativa)\n\n");

    // --- 3. Método de Simpson 1/3 de aplicación múltiple ---
    printf("3. Método de Simpson 1/3 (Múltiple con N=%d)\n", N_13);
    double h_13_m = (b - a) / N_13;
    
    // Versión Secuencial
    double sum_odd = 0.0;
    double sum_even = 0.0;
    t_sec = omp_get_wtime();
    for (int i = 1; i < N_13; i++) {
        if (i % 2 == 1) { // Índices impares
            sum_odd += f(a + i * h_13_m);
        } else { // Índices pares
            sum_even += f(a + i * h_13_m);
        }
    }
    integral = (h_13_m / 3.0) * (f(a) + 4.0 * sum_odd + 2.0 * sum_even + f(b));
    t_sec = omp_get_wtime() - t_sec;
    printf("   [Secuencial] Resultado: %f, Tiempo: %f s\n", integral, t_sec);

    // Versión Paralela
    sum_odd = 0.0;
    sum_even = 0.0;
    t_par = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_odd) reduction(+:sum_even)
    for (int i = 1; i < N_13; i++) {
        if (i % 2 == 1) {
            sum_odd += f(a + i * h_13_m);
        } else {
            sum_even += f(a + i * h_13_m);
        }
    }
    integral = (h_13_m / 3.0) * (f(a) + 4.0 * sum_odd + 2.0 * sum_even + f(b));
    t_par = omp_get_wtime() - t_par;
    printf("   [Paralelo]   Resultado: %f, Tiempo: %f s\n", integral, t_par);
    printf("   => Speedup: %.2fx\n\n", t_sec / t_par);


    // --- 4. Método de Simpson 3/8 de aplicación múltiple ---
    printf("4. Método de Simpson 3/8 (Múltiple con N=%d)\n", N_38);
    double h_38_m = (b - a) / N_38;

    // Versión Secuencial
    double sum_mult_3 = 0.0;
    double sum_not_mult_3 = 0.0;
    t_sec = omp_get_wtime();
    for (int i = 1; i < N_38; i++) {
        if (i % 3 == 0) { // Índices múltiplos de 3
            sum_mult_3 += f(a + i * h_38_m);
        } else { // Resto de índices
            sum_not_mult_3 += f(a + i * h_38_m);
        }
    }
    integral = (3.0 * h_38_m / 8.0) * (f(a) + 3.0 * sum_not_mult_3 + 2.0 * sum_mult_3 + f(b));
    t_sec = omp_get_wtime() - t_sec;
    printf("   [Secuencial] Resultado: %f, Tiempo: %f s\n", integral, t_sec);

    // Version Paralela
    sum_mult_3 = 0.0;
    sum_not_mult_3 = 0.0;
    t_par = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_mult_3) reduction(+:sum_not_mult_3)
    for (int i = 1; i < N_38; i++) {
        if (i % 3 == 0) {
            sum_mult_3 += f(a + i * h_38_m);
        } else {
            sum_not_mult_3 += f(a + i * h_38_m);
        }
    }
    integral = (3.0 * h_38_m / 8.0) * (f(a) + 3.0 * sum_not_mult_3 + 2.0 * sum_mult_3 + f(b));
    t_par = omp_get_wtime() - t_par;
    printf("   [Paralelo]   Resultado: %f, Tiempo: %f s\n", integral, t_par);
    printf("   => Speedup: %.2fx\n\n", t_sec / t_par);

    return 0;
}
