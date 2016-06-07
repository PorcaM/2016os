#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define THREAD_SIZE 3

int *arr, *sorted, flag = 0, c;
sem_t mutex;

typedef struct{
int from_index;
int to_index;
} parameters;

void *b_sort(void *);
void *merge(void *);

int main(int argc, char *argv[]){
FILE *fp;
int i;
parameters *param = (parameters *)malloc(3*sizeof(parameters));
pthread_t tid[THREAD_SIZE + 1];
pthread_attr_t attr;

if (argc != 2){
	fprintf(stderr, "useage: sorting <file name>\n");
	return -1;
}

fp = fopen(argv[1], "r");
c = 0;
while (fscanf(fp, "%d", &i) != EOF) c++;
fp = fopen(argv[1], "r");
arr = (int *)malloc(c*sizeof(int));
i = 0;
while (fscanf(fp, "%d", &arr[i++]) != EOF);

pthread_attr_init(&attr);
for (i = 0; i < THREAD_SIZE; i++) {
	param[i].from_index = i * c / THREAD_SIZE;
	param[i].to_index = ((i+1) * c) / THREAD_SIZE;
}

sem_init(&mutex, 1, 1);
for(i = 0; i < THREAD_SIZE; i++)
	pthread_create(&tid[i], &attr, b_sort, &param[i]);
pthread_create(&tid[THREAD_SIZE], &attr, merge, NULL);
pthread_join(tid[THREAD_SIZE], NULL);

fp = fopen("output.txt", "w");
for (i = 0; i < c ; i++)
	fprintf(fp, "%d ", sorted[i]);
fprintf(fp, "\n");

return 0;
}

void *b_sort(void *_param){
parameters *param = (parameters *)_param;
int b = param->from_index, e = param->to_index;
int i, j, temp;

	for (i = b; i < e; i++) {
		for (j = b; j < e-1; j++) {
			if (arr[j] > arr[j+1]){
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}

	sem_wait(&mutex);
	flag++;
	sem_post(&mutex);
	pthread_exit(0);
}

void *merge(void *p){
int i, j, turn, min;
int it[THREAD_SIZE], e[THREAD_SIZE];
for (i = 0; i < THREAD_SIZE; i++) {
	it[i] = i * c / THREAD_SIZE;
	e[i] = ((i+1) * c) / THREAD_SIZE;
}
while (flag < 3); 		// SPIN LOCK
//printf("MERGE\n");
sorted = (int *)malloc(c * sizeof(int));

for (i = 0; i < c; i++) {
	min = 0x7FFFFFFF;
	for(j = 0; j < THREAD_SIZE; j++) {
//		printf("j: %d it: %d e: %d v: %d\n", j, it[j], e[j],  arr[it[j]]);
		if(it[j] < e[j] && arr[it[j]] <  min){
			min = arr[it[j]];
			turn = j;
		}
	}
	sorted[i] = min;
	it[turn]++;
//	printf("%d from %d\n", min, turn);
}

pthread_exit(0);
}
