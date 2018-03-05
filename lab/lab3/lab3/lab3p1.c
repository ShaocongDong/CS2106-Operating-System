//
//  lab3p1.c
//  lab3
//
//  Created by DongShaocong on 5/3/18.
//  Copyright © 2018 nus.cs3217.a0148008. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUMELTS             16384
#define PARENTNUMSINCE      0
#define PARENTNUMTO         8192
#define BUFFERLENGTH        20

// IMPORTANT: Compile using "gcc lab3p1.c .lm -o lab3p1".

// The "-lm" is important as it brings in the Math library.

// Implements the naive primality test.

// Returns TRUE if n is a prime number

int prime(int n)

{

    int ret=1, i;

    for(i=2; i<=(int) sqrt(n) && ret; i++)

        ret=n % i;

    return ret;

}

int checkPrime(int start, int end, int* array) {
    int sum = 0;
    for (int i = start; i < end; i++) {
        if (prime(array[i])) {
            sum ++;
        }
    }
    return sum;
}

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

int main()

{

    int data[NUMELTS];
    // Create the random number list.
    srand(time(NULL));

    for(int i=0; i<NUMELTS; i++)
        data[i]=(int) (((double) rand() / (double) RAND_MAX) * 10000);

    // pipe a file discriptor array for inter process communication
    int fd[2];
    pipe(fd);

    // Now create a parent and child process.
    if (fork() != 0) { //Inside the PARENT process
        // Check the 0 to 8191 sub-list
        int parentCount = checkPrime(PARENTNUMSINCE, PARENTNUMTO, data);

        int status;
        // Then wait for the prime number count from the child.
        wait(&status);

        close(fd[1]);
        char buffer[BUFFERLENGTH];
        read(fd[0], buffer, BUFFERLENGTH);
        close(fd[0]);

        // Parent print the statistics
        int childCount = atoi(buffer);
        printf("Parent Prime number Count: %d \n", parentCount);
        printf("Child Prime number Count: %d \n", childCount);
        printf("Total Prime number Count: %d \n", parentCount + childCount);

    } else { // Inside the CHILD process

        // Check the 8192 to 16383 sub-list.
        int childCount = checkPrime(PARENTNUMTO, NUMELTS, data);
        // Send # of primes found to the parent.
        char buffer[BUFFERLENGTH];
        close(fd[0]);
        sprintf(buffer, "%s", itoa(childCount, 10));
        write(fd[1], buffer, strlen(buffer)+1);
        close(fd[1]);
    }

}
