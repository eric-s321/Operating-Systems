#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "prog2tree.h"



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
        bool *leafPause, int *sleepTime, bool *sleepEnabled){
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
                *sleepEnabled = true;
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

void createProcesses(int numLevels, int numChildren, 
        bool leafPause, int sleepTime, bool sleepEnabled){

    pid_t pid;

    char newNumLevels[255];
    sprintf(newNumLevels, "%d", (numLevels - 1));

    if (numLevels == 1){ //This process is a leaf node.
        int status = 0;

        if(sleepEnabled){
            char strSleepTime[255];
            sprintf(strSleepTime, "%d", sleepTime);
            status = execlp(getExecutablePath(1), "sleepOrPause", "-N", newNumLevels, "-s", strSleepTime, NULL);
            if(status == -1){
                fprintf(stderr, "Exec call failed.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if(leafPause){
            status = execlp(getExecutablePath(1), "sleepOrPause", "-N", newNumLevels, "-p", NULL);
            if(status == -1){
                fprintf(stderr, "Exec call failed.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    else if(numLevels > 1){ // This process still has children to make
        for(int i = 0; i < numChildren; i++){
            pid = fork();
            if(pid < 0){
                fprintf(stderr, "Fork Failed.\n");
                exit(EXIT_FAILURE);
            }
            if(pid == 0){ //Child process code
                char strNumChildren[255]; 
                sprintf(strNumChildren, "%d", numChildren);
                
                int status = 0;
                if(sleepEnabled){
                    char strSleepTime[255];
                    sprintf(strSleepTime, "%d", sleepTime);
                    //status = execlp(executablePath,"prog2tree", "-N", newNumLevels, "-M", strNumChildren,
                     //       "-s", strSleepTime, NULL); 
                    status = execlp(getExecutablePath(0),"prog2tree", "-N", newNumLevels, "-M", strNumChildren,
                            "-s", strSleepTime, NULL); 
                           
                    if(status == -1){
                        fprintf(stderr, "Exec call failed.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else if(leafPause){
                    //status = execlp(executablePath,"prog2tree", "-N", newNumLevels, "-M", strNumChildren,
                    //        "-p", NULL); 
                    status = execlp(getExecutablePath(0),"prog2tree", "-N", newNumLevels, "-M", strNumChildren,
                            "-p", NULL); 
                    if(status == -1){
                        fprintf(stderr, "Exec call failed.\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
        /*
            Parent process waits for all it's children to finish
            leaf nodes never get here because they do not have children of their own to create
        */
        for(int i = 0; i < numChildren; i++){ 
            wait(NULL);
        }
    }
}
// path is 0 to get path to prog2tree and 1 for sleepOrPause
char * getExecutablePath(int path){

// Use this to avoid hardcoding the path so this will work on different machines 
    char *retcwd = (char *) malloc(sizeof(char) * 1024);
    char cwd[1024];
    int lastIndex = 0;
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        while(cwd[lastIndex] != '\0'){
            lastIndex++;
        }

        if(path == 0){
            char *executable = "/prog2tree";
            for(int i = 0; i < 10; i++){
                cwd[lastIndex + i] = executable[i];
            }
            cwd[lastIndex + 10] = '\0'; //Put NULL character after executable name
        }
        else if (path == 1){
            char *executable = "/sleepOrPause";
            for(int i = 0; i < 13; i++){
                cwd[lastIndex + i] = executable[i];
            }
            cwd[lastIndex + 13] = '\0'; //Put NULL character after executable name
        }
    }
    for(int i = 0; cwd[i] != '\0'; i++){
        retcwd[i] = cwd[i];    
    }

    return retcwd;
}

int main(int argc, char *argv[]){
    
    
    //Set default values
    int numLevels = 0;
    int numChildren = 1;
    bool leafPause = false;
    int sleepTime = 1;
    bool sleepEnabled = false;

    parseInput(argc, argv, &numLevels, &numChildren, &leafPause, &sleepTime,
            &sleepEnabled);

    checkRangeOfInputs(numLevels, numChildren);

    if(leafPause && sleepEnabled){
        fprintf(stderr, "The -p and -s flags cannot both be set simultaneously.\n");
        printUsageAndExit();
    }
    
    if(!leafPause && !sleepEnabled)
        sleepEnabled = true; //If user specifies neither -p nor -s we assume -s

    if(numLevels >= 1){
        fprintf(stdout,"ALIVE: Level %d process with pid=%d, child of ppid=%d.\n",
            numLevels - 1, getpid(), getppid());
    }

    createProcesses(numLevels, numChildren, leafPause, sleepTime, sleepEnabled); 
    
    if(numLevels >= 1){
        fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n",
                numLevels - 1, getpid(), getppid()); 
    }


    return 0;
}
