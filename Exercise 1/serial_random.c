//serial random

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int r1, c1, r2, c2;
    int i, j, k;
    int **a, **b, **c;
    double time_ms;

    printf("Enter rows and columns of first matrix: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter rows and columns of second matrix: ");
    scanf("%d %d", &r2, &c2);

    if (c1 != r2)
    {
        printf("Matrix multiplication not possible\n");
        return 0;
    }

    a = (int **)malloc(r1 * sizeof(int *));
    b = (int **)malloc(r2 * sizeof(int *));
    c = (int **)malloc(r1 * sizeof(int *));
    for (i = 0; i < r1; i++)
    {
        a[i] = (int *)malloc(c1 * sizeof(int));
        c[i] = (int *)malloc(c2 * sizeof(int));
    }
    for (i = 0; i < r2; i++)
        b[i] = (int *)malloc(c2 * sizeof(int));

    for (i = 0; i < r1; i++)
        for (j = 0; j < c1; j++)
            a[i][j] = rand() % 10;
    for (i = 0; i < r2; i++)
        for (j = 0; j < c2; j++)
            b[i][j] = rand() % 10;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            c[i][j] = 0;
            for (k = 0; k < c1; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    time_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Time taken for matrix multiplication: %.6f milliseconds\n", time_ms);

    return 0;
}
