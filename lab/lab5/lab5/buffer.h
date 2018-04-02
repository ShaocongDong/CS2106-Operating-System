#ifndef __Q_HEADER__
#define __Q_HEADER__

#include <pthread.h>
#include <semaphore.h>

//# of entries in the queue
#define QLEN		10

// Length of each queue entry in bytes
#define ENTRY_SIZE	64

typedef struct
{
	char data[QLEN][ENTRY_SIZE];
	int len[QLEN];
	int front, back;
	int count;
    pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
    sem_t empty, full;
} TBuffer;

void initBuffer(TBuffer *buffer);
void enq(TBuffer *buffer, const char *data, int len);
int deq(TBuffer *buffer, char *data);
#endif

