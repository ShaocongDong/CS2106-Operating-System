#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "barrier.h"

void initBarrier(TBarrier *barrier, int numProcesses)
{
	barrier->semArray = (sem_t *) calloc(numProcesses, sizeof(sem_t));

	int i;

	for(i=0; i<numProcesses; i++)
		sem_init(&barrier->semArray[i], 0, 0);

	barrier->numProcesses=numProcesses;
	barrier->numReached=0;
}

void resetBarrier(TBarrier *barrier)
{
	barrier->numReached=0;
}

void reachBarrier(TBarrier *barrier, int procNum)
{
	barrier->numReached++;

	if(barrier->numReached == barrier->numProcesses)
	{
		sem_post(&barrier->semArray[barrier->numProcesses-1]);
	}
	else
		sem_wait(&barrier->semArray[procNum]);

	if(procNum > 0)
		sem_post(&barrier->semArray[procNum-1]);
}
