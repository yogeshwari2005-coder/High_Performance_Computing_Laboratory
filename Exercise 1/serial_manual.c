#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int get_Det(int **mat, int n)
{
    double det = 1;
    int i, j, k;
    for(i=0; i<n; i++)
    {
        if (mat[i][i] == 0)
           return 0;
        for(j=i+1; j<n; j++)
        {
            double factor = (double)mat[j][i] / mat[i][i];
            for(k=i; k<n; k++)
                mat[j][k] -= factor * mat[i][k];
        }
        det *= mat[i][i];
    }
    return (int)det;
}

int main()
{
    int m1, n1, m2, n2;
    int i, j, k;
    int sum;
    struct timespec start, end;

    printf("\nEnter matrixA size [m,n] : ");
    scanf("%d %d", &m1, &n1);
    printf("Enter matrixB size [m,n] : ");
    scanf("%d %d", &m2, &n2);

    if(!(m1 == m2 && n1 == n2))
    {
        printf("Matrix size invalid for addition/subtraction!\n");
        return 0;
    }
    if(n1 != m2)
    {
        printf("Matrix size invalid for multiplication!\n");
        return 0;
    }
    if(m1 != n1 || m2 != n2)
    {
        printf("Matrix size invalid for determinant!\n");
        return 0;
    }

    int **A = (int **)malloc(m1 * sizeof(int*));
    int **B = (int **)malloc(m2 * sizeof(int*));
    int **C = (int **)malloc(m1 * sizeof(int*));

    for(i=0; i<m1; i++)
    {
       A[i] = (int *)malloc(n1 * sizeof(int));
       C[i] = (int *)malloc(n2 * sizeof(int));
    }
    for(i=0; i<m2; i++)
       B[i] = (int *)malloc(n2 * sizeof(int));

    for(i=0; i<m1; i++)
        for(j=0; j<n1; j++)
           A[i][j] = rand() % 10;
    for(i=0; i<m2; i++)
        for(j=0; j<n2; j++)
           B[i][j] = rand() % 10;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for(i=0; i<m1; i++)
        for(j=0; j<n1; j++)
           C[i][j] = A[i][j] + B[i][j];

    for(i=0; i<m1; i++)
        for(j=0; j<n1; j++)
           C[i][j] = A[i][j] - B[i][j];

    for(i=0; i<m1; i++)
    {
        for(j=0; j<n2; j++)
        {
            sum = 0;
            for(k=0; k<n1; k++) sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }

    get_Det(A, m1);
    get_Det(B, m2);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec-start.tv_sec)*1000.0 + (end.tv_nsec-start.tv_nsec)/1000000.0;
    printf("Time taken for serial execution: %f ms\n\n", time_taken);

    for(i=0; i<m1; i++) { free(A[i]); free(C[i]); }
    for(i=0; i<m2; i++) { free(B[i]); }
    free(A); free(B); free(C);

    return 0;
}
