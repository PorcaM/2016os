#include <stdlib.h> /* required for rand() */
#include "buffer.h"

int insert_item(buffer_item);
int remove_item(buffer_item *);

int count;

void *producer(void *param){
	//printf("producer thread is created!\n");
	buffer_item item;
	while(true){
		/* sleep for a random period of time */
		sleep(1+rand()%3);
		/* generate a random number */
		//pthread_mutex_lock(&mutex);
		item = rand()%10;
		sem_wait(&empty);
		sem_wait(&monitor);
		if(insert_item(item))
			fprintf(stdout, "report error condition\n");
		else
			printf("INSERT: %d\n", item);
		sem_post(&request);
		sem_post(&full);
		//pthread_mutex_unlock(&mutex);
	}
}

void *consumer(void *param){
	buffer_item item;
	//printf("consumer thread is created!\n");
	while(true){
		/* sleep for a random period of time */
		//pthread_mutex_lock(&mutex);
		sleep(1+rand()%3);
		sem_wait(&full);
		sem_wait(&monitor);
		if(remove_item(&item))
			fprintf(stdout, "report error condition\n");
		else
			printf("REMOVE: %d\n", item);
		sem_post(&request);
		sem_post(&empty);
		//pthread_mutex_lock(&mutex);
	}
}

void print_buffer();
void *monitoring(void *param){
	int ack_cnt = 0;
	while(true){
		sleep(1);
		sem_post(&monitor);
		sem_wait(&request);
		printf("ACK NO: %d -> couunt: %d\n", ack_cnt++, count);
		//print_buffer();
	}
}
