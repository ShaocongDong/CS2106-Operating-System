#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "buffer.h"

void initBuffer(TBuffer *buffer)
{
	buffer->count=0;
	buffer->front=0;
	buffer->back=0;
    buffer->bmutex=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    sem_init(&buffer->full, 0, 0);
    sem_init(&buffer->empty, 0, QLEN);
}

void enq(TBuffer *buffer, const char *data, int len)
{

    sem_wait(&buffer->empty);
    pthread_mutex_lock(&buffer->bmutex);

	unsigned int bytesToCopy = (len < ENTRY_SIZE ? len : ENTRY_SIZE);

	memcpy(buffer->data[buffer->back], data, bytesToCopy);
	buffer->len[buffer->back] = bytesToCopy;
	buffer->count++;
	buffer->back = (buffer->back + 1) % QLEN;
    sem_post(&buffer->full);
    pthread_mutex_unlock(&buffer->bmutex);
}

int deq(TBuffer *buffer, char *data)
{

    sem_wait(&buffer->full);
    pthread_mutex_lock(&buffer->bmutex);

	int len = buffer->len[buffer->front];
	memcpy(data, buffer->data[buffer->front], len);
	buffer->count--;
	buffer->front = (buffer->front + 1) % QLEN;
    sem_post(&buffer->empty);
    pthread_mutex_unlock(&buffer->bmutex);
	return len;
}
