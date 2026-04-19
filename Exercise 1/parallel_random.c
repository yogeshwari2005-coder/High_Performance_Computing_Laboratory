//parallel random

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int r1, c1, r2, c2;
    int i, j, k;

    printf("Enter rows and columns of first matrix: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter rows and columns of second matrix: ");
    scanf("%d %d", &r2, &c2);

    if (c1 != r2)
    {
        printf("Matrix multiplication not possible\n");
        return 0;
    }

    int sizeA = r1 * c1;
    int sizeB = r2 * c2;
    int sizeC = r1 * c2;

    int shm_id_int = shmget(1089, (sizeA+sizeB+sizeC)*sizeof(int), IPC_CREAT|0777);
    int *data = (int *)shmat(shm_id_int, NULL, 0);

    int *A = data;
    int *B = data + sizeA;
    int *C = data + sizeA + sizeB;

    int shm_id_time = shmget(2089, r1*sizeof(double), IPC_CREAT|0777);
    double *row_time = (double *)shmat(shm_id_time, NULL, 0);

    for (i = 0; i < sizeA; i++)
       A[i] = rand() % 10;
    for (i = 0; i < sizeB; i++)
       B[i] = rand() % 10;

    for (i = 0; i < r1; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (j = 0; j < c2; j++)
            {
                C[i * c2 + j] = 0;
                for (k = 0; k < c1; k++)
                {
                    C[i*c2+j] += A[i*c1+k] * B[k*c2+j];
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            row_time[i] = (end.tv_sec-start.tv_sec)*1000.0 + (end.tv_nsec-start.tv_nsec)/1000000.0;
            exit(0);
        }
    }

    for (i = 0; i < r1; i++)
        wait(NULL);

    double max_time = row_time[0];
    for (i = 1; i < r1; i++)
    {
        if (row_time[i] > max_time)
            max_time = row_time[i];
    }

    printf("Time taken for parallel matrix multiplication: %.6f milliseconds\n", max_time);

    shmdt(data);
    shmdt(row_time);
    shmctl(shm_id_int, IPC_RMID, NULL);
    shmctl(shm_id_time, IPC_RMID, NULL);

    return 0;
}
