#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int count = 0;
int nthread = 1;
int worker_loop_cnt = 1;
pthread_mutex_t lock;
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
	pthread_mutex_init(&lock, NULL);
    printf("main: begin (count = %d)\n", count);
    for (i=0; i < nthread; i++)
        assert(pthread_create(&th[i], NULL, worker, (void*) i) == 0);

    for(i=0; i < nthread; i++)
        assert(pthread_join(th[i], &value) == 0);
    printf("main: done (count = %d)\n", count);
    return 0;
}

static void *worker(void *num)
{
    int number = (int)num;

	pthread_mutex_lock(&lock);
    for (int i=0; i < worker_loop_cnt; i++)
        count++;
    printf("Thread number %d: %d \n", number, count);
    pthread_mutex_unlock(&lock);
	return NULL;

}

