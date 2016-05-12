#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
int count = 0, in = 0, out = 0;

void print_buffer(){
	int i;
	for (i = 0; i < BUFFER_SIZE; i++){
		printf("%d ", buffer[i]);
	}
	printf("\n");
}

int insert_item(buffer_item item){
	/* insert item into buffer
	   return 0 if successful, otherwise
	   return -1 indicating an error condition */
	pthread_mutex_lock(&mutex);
	//print_buffer();
	buffer[in] = item;
	in = (in+1)%BUFFER_SIZE;
	count++;
	//printf("p count: %d\n", count);
	pthread_mutex_unlock(&mutex);
	if (count > BUFFER_SIZE) return -1;
	return 0;
}
int remove_item(buffer_item *item){
	/* remove an object from buffer placing it in item
	   return 0 if successful, otherwise
	   return -1 indicating an error condition */
	//print_buffer();
	pthread_mutex_lock(&mutex);
	*item = buffer[out];
	out = (out+1)%BUFFER_SIZE;
	count--;
	//printf("c count: %d\n", count);
	pthread_mutex_unlock(&mutex);
	if (count < 0) return -1;
	return 0;
}
