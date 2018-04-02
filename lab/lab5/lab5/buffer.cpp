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
}

void enq(TBuffer *buffer, const char *data, int len)
{

	if(buffer->count >= QLEN)
		return;

    pthread_mutex_lock(&buffer->mutex);

	unsigned int bytesToCopy = (len < ENTRY_SIZE ? len : ENTRY_SIZE);

	memcpy(buffer->data[buffer->back], data, bytesToCopy);
	buffer->len[buffer->back] = bytesToCopy;
	buffer->count++;
	buffer->back = (buffer->back + 1) % QLEN;
    pthread_mutex_unlock(&buffer->mutex);
}

int deq(TBuffer *buffer, char *data)
{

	if(buffer->count == 0)
		return -1;

    pthread_mutex_lock(&buffer->mutex);

	int len = buffer->len[buffer->front];
	memcpy(data, buffer->data[buffer->front], len);
	buffer->count--;
	buffer->front = (buffer->front + 1) % QLEN;
    pthread_mutex_unlock(&buffer->mutex);
	return len;
}
