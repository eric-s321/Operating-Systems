#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>



void printUsageAndExit(){
    fprintf(stderr, "Usage: prog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time>]\n");
    exit(EXIT_FAILURE);
}

bool checkArgument(char *optarg){
    if(optarg[0] == '-'){
        fprintf(stderr, "Missing an argument.\n");
        return true;
    }
    return false;
}

void parseInput(int argc, char *argv[], int *numLevels, int *numChildren, 
        bool *leafPause, int *sleepTime){
    int arg;
    bool missingArg = false;
    while((arg = getopt(argc, argv, "uN:M:ps:")) != -1){
        switch(arg){
            case '?':
                fprintf(stderr, "Invalid input.\n");
                printUsageAndExit();
                break;
            case 'u':
                printUsageAndExit();
                break;
            case 'N':
                missingArg = checkArgument(optarg);
                if(missingArg){
                    fprintf(stderr, "The -N flag requires an argument to follow it.\n");
                    exit(EXIT_FAILURE);
                }
                *numLevels = atoi(optarg);
                break;
            case 'M':
                missingArg = checkArgument(optarg);
                if(missingArg){
                    fprintf(stderr, "The -M flag requires an argument to follow it.\n");
                    exit(EXIT_FAILURE);
                }
                *numChildren = atoi(optarg);
                break;
            case 'p':
                *leafPause = true;
                break;
            case 's':
                missingArg = checkArgument(optarg);
                if(missingArg){
                    fprintf(stderr, "The -s flag requires an argument to follow it.\n");
                    exit(EXIT_FAILURE);
                }
                *sleepTime = atoi(optarg);
                break;
        }
    }
}

void checkRangeOfInputs(int numLevels, int numChildren){

    if(numLevels > 4){
        fprintf(stderr, "The maximum number of levels allowed is 4.\n");
        printUsageAndExit();
    }
    if(numChildren > 3){
        fprintf(stderr, "The maximum number of children allowed is 3.\n");
        printUsageAndExit();
    }
}


int main(int argc, char *argv[]){

/////////////////////////USE THIS TO AVOID HARDCODING THE PATH ////////////////////////////////////
//Found at http://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "Current working dir: %s\n", cwd);

    for(int i = 0; cwd[i] != '\0'; i++){
        printf("%c",cwd[i]);    
    }
    printf("\n");

    /*
    printf("\n\n");
    printf("argc is %d\n", argc);
    for(int i = 0; i < argc; i++){
        printf("%s\t", argv[i]);
    }
    printf("\n\n");
    */
    

    //Set default values
    int numLevels = 0;
    int numChildren = 1;
    bool leafPause = false;
    int sleepTime = 1;

    parseInput(argc, argv, &numLevels, &numChildren, &leafPause, &sleepTime);

    checkRangeOfInputs(numLevels, numChildren);

/*
    printf("numLevels is %d\n"
        "numChildren is %d\n"
        "leafPause is %s\n"
        "sleepTime is %d\n",
    numLevels, numChildren,
    leafPause ? "true" : "false", sleepTime);    
*/
    
    fprintf(stdout,"ALIVE: Level %d process with pid=%d, child of ppid=%d.\n",
            numLevels - 1, getpid(), getppid());

    pid_t pid;

    if(numLevels > 1){ // This process still has children to make
        for(int i = 0; i < numChildren; i++){
            pid = fork();
            if(pid < 0){
                fprintf(stderr, "Fork Failed.\n");
                return 1;
            }
            if(pid == 0){ //Child process code
                char newNumLevels[255];
                char strNumChildren[255]; 
                sprintf(newNumLevels, "%d", (numLevels - 1));
                sprintf(strNumChildren, "%d", numChildren);
                execlp("/import/linux/home/escagne1/Desktop/Operating-Systems/Prog2Scagnelli_escagne1/prog2tree",
                           "prog2tree", "-N", newNumLevels, "-M", strNumChildren, NULL); 
            }
            else{ //Parent Process
                wait(NULL);
                printf("Child has completed.\n"); 
            }
        }
    }

    else{ //This process is a leaf node.
        printf("I am a leaf node. My pid is %d and my parents pid is %d\n", getpid(), getppid());
    }
    
    fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n",
                numLevels - 1, getpid(), getppid()); 
    return 0;
}
