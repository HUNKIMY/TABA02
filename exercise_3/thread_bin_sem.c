#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

int count = 0;
int nthread = 1;
int worker_loop_cnt = 1;
sem_t semaphore;
static void *worker(void *num);

int main(int argc, char *argv[]){
	pthread_t *th;
	void *value;
	long i;

	if (argc < 3){
		fprintf(stderr, "%s parameter : nthread, worker_loop_cnt\n", argv[0]);
		exit(-1);
	}

	nthread = atoi(argv[1]);
	worker_loop_cnt = atoi(argv[2]);
	th = malloc(sizeof(pthread_t) * nthread);
	
	sem_init(&semaphore, 0, 1);
	printf("main: begin (count = %d)\n", count);
	for (i=0; i < nthread; i++)
		assert(pthread_create(&th[i], NULL, worker, (void*) i) == 0);

	for(i=0; i < nthread; i++)
		assert(pthread_join(th[i], &value) == 0);
	sem_destroy(&semaphore);
	printf("main: done (count = %d)\n", count);
	return 0;
}

static void *worker(void *num)
{
	int number = (int)num;
	
	sem_wait(&semaphore);

	for (int i=0; i < worker_loop_cnt; i++)
		count++;
	printf("Thread number %d: %d \n", number, count);
	
	sem_post(&semaphore);

	return NULL;

}
