#ifndef __BARRIER_H__
#define __BARRIER_H__

#include <semaphore.h>

typedef struct
{
	int numProcesses;
	int numReached;
	sem_t *semArray;
} TBarrier;

void initBarrier(TBarrier *barrier, int numProcesses);
void resetBarrier(TBarrier *barrier);
void reachBarrier(TBarrier *barrier, int procNum);

#endif
