#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <omp.h>

#define QUEUE_SIZE 10
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 2
#define TOTAL_ITEMS 100

int data[QUEUE_SIZE];
int front = 0, rear = 0, count = 0;
int produced_count = 0;
int global_sum = 0.0;
int total_received = 0;
omp_lock_t lock;

void enqueue(int value) {
    while (1) {
        omp_set_lock(&lock);
        if (count < QUEUE_SIZE) {
            data[rear] = value;
            rear = (rear + 1) % QUEUE_SIZE;
            count++;
            omp_unset_lock(&lock);
            break;
        }
        omp_unset_lock(&lock);
    }
}
int dequeue() {
    int value = -2;
    omp_set_lock(&lock);
    if (count > 0) {
        value = data[front];
        front = (front + 1) % QUEUE_SIZE;
        count--;
    } else if (produced_count >= TOTAL_ITEMS) {
        value = -1;
    }
    omp_unset_lock(&lock);
    return value;
}
void producer() {
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        enqueue(i);
        #pragma omp atomic
        produced_count++;
    }
}
void consumer() {
    double local_sum = 0.0;
    int local_count = 0;
    while (1) {
        int value = dequeue();
        if (value == -1) break;
        if (value == -2) continue;
        local_sum += value;
        local_count++;
    }
    #pragma omp critical
    {
        global_sum += local_sum;
        total_received += local_count;
    }
}
int main() {
    omp_init_lock(&lock);


    #pragma omp parallel num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        if (omp_get_thread_num() < NUM_PRODUCERS) {
            producer();
        } else {
            consumer();
        }
    }
    double mean = global_sum / total_received;
    printf("Global Mean: %f\n", mean);

    double variance_sum = 0.0;
    #pragma omp parallel for reduction(+:variance_sum)
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        variance_sum += (i - mean) * (i - mean);
    }
    printf("Global Variance: %f\n", variance_sum / TOTAL_ITEMS);
    omp_destroy_lock(&lock);
    return 0;
}
