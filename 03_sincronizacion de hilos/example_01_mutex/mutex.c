/* output and strings */
#include <errno.h>
#include <stdio.h>
#include <string.h>
/* hilos */
#include <pthread.h>

static int count = 10;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *thread_routine_1(void *params)
{
    for(int i = 0; i < 500000; i++)
    {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
    }
}

void *thread_routine_2(void *params)
{
    for(int i = 0; i < 500000; i++)
    {
        pthread_mutex_lock(&mutex);
        count--;
        pthread_mutex_unlock(&mutex);
    }
}

void error_exit(char *strerror)
{
    perror(strerror);
}

int main(int argc, char *args[])
{
    /* create two threads */
    pthread_t thread_1, thread_2;

    if(0 != pthread_create(&thread_1, NULL, thread_routine_1, NULL))
    {
        error_exit("No se pudo crear el hilo: thread_1");
    }

    if(0 != pthread_create(&thread_2, NULL, thread_routine_2, NULL))
    {
        error_exit("No se pudo crear el hilo: thread_2");
    }

    /* join threads */
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("count value: %d\n", count);

    return 0;
}