#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "barrier.h"

TBarrier barrier;

#define NUM_THREADS	10

void *threadCode(void *p)
{
	int tid = (int) p;
	
	// Delay a random number of seconds
	unsigned  delay = (unsigned) ((float)rand() / RAND_MAX * 5.0);
	printf("Thread %d delaying %u seconds\n", tid, delay);
	sleep(delay);

	printf("Thread %d calling barrier\n", tid);
	reachBarrier(&barrier, tid);
	printf("Thread %d crossed barrier\n", tid);
	pthread_exit(NULL);

}

int main()
{
	pthread_t _tid[NUM_THREADS];
	void *val;

	srand(time(NULL));
	initBarrier(&barrier, NUM_THREADS);
	
	int i;
	for(i=0; i<10; i++)
	{
		pthread_create(&_tid[i], NULL, threadCode, (void *) i);
	}

	for(i=0; i<10; i++)
		pthread_join(_tid[i], &val);

	printf("\n\nMAIN EXITING\n\n");
}
