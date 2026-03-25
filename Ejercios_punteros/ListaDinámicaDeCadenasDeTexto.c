#include <stdio.h>
#include <stdlib.h> // Para malloc y free
#include <string.h> // Para strlen y strcpy

int main() {
    int num_cadenas;
    char buffer[1024]; // Un buffer temporal para leer las cadenas

    printf("¿Cuántas cadenas de texto deseas almacenar? ");
    scanf("%d", &num_cadenas);

    // 1. Reservar memoria para el array de punteros (la lista)
    // Usamos char** porque es un puntero a un array de punteros a char.
    char **listaDeCadenas = (char **)malloc(num_cadenas * sizeof(char *));

    if (listaDeCadenas == NULL) {
        printf("Error: No se pudo asignar memoria para la lista.\n");
        return 1;
    }

    // Limpiar el buffer de entrada después de scanf
    while (getchar() != '\n');

    // 2. Pedir cada cadena y reservar memoria para ella
    for (int i = 0; i < num_cadenas; i++) {
        printf("Ingresa la cadena %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        
        // Quitar el salto de línea que fgets agrega al final
        buffer[strcspn(buffer, "\n")] = 0;

        // Reservar la memoria exacta para esta cadena (+1 para el carácter nulo '\0')
        listaDeCadenas[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));

        if (listaDeCadenas[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la cadena %d.\n", i + 1);
            // Liberar todo lo que se haya reservado hasta ahora para evitar fugas de memoria
            for(int j = 0; j < i; j++) free(listaDeCadenas[j]);
            free(listaDeCadenas);
            return 1;
        }
        
        // Copiar la cadena del buffer a su espacio de memoria definitivo
        strcpy(listaDeCadenas[i], buffer);
    }

    // 3. Imprimir las cadenas almacenadas
    printf("\n--- Cadenas almacenadas ---\n");
    for (int i = 0; i < num_cadenas; i++) {
        printf("Cadena %d: %s\n", i + 1, listaDeCadenas[i]);
    }

    // 4. Liberar la memoria (en orden inverso a la asignación)
    printf("\n--- Liberando memoria ---\n");
    // Primero, liberamos la memoria de cada cadena individual
    for (int i = 0; i < num_cadenas; i++) {
        free(listaDeCadenas[i]);
    }
    // Finalmente, liberamos la memoria del array de punteros
    free(listaDeCadenas);
    printf("Memoria liberada correctamente.\n");

    return 0;
}
