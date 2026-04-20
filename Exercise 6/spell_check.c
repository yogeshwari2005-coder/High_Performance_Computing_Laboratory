#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define THREADS 3
#define WORD_LEN 20

// Dictionary
char *dictionary[] = {
    "apple","banana","cat","dog","computer",
    "science","book","program","server","thread"
};

int dict_size = 10;

// Input words
char *input_words[] = {
    "apple","scie","dog","monkey","banana"
};

int total_words = 5;

// Work buffer
char work_buffer[BUFFER_SIZE][WORD_LEN];
int work_count = 0;
int next_word = 0;

// Result buffer
char result_buffer[BUFFER_SIZE][50];
int result_count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

// Dictionary check function
int check_dictionary(char word[])
{
    for(int i = 0; i < dict_size; i++)
    {
        if(strcmp(word, dictionary[i]) == 0)
            return 1;
    }
    return 0;
}

// Worker thread
void *spell_check(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(work_count == 0 && next_word < total_words)
            pthread_cond_wait(&cond, &mutex);

        if(work_count == 0 && next_word >= total_words)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        char word[WORD_LEN];
        strcpy(word, work_buffer[--work_count]);

        pthread_mutex_unlock(&mutex);

        int correct = check_dictionary(word);

        pthread_mutex_lock(&mutex);

        if(correct)
            sprintf(result_buffer[result_count++], "%s -> Correct", word);
        else
            sprintf(result_buffer[result_count++], "%s -> Incorrect", word);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main()
{
    pthread_t threads[THREADS];

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    // Create worker threads
    for(int i=0;i<THREADS;i++)
        pthread_create(&threads[i],NULL,spell_check,NULL);

    // Master thread fills buffer
    while(next_word < total_words)
    {
        pthread_mutex_lock(&mutex);

        strcpy(work_buffer[work_count++], input_words[next_word++]);

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    // Wake all threads so they can exit
    pthread_cond_broadcast(&cond);

    // Wait for threads
    for(int i=0;i<THREADS;i++)
        pthread_join(threads[i],NULL);

    printf("\nSpell Check Results:\n");

    for(int i=0;i<result_count;i++)
        printf("%s\n",result_buffer[i]);

    return 0;
}
