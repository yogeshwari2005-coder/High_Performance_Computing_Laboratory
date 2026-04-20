#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_WORKERS 4

typedef struct {
    int num1;
    int num2;
    char op;
} task;

// Separate buffers for each operation: 0:+, 1:-, 2:*, 3:/
task buffers[NUM_WORKERS][BUFFER_SIZE];
int ins[NUM_WORKERS] = {0, 0, 0, 0};
int outs[NUM_WORKERS] = {0, 0, 0, 0};

sem_t empty[NUM_WORKERS];
sem_t full[NUM_WORKERS];
pthread_mutex_t mutexes[NUM_WORKERS];
pthread_mutex_t file_mutex;

FILE *fp;
int TOTAL_TASKS;

void* worker(void *arg) {
    int id = *(int*)arg;
    char ops[] = {'+', '-', '*', '/'};
    char operation = ops[id];
    pthread_t tid = pthread_self(); // Get Thread ID

    while(1) {
        sem_wait(&full[id]);
        pthread_mutex_lock(&mutexes[id]);

        task t = buffers[id][outs[id]];
        outs[id] = (outs[id] + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutexes[id]);
        sem_post(&empty[id]);

        if(t.op == 'X') break;

        float result;
        if (operation == '+') result = t.num1 + t.num2;
        else if (operation == '-') result = t.num1 - t.num2;
        else if (operation == '*') result = t.num1 * t.num2;
        else if (operation == '/') result = (t.num2 != 0) ? (float)t.num1 / t.num2 : 0;

        // Print Thread ID and Result to file
        pthread_mutex_lock(&file_mutex);
        fprintf(fp, "[TID: %lu] %d %c %d = %.2f\n", (unsigned long)tid, t.num1, operation, t.num2, result);
        pthread_mutex_unlock(&file_mutex);
    }
    return NULL;
}

void* server(void *arg) {
    char ops[] = {'+', '-', '*', '/'};
    srand(time(NULL));

    for(int i = 0; i < TOTAL_TASKS; i++) {
        task t;
        t.num1 = rand() % 100;
        t.num2 = rand() % 50 + 1;
        int op_idx = rand() % 4;
        t.op = ops[op_idx];

        sem_wait(&empty[op_idx]);
        pthread_mutex_lock(&mutexes[op_idx]);

        buffers[op_idx][ins[op_idx]] = t;
        ins[op_idx] = (ins[op_idx] + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutexes[op_idx]);
        sem_post(&full[op_idx]);
    }

    // Send exit signal 'X' to all 4 queues
    for(int i = 0; i < 4; i++) {
        sem_wait(&empty[i]);
        task exit_task = {0, 0, 'X'};
        buffers[i][ins[i]] = exit_task;
        ins[i] = (ins[i] + 1) % BUFFER_SIZE;
        sem_post(&full[i]);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <tasks>\n", argv[0]);
        return 1;
    }
    TOTAL_TASKS = atoi(argv[1]);
    fp = fopen("result.txt", "w");

    pthread_t server_thread, workers[4];
    int worker_ids[4] = {0, 1, 2, 3};
    pthread_mutex_init(&file_mutex, NULL);

    for(int i = 0; i < 4; i++) {
        sem_init(&empty[i], 0, BUFFER_SIZE);
        sem_init(&full[i], 0, 0);
        pthread_mutex_init(&mutexes[i], NULL);
        pthread_create(&workers[i], NULL, worker, &worker_ids[i]);
    }

    pthread_create(&server_thread, NULL, server, NULL);
    pthread_join(server_thread, NULL);

    for(int i = 0; i < 4; i++) pthread_join(workers[i], NULL);

    fclose(fp);
    printf("Results with Thread IDs written to result.txt\n");
    return 0;
}
