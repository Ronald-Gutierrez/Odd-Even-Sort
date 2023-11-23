#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int fase, i;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void generarArrayAleatorio(int a[], int b[], int n) {
    srand(time(NULL));  
    for (int i = 0; i < n; ++i) {
        int numeroAleatorio = rand() % 100000; 
        a[i] = numeroAleatorio;
        b[i] = numeroAleatorio;
    }
}

void imprimirArray(const int a[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void oddEvenSortParallelFor(int a[], int n, int num_hilos) {
    #pragma omp parallel for num_threads(num_hilos) shared(a, n) private(i)
    for (fase = 0; fase < n; fase++) {
        if (fase % 2 == 0) {
            #pragma omp parallel for num_threads(num_hilos) shared(a, n) private(i)
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    swap(&a[i-1], &a[i]);
                }
            }
        } else {
            #pragma omp parallel for num_threads(num_hilos) shared(a, n) private(i)
            for (i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    swap(&a[i], &a[i+1]);
                }
            }
        }
    }
}

void oddEvenSortImproved(int a[], int n, int num_hilos) {
    #pragma omp parallel num_threads(num_hilos) shared(a, n) private(i)
    for (fase = 0; fase < n; fase++) {
        if (fase % 2 == 0) {
            #pragma omp for
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    swap(&a[i-1], &a[i]);
                }
            }
        } else {
            #pragma omp for
            for (i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    swap(&a[i], &a[i+1]);
                }
            }
        }
    }
}

int main() {
    const int n = 20000;
    int arreglo1[n], arreglo2[n];
    generarArrayAleatorio(arreglo1, arreglo2, n);
    //printf("Array Original: ");
    //imprimirArray(arreglo1, n);

    //primera implementación
    int num_hilos = 2; 
    double tiempo_inicio = omp_get_wtime();
    oddEvenSortParallelFor(arreglo1, n, num_hilos);
    double tiempo_fin = omp_get_wtime();
    
    printf("Array Ordenado (Parallel For): ");
    //imprimirArray(arreglo1, n);
    printf("Tiempo tomado: %f segundos\n", tiempo_fin - tiempo_inicio);


    //printf("\nArray Original: ");
    //imprimirArray(arreglo2, n);

    // Medir tiempo 
    tiempo_inicio = omp_get_wtime();
    oddEvenSortImproved(arreglo2, n, num_hilos);
    tiempo_fin = omp_get_wtime();
    
    printf("Array Ordenado (Improved): ");
    //imprimirArray(arreglo2, n);
    printf("Tiempo tomado: %f segundos\n", tiempo_fin - tiempo_inicio);

    return 0;
}