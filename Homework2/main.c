#include <stdio.h>
#include <unistd.h>
#include "buffer.h"

void *producer(void *);
void *consumer(void *);
void *monitoring(void *);

int main(int argc, char *argv[]){
	int	i;
	pthread_t		tid;
	pthread_attr_t	attr;
	pthread_attr_init(&attr);
	/* 1. Get command line arguments argv[1], argv[2], argv[3] */
	/* 2. Initialize buffer */
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	sem_init(&monitor, 0, 0);
	sem_init(&request, 0, 0);
	pthread_create(&tid, &attr, monitoring, NULL);
	/* 3. Create producer thread(s) */
	for (i = 0; i < atoi(argv[2]); i++)
		pthread_create(&tid, &attr, producer, NULL);
	/* 4. Create consumer thread(s) */
	for (i = 0; i < atoi(argv[3]); i++)
		pthread_create(&tid, &attr, consumer, NULL);
	/* 5. Sleep */
	sleep(atoi(argv[1]));
	/* 6. Exit */
	return 0;
}
