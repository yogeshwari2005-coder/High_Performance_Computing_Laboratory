#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **A, **B, **Sum, **Prod;
int N;
int i,j,k;
// Thread function for Matrix Addition
void* add_matrices(void* arg) {
    for ( i = 0; i < N; i++)
        for ( j = 0; j < N; j++)
            Sum[i][j] = A[i][j] + B[i][j];
    pthread_exit(NULL);
}

// Thread function for Matrix Multiplication
void* multiply_matrices(void* arg) {
    for ( i = 0; i < N; i++) {
        for ( j = 0; j < N; j++) {
            Prod[i][j] = 0;
            for ( k = 0; k < N; k++)
                Prod[i][j] += A[i][k] * B[k][j];
        }
    }
    pthread_exit(NULL);
}

int** allocate_matrix(int size) {
    int** matrix = malloc(size * sizeof(int*));
    for ( i = 0; i < size; i++)
        matrix[i] = malloc(size * sizeof(int));
    return matrix;
}

int main() {
    pthread_t thread1, thread2;

    printf("\n***** MATRIX ADDITION AND MULTIPLICATION -TASK PARALLELISM *****\n");
    printf("Enter matrix size (N for NxN): ");
    scanf("%d", &N);

    A = allocate_matrix(N);
    B = allocate_matrix(N);
    Sum = allocate_matrix(N);
    Prod = allocate_matrix(N);

    printf("Enter elements for Matrix A:\n");
    for ( i = 0; i < N; i++)
        for ( j = 0; j < N; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements for Matrix B:\n");
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            scanf("%d", &B[i][j]);

    pthread_create(&thread1, NULL, add_matrices, NULL);
    pthread_create(&thread2, NULL, multiply_matrices, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nResult of Addition:\n");
    for (i = 0; i < N; i++) {
        for ( j = 0; j < N; j++)
           printf("%d ", Sum[i][j]);
        printf("\n");
    }

    printf("\nResult of Multiplication:\n");
    for ( i = 0; i < N; i++) {
        for ( j = 0; j < N; j++)
           printf("%d ", Prod[i][j]);
        printf("\n");
    }

    // Cleanup memory
    for ( i = 0; i < N; i++) {
        free(A[i]); free(B[i]); free(Sum[i]); free(Prod[i]);
    }
    free(A); free(B); free(Sum); free(Prod);

    return 0;
}
