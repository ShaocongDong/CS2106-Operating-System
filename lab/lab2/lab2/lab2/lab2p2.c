//
//  lab2p2.c
//  lab2
//
//  Created by DongShaocong on 12/2/18.
//  Copyright © 2018 nus.cs3217.a0148008. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Maximum size of our comms buffer
#define MAX_BUFFER_LEN          1024
int main() {
    int fd[2];
    pipe(fd);

    char buffer[1024];

    int n;

    if (fork() == 0) {
        close(fd[1]);
        n = read(fd[0], buffer, MAX_BUFFER_LEN);
        printf("Child read %d bytes from parent: %s\n", n, buffer);
        close(fd[0]);
    } else {
        int status;
        close(fd[0]);
        sprintf(buffer, "Hello child! This is your parent!");
        n = write(fd[1], buffer, strlen(buffer)+1);
        printf("parent wrote %d bytes to the child: %s\n", n, buffer);
        close(fd[1]);

        wait(&status);
    }
}
