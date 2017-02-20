#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "prog3ipc.h"


void printUsageAndExit(){
    fprintf(stderr, "Usage: ./prog3ipc <num-procs>\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int unnamedPipe[2];
    char buf[30];
    pid_t pid;

    if(argc != 2)
        printUsageAndExit();

    int numProcs = strtol(argv[1], NULL, 10);

    if(numProcs < 1 || numProcs > 32){
        fprintf(stderr, "num-procs must be between 1 and 32 inclusive.\n");
        exit(EXIT_FAILURE);
    }

    int pipeResult = pipe(unnamedPipe);
    if(pipeResult == -1){
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid < 0){
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
        printf("I am the child\n");
        //close the write end of pipe
        close(unnamedPipe[1]);
        // read message from parent through read end
        if(read(unnamedPipe[0],buf,6) <= 0){
            perror("Problem reading from pipe");
            exit(EXIT_FAILURE); 
        }

        printf("Child read %s\n", buf);
    }
    else{
        printf("I am the parent\n");

        //close the read end of pipe
        close(unnamedPipe[0]);

        //write message to child from write end
        sprintf(buf, "%d", numProcs - 1);
        if(write(unnamedPipe[1],buf, 6) <= 0){
            perror("Problem writing to the pipe");
            exit(EXIT_FAILURE);
        }   

        printf("Parent exiting\n");
    }

    
    
    






//    printf("Num procs is %d\n", numProcs);





}
