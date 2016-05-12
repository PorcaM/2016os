#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

typedef int buffer_item;
#define BUFFER_SIZE 10
#define true		1

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
sem_t monitor;
sem_t request;
