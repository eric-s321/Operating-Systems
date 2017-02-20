#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/mman.h>
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

char * getExecutablePath(){

// Use this to avoid hardcoding the path so this will work on different machines 
    char *retcwd = (char *) malloc(sizeof(char) * 1024);
    char cwd[1024];
    int lastIndex = 0;
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        while(cwd[lastIndex] != '\0'){
            lastIndex++;
        }

        char *executable = "/prog3ipc";
        int i;
        for(i = 0; i < 9; i++){
            cwd[lastIndex + i] = executable[i];
        }
        cwd[lastIndex + 9] = '\0'; //Put NULL character after executable name
    }
    int i;
    for(i = 0; cwd[i] != '\0'; i++){
        retcwd[i] = cwd[i];
    }
    retcwd[lastIndex + 9] = '\0';

    return retcwd;
}

void parent(int unnamedPipe[], int numProcs, bool topLevelProcess, const char *name){

    numProcs -= 1;

    printf("I am the parent with PID: %d writing %d\n", getpid(), numProcs);

    //close the read end of pipe
    close(unnamedPipe[0]);

    //write message to child from write end
    //Doing this before fork so that we write before child tries to read
    if(write(unnamedPipe[1],&numProcs, sizeof(numProcs)) <= 0){
        fprintf(stderr, "Problem in process %d", getpid());
        perror("Problem writing to the pipe");
        exit(EXIT_FAILURE);
    }   
    close(unnamedPipe[1]);

    wait(NULL);

    //Top level process will remove the shared memory
    if(topLevelProcess)
        shm_unlink(name);

    printf("Parent exiting\n");
}

void child(int unnamedPipe[], int numProcs, int shmPtr[]){

    printf("I am the child\n");
    printf("Address of shmPtr is %p\n", shmPtr);

    //close the write end of pipe
    close(unnamedPipe[1]);

    // read message from parent through read end
    if(read(unnamedPipe[0],&numProcs,sizeof(numProcs)) <= 0){
        fprintf(stderr, "Problem in process %d", getpid());
        perror("Problem reading from pipe");
        exit(EXIT_FAILURE); 
    }
    close(unnamedPipe[0]);

    printf("Child read %d with PID: %d\n", numProcs, getpid());
    printf("Writing at index %d\n", numProcs - 1);

    //Write PID to array
    //Every process will be a child besides the top level process
    printf("Writing %d to index %d\n", getpid(),numProcs-1);
    void *writeLocation;
    writeLocation = shmPtr + (numProcs - 1); //Since shpPtr is an int array here the *sizeof(int) is implicit
    int myPid = getpid();
    memcpy(writeLocation, &myPid, sizeof(myPid));

    if(numProcs == 1){ //Leaf process
        
        printf("\n\n");
        printf("I am the leaf child. Printing the shared mem...\n");
        int i;
        for(i = 0; shmPtr[i] != -1; i++){
            printf("%d\n", shmPtr[i]);    
        } 
        printf("\n\n");
           
    }

    char strNumProcs[255];
    sprintf(strNumProcs, "%d", numProcs);

    int status = execlp(getExecutablePath(), "prog3ipc", strNumProcs, NULL);

    if(status == -1){
        perror("Execlp failed.");
        exit(EXIT_FAILURE);
    }

}

int main(int argc, char *argv[]){
    int unnamedPipe[2];
//    char buf[30];
    const char *name = "sharedMem";
    int SIZE;
    pid_t pid;

    if(argc != 2)
        printUsageAndExit();

    int numProcs = strtol(argv[1], NULL, 10);

    if(numProcs < 1 || numProcs > 32){
        fprintf(stderr, "num-procs must be between 1 and 32 inclusive.\n");
        exit(EXIT_FAILURE);
    }

    SIZE = numProcs;

    //Check if the shared memory has already been created
    bool shmExists = false;
    if(access("/dev/shm/sharedMem", F_OK) != -1)
        shmExists = true;


    //Set up shared memory array
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    //Only resize the memory if we are creating it (root process only)
    if(!shmExists){
        ftruncate(shm_fd,SIZE);
        printf("PID %d RESIZING THE ARRAY\n", getpid());
    }

    void *shmPtr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    void *writeLocation;

    printf("In main shmPtr address is %p\n", shmPtr);

    if(!shmExists){ //This is the top level process

        //Write -1 at the very end of array to mark the end of it
        printf("Writing -1 to index %d\n", numProcs);
        writeLocation = shmPtr + (numProcs) * sizeof(int); 
        int endOfArray = -1;
        memcpy(writeLocation, &endOfArray, sizeof(endOfArray));

        //Write pid in the last usable index of the array
        writeLocation = shmPtr + (numProcs - 1) * sizeof(int); 
        printf("Writing %d to index %d\n", getpid(),numProcs-1);
        int myPid = getpid();
        memcpy(writeLocation, &myPid, sizeof(myPid));
    }

    //Create unnamed pipe to communicate numProcs to child
    int pipeResult = pipe(unnamedPipe);

    if(pipeResult == -1){
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    if(numProcs - 1 >= 1){
        pid = fork();

        if(pid < 0){
            perror("Fork Failed");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0)//Child 
            child(unnamedPipe, numProcs, (int *)shmPtr);
        else //Parent
            parent(unnamedPipe, numProcs, !shmExists, name);
    }
    return 0;
}
