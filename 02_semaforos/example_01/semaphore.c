/* output and strings */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

static int count = 10; 
sem_t semaphore;

void *thread_routine_1(void *params);
void *thread_routine_2(void *params);

int main(int argc, char *args[])
{
    pthread_t thread_1, thread_2;
    
    sem_init(&semaphore, 0, 1);

    pthread_create(&thread_1, NULL, thread_routine_1, NULL);
    pthread_create(&thread_2, NULL, thread_routine_2, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("count value: %d\n", count);

    return 0;
}

void *thread_routine_1(void *params)
{
    for(int i = 0; i < 65000; i++)
    {
        sem_wait(&semaphore);
        count++;
        sem_post(&semaphore);
    }
}

void *thread_routine_2(void *params)
{
    for(int i = 0; i < 65000; i++)
    {
        sem_wait(&semaphore);
        count--;
        sem_post(&semaphore);
    }   
}