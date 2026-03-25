#include <stdio.h>
#include <omp.h>

int main() {
    int sum = 0;
    int n = 100;

    // #pragma omp parallel for: Divide las iteraciones del bucle entre los hilos.
    // reduction(+:sum): Realiza una suma segura en paralelo.
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        sum = sum + i;
    }

    // Al final del bucle, OpenMP combina los resultados parciales en la variable 'sum' original.
    printf("La suma total de los números del 1 al %d es: %d\n", n, sum);

    return 0;
}
