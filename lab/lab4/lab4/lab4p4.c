//
//  lab4p4.c
//  lab4
//
//  Created by DongShaocong on 12/3/18.
//  Copyright © 2018 nus.cs3217.a0148008. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int globP; //change the var name to avoid conflicting name in the current namespace
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * child(void *t)

{

    // Increment globP by 1, wait for 1 second, then increment by 1 again.
    printf("Child %d entering. globP is currently %d\n", t, globP);

    pthread_mutex_lock(&mutex);
    globP++;
    sleep(1);
    globP++;
    pthread_mutex_unlock(&mutex);
    printf("Child %d exiting. globP is currently %d\n", t, globP);
    pthread_exit(NULL);
}

int main()

{

    int i;

    globP=0;

    for(i=0; i<10; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, child, (void *) i);
        pthread_join(thread, NULL);
    }

    printf("Final value of globP is %d\n", globP);
    pthread_mutex_destroy(&mutex);

    return 0;

}
