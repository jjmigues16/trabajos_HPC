#include <stdio.h>

// La función recibe un puntero al inicio del array y su tamaño.
void ordenarConPunteros(int *arr, int size) {
    int *fin = arr + size; // Puntero que apunta una posición DESPUÉS del último elemento.
    int *ptr_i, *ptr_j;
    int temp;

    // Bucle externo: recorre desde el inicio hasta el penúltimo elemento.
    for (ptr_i = arr; ptr_i < fin - 1; ptr_i++) {
        // Bucle interno: compara elementos adyacentes.
        for (ptr_j = arr; ptr_j < fin - (ptr_i - arr) - 1; ptr_j++) {
            // Comparamos el valor al que apunta ptr_j con el siguiente valor.
            if (*ptr_j > *(ptr_j + 1)) {
                // Intercambio de valores usando punteros
                temp = *ptr_j;
                *ptr_j = *(ptr_j + 1);
                *(ptr_j + 1) = temp;
            }
        }
    }
}

void imprimirArray(int *arr, int size) {
    int *ptr;
    int *fin = arr + size;
    for (ptr = arr; ptr < fin; ptr++) {
        printf("%d ", *ptr);
    }
    printf("\n");
}

int main() {
    int mi_array[] = {5, 3, 8, 4, 1, 9, 2};
    int size = sizeof(mi_array) / sizeof(mi_array[0]);

    printf("Array original: ");
    imprimirArray(mi_array, size);

    ordenarConPunteros(mi_array, size);

    printf("Array ordenado: ");
    imprimirArray(mi_array, size);

    return 0;
}
