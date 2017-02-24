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
#include <signal.h>
#include "prog3ipc.h"

#define PROCESS_COMPLETE -999

int LEVEL;
//int ROOT_PID;
int *globalShmPtr;

void printUsageAndExit(){
    fprintf(stderr, "Usage: ./prog3ipc <num-procs>\n");
    exit(EXIT_FAILURE);
}

void signalHandler(int signo){
    fprintf(stdout,"EXITING: Level %d process with pid=%d, child of ppid=%d.\n",
            LEVEL, getpid(), getppid());
//    printf("Setting address %p to %d\n", globalShmPtr, PROCESS_COMPLETE);
    *globalShmPtr = PROCESS_COMPLETE;
    //kill(ROOT_PID, SIGCONT);
    exit(0);
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

void parent(int unnamedPipe[], int numProcs, bool topLevelProcess, const char *name, 
        int shmPtr[]){

    LEVEL = numProcs - 1;
    if(signal(SIGUSR1, signalHandler) == SIG_ERR){
        fprintf(stderr,"An error occurred while setting a signal handler.\n");
        exit(EXIT_FAILURE);
    }

    globalShmPtr = shmPtr;
    while(*globalShmPtr != getpid())
        globalShmPtr++;
    
    
    while(*shmPtr != -1){
        shmPtr++;
    }
    shmPtr--; //pointer to last element in array (root process ID)
    //ROOT_PID = *shmPtr;
    

    numProcs -= 1;
     
    //printf("I am the parent with PID: %d writing %d to unnamed pipe\n", getpid(), numProcs);
     
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


    int fd;
    char * myfifo = "/tmp/erics_fifo";

    fd = open(myfifo, O_RDONLY);

    int fifoInfo;

    if(read(fd, &fifoInfo, sizeof(int)) <=0){
        printf("Read failed on process %d\n", getpid());
        perror("Read failed");
    }

    if(fifoInfo != getpid()){ //Not the root process
        close(fd);
        //pause();
        wait(NULL);
    }
    else{//Root process
      //  printf("Process %d is the root process\n", getpid());
      /*
        while(*shmPtr != -1){
            shmPtr++;
        }
        shmPtr--; //pointer to last element in array (root process ID)
        */
        int *sharedArrayEnd = shmPtr;
        
        int i;
        for(i = 0; i < numProcs + 1;i++){  // +1 because we already sutracted from numProcs to send the child that number
            printf("%d\n", *shmPtr);
            shmPtr--;
        }

        sharedArrayEnd--;   //Skip the root process
        for(i = 0; i < numProcs;i++){ 
            //printf("Sending signal to process %d\n", *sharedArrayEnd);
            globalShmPtr = sharedArrayEnd;
            int status = kill(*sharedArrayEnd, SIGUSR1);
            if(status == -1){
                perror("Kill failed");
                exit(EXIT_FAILURE);
            }
//            printf("Global Shm Ptr is %p\tshared array end is %p\n", globalShmPtr, sharedArrayEnd);

            while(*sharedArrayEnd != PROCESS_COMPLETE){ //Wait for process we just signaled to exit
    //            printf("IN THE LOOP\n");
                //printf("Global Shm Ptr is %d\tshared array end is %d\n", *globalShmPtr, *sharedArrayEnd);
            }
            

            sharedArrayEnd--;
      //      kill(ROOT_PID, SIGSTOP);
      }

        wait(NULL);

        shm_unlink(name);
    }

}

void child(int unnamedPipe[], int numProcs, int shmPtr[], char *fifo){

    //close the write end of pipe
    close(unnamedPipe[1]);

    // read message from parent through read end
    if(read(unnamedPipe[0],&numProcs,sizeof(numProcs)) <= 0){
        fprintf(stderr, "Problem in process %d\n", getpid());
        perror("Problem reading from pipe");
        exit(EXIT_FAILURE); 
    }
    close(unnamedPipe[0]);

    //Write PID to array
    //Every process will be a child besides the top level process
    void *writeLocation;
    writeLocation = shmPtr + (numProcs - 1); //Since shpPtr is an int array here the *sizeof(int) is implicit
    int myPid = getpid();
    memcpy(writeLocation, &myPid, sizeof(myPid));

    //Just setting for the leaf node - others will exec
    globalShmPtr = writeLocation;

    if(numProcs == 1){ //Leaf process

        //Leaf process never execs so we must print out ALIVE message here
        fprintf(stdout,"ALIVE: Level %d process with pid=%d, child of ppid=%d.\n",
             numProcs - 1, getpid(), getppid());
       
        //Set signal handler for leaf process
        if(signal(SIGUSR1, signalHandler) == SIG_ERR){
            fprintf(stderr,"An error occurred while setting a signal handler.\n");
            exit(EXIT_FAILURE);
        }

        int numPotentialReaders = 0;
        while(*shmPtr != -1){
            shmPtr += 1;
            numPotentialReaders++;
        }
        numPotentialReaders--;

        shmPtr -= 1; //Make shmPtr point to root process id
        
        //ROOT_PID = *shmPtr;

        int fd;
        fd = open(fifo, O_WRONLY);
        
        int i;
        for(i = 0; i < numPotentialReaders; i++){
            //printf("Write to pipe %d\n", i+1);
            if(write(fd, shmPtr, sizeof(int)) <= 0){
                fprintf(stderr, "Problem in process %d\n", getpid());
                perror("Problem writing to the named pipe");
                exit(EXIT_FAILURE);
            }   
        }

        close(fd);
        unlink(fifo);

        pause();
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
    const char *name = "erics_sharedMem";
    int SIZE;
    pid_t pid;

    if(argc != 2)
        printUsageAndExit();

    int numProcs = strtol(argv[1], NULL, 10);

    if(numProcs < 1 || numProcs > 32){
        fprintf(stderr, "num-procs must be between 1 and 32 inclusive.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout,"ALIVE: Level %d process with pid=%d, child of ppid=%d.\n",
            numProcs - 1, getpid(), getppid());


    SIZE = numProcs;

    //Check if the shared memory has already been created
    bool shmExists = false;
    if(access("/dev/shm/erics_sharedMem", F_OK) != -1)
        shmExists = true;

    //Set up shared memory array
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    char * fifo = "/tmp/erics_fifo";

    //Only resize the memory if we are creating it (root process only)
    if(!shmExists){
        ftruncate(shm_fd,SIZE + 1);
        
        //Create the named pipe
        if(mkfifo(fifo, 0666) != 0){
            perror("MKFIFO Failed");
        }
    }

    void *shmPtr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    void *writeLocation;


    if(!shmExists){ //This is the top level process
        
        //Write -1 at the very end of array to mark the end of it
        writeLocation = shmPtr + (numProcs) * sizeof(int); 
        int endOfArray = -1;
        memcpy(writeLocation, &endOfArray, sizeof(endOfArray));

        //Write pid in the last usable index of the array
        writeLocation = shmPtr + (numProcs - 1) * sizeof(int); 
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
            child(unnamedPipe, numProcs, (int *)shmPtr, fifo);
        else //Parent
            parent(unnamedPipe, numProcs, !shmExists, name, (int *)shmPtr);
    }

    if(numProcs == 1 && !shmExists){
        shm_unlink(name);
        unlink(fifo);
    }
    
    //Root process exit message
    fprintf(stdout,"EXITING: Level %d process with pid=%d, child of ppid=%d.\n",
            numProcs - 1, getpid(), getppid());
    
    return 0;
}
