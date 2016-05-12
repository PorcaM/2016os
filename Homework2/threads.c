#include <stdlib.h> /* required for rand() */
#include "buffer.h"

int insert_item(buffer_item);
int remove_item(buffer_item *);

void *producer(void *param){
	printf("producer thread is created!\n");
	buffer_item item;
	while(true){
		/* sleep for a random period of time */
		sleep(1+rand()%3);
		/* generate a random number */
		//pthread_mutex_lock(&mutex);
		item = rand()%10;
		sem_wait(&empty);
		if(insert_item(item))
			fprintf(stdout, "report error condition\n");
		else
			printf("INSERT: %d\n", item);
		sem_post(&full);
		//pthread_mutex_unlock(&mutex);
	}
}

void *consumer(void *param){
	buffer_item item;
	printf("consumer thread is created!\n");
	while(true){
		/* sleep for a random period of time */
		//pthread_mutex_lock(&mutex);
		sleep(2+rand()%3);
		sem_wait(&full);
		if(remove_item(&item))
			fprintf(stdout, "report error condition\n");
		else
			printf("REMOVE: %d\n", item);
		sem_post(&empty);
		//pthread_mutex_lock(&mutex);
	}
}

