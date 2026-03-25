#include <stdio.h>
#include <omp.h>

#define NUM_PASOS 10

int main() {
    double x0 = 0.0, v0 = 5.0, a = 2.0, dt = 0.5;
    double posiciones[NUM_PASOS], velocidades[NUM_PASOS];

    // --- SOLUCIÓN ÓPTIMA: 'parallel for' simple ---
    // Se elige 'parallel for' porque el problema es "vergonzosamente paralelo".
    // ¿Por qué?
    // Las fórmulas v(t) = v0 + a*t y x(t) = x0 + v0*t + 0.5*a*t^2 muestran que cada
    // cálculo para un tiempo 't' depende únicamente de las constantes iniciales (x0, v0, a).
    // No hay dependencia entre la iteración 'i' y la 'i-1'. Por lo tanto, no se necesita
    // ninguna sincronización compleja, y el trabajo se puede dividir limpiamente entre los hilos.
    #pragma omp parallel for
    for (int i = 0; i < NUM_PASOS; i++) {
        double t = (i + 1) * dt;
        velocidades[i] = v0 + a * t;
        posiciones[i] = x0 + v0 * t + 0.5 * a * t * t;
    }

    printf("Resultados del cálculo de MRUA:\n");
    for (int i = 0; i < NUM_PASOS; i++) {
        printf("t=%.2f -> v=%.2f, x=%.2f\n", (i + 1) * dt, velocidades[i], posiciones[i]);
    }

    return 0;
}
