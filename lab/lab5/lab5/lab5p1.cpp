#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "buffer.h"

#define NUM_THREADS			32

TBuffer buffer;

void *serialSendThread(void *p)
{
	// This thread simulates sending a character over a 115200 bps serial link, e.g.
	// e.g. over a 433 MHz radio to a remote system. 115200 bps transmission
	// transmits one character every 70 microseconds.

	while(1)
	{
		char data[ENTRY_SIZE];
		int len = deq(&buffer, data);

		if(len >= 0)
		{
			int i;

			for(i=0; i<len; i++)
			{
				// This printf simulates a send over a 115200 bps data link
				printf("%c", data[i]);

				// Sleep 70 microseconds for data to be sent out by the serial link
				usleep(70);
			}

		}
	}
}

void *senderThreads(void *p)
{
	int threadNum = (long) p;
	int count=0;
	while(1)
	{
		char data[ENTRY_SIZE];
		sprintf(data, "Thread %d entry %d\n", threadNum, count);
		enq(&buffer, data, strlen(data)+1);
		count++;
	}
}

int main()
{
	pthread_t _thr[NUM_THREADS];
	pthread_t _serialSend;
	initBuffer(&buffer);

	int i;

	pthread_create(&_serialSend, NULL, serialSendThread, NULL);
	pthread_detach(_serialSend);

	for(i=0; i<NUM_THREADS;i++)
	{
		pthread_create(&_thr[i], NULL, senderThreads, (void *) i);
		pthread_detach(_thr[i]);
	}

	// Infinite loop to prevent parent from exiting
	while(1);
}

