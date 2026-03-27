# Proyecto de Optimización: Multiplicación de Matrices en C

Hola, soy Jose Miguel Jimenez Sanchez y este es un proyecto para la materia de **Computación de Alto Rendimiento (HPC)**. El objetivo era tomar una tarea simple como la multiplicación de matrices y hacerla lo más rápida posible, aplicando los conceptos que vimos en clase.

## El Problema: ¿Por qué optimizar?

Empezamos con una multiplicación de matrices normal, la que todos aprendemos en álgebra. El código es sencillo, pero cuando las matrices son muy grandes (por ejemplo, 2000x2000), ¡se vuelve súper lento!

El proyecto se dividió en 3 partes o "ejercicios":

1.  **La Versión Lenta (Ingenua):** El código base, sin ninguna optimización.
2.  **La Versión Inteligente (Caché):** El primer intento de hacerlo más rápido.
3.  **La Versión Súper Rápida (Paralela):** Usando todo el poder de la CPU.

---

## Preguntas y Aprendizajes del Proyecto

### 1. ¿Cuál fue el primer problema que encontraste? (ejercicio1.c)

El primer código que hicimos fue el de `ejercicio1.c`. Usa tres bucles `for` anidados para multiplicar las matrices `A` y `B`.

```c
// Bucle de la versión ingenua
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            suma += A[i][k] * B[k][j];
        }
    }
}
```

El problema está en `B[k][j]`. Mientras el bucle más interno (el de `k`) avanza, estamos accediendo a la matriz `A` de forma secuencial (`A[i][0]`, `A[i][1]`, `A[i][2]`, ...), lo cual es bueno. Pero en la matriz `B`, estamos "saltando" por toda la memoria (`B[0][j]`, `B[1][j]`, `B[2][j]`, ...).

El profe nos explicó que esto es malísimo para la **memoria caché** de la CPU. La caché es una memoria pequeña y súper rápida donde la CPU guarda los datos que cree que va a necesitar pronto. Al saltar por toda la memoria con `B`, la caché no sirve de nada y la CPU tiene que ir a buscar los datos a la memoria RAM, que es mucho más lenta.

### 2. ¿Cómo solucionaste el problema de la caché? (ejercicio2.c)

¡Aquí es donde se puso interesante! La solución fue un truco bastante inteligente: si el problema es que recorremos la matriz `B` por columnas, ¿por qué no la "acostamos"?

Eso es lo que hace la **transposición**. Creamos una nueva matriz, `Bt`, que es la versión transpuesta de `B`. Ahora, en lugar de multiplicar `A[i][k] * B[k][j]`, hacemos la multiplicación `A[i][k] * Bt[j][k]`.

```c
// Bucle de la versión optimizada para caché
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            suma += A[i][k] * Bt[j][k];
        }
    }
}
```

¡Miren la diferencia! Ahora, cuando el bucle de `k` avanza, tanto `A[i][k]` como `Bt[j][k]` se recorren de forma secuencial. Esto hace que la memoria caché funcione de maravilla. La CPU pide un dato y la memoria le entrega ese y los siguientes, que son justo los que va a necesitar. ¡El resultado fue una mejora de velocidad increíble!

### 3. ¿Cómo hiciste para usar toda la potencia de la CPU? (ejercicio3.c)

Después de optimizar la memoria, el siguiente paso fue usar el **paralelismo**. Mi computadora tiene varios núcleos, pero un programa normal solo usa uno. ¡Estábamos desperdiciando un montón de poder!

Usamos una herramienta llamada **OpenMP**. Con una sola línea de código, pudimos decirle al programa que dividiera el trabajo entre todos los núcleos disponibles.

```c
// ¡La línea mágica!
#pragma omp parallel for
for (int i = 0; i < n; i++) {
    // ... el resto del código de multiplicación ...
}
```

Esa línea `#pragma omp parallel for` le dice al compilador: "Oye, este bucle `for` (el que recorre las filas `i`), repártelo entre todos los hilos que tengas". Así, si tengo 8 núcleos, cada uno se encarga de una parte de las filas de la matriz. Es como si 8 personas se pusieran a resolver el problema al mismo tiempo en lugar de una sola.

## Resultados Finales

Para una matriz de 2000x2000, los resultados en mi computadora fueron espectaculares.

| Versión | Tiempo de Ejecución (N=2000) | Mejora |
| :-------------------------------------- | :--------------------------- | :----------------- |
| 1. Ingenua (`ejercicio1.c`)             | ~16.2 segundos               | 1x (Base)          |
| 2. Optimizada con Caché (`ejercicio2.c`)  | ~4.1 segundos                | ~4x más rápido     |
| 3. Paralela + Caché (`ejercicio3.c`)      | **~0.9 segundos**            | **~18x más rápido**|

*(Nota: Los tiempos pueden variar mucho dependiendo de la CPU)*

## ¿Cómo Compilar y Ejecutar?

Se esitas tener un compilador de C como `gcc`.

**Ejercicio 1 y 2 (Sin paralelismo):**
```sh
# Compilar (con optimización del compilador O3)
gcc -O3 ejercicio1.c -o ejercicio1
gcc -O3 ejercicio2.c -o ejercicio2

# Ejecutar con una matriz de 1500x1500
./ejercicio1 1500
./ejercicio2 1500
```

**Ejercicio 3 (Con OpenMP):**
Aquí necesitamos añadir `-fopenmp` para que el compilador entienda el paralelismo.
```sh
# Compilar
gcc -O3 -fopenmp ejercicio3.c -o ejercicio3

# Ejecutar con una matriz de 2000x2000
./ejercicio3 2000
```

## Conclusión

Este proyecto me enseñó que no solo es importante que el código funcione, sino **cómo** funciona por dentro. Entender cómo la CPU usa la memoria caché y cómo podemos usar todos sus núcleos puede transformar un programa lento en uno súper rápido. ¡Pasar de 16 segundos a menos de 1 segundo con solo unos pocos cambios fue increíble!
