#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char buffer[128];
int len;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define NUM_THREADS			32

void *writeSerialThread(void *p)
{
	// This thread simulates the behavior of a thread
	// communicating with another computer over a 115200bps
	// serial communications link.
	//
	// 115200 bits/sec = 14,400 bytes/s
	// There is therefore a delay of 70 microseconds between
	// each byte simulated with a 70 microsecond sleep.

	while(1)
	{
		if(len > 0)
		{
			int i;
			for(i=0; i<len; i++)
			{
				printf("%c", buffer[i]);
				usleep(70);
			}
			len=0;
			pthread_mutex_unlock(&mutex);
		}
	}

}

void *threadCode(void *p)
{
	int num = (int) p;
	int count=0;

	while(1)
	{
		pthread_mutex_lock(&mutex);
		sprintf(buffer, "This is thread %d iteration %d\n", num, ++count);
		len = strlen(buffer);
	}
}

int main()
{
	int i;
	pthread_t th_desc[NUM_THREADS];
	pthread_t th_serial;

	// Create listener thread
	len=0;

	pthread_create(&th_serial, NULL, writeSerialThread, NULL);
	pthread_detach(th_serial);

	for(i=0; i<NUM_THREADS; i++)
	{
		pthread_create(&th_desc[i], NULL, threadCode, (void *) i);
		pthread_detach(th_desc[i]);
	}

	// Infinite loop so we don't exit and orphan all the threads
	while(1);
}
