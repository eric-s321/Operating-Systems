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
        bool leafPause, int sleepTime, bool sleepEnabled, char *executablePath){

    pid_t pid;
//    printf("In Create Processes sleep enabled is %s\n",
//            sleepEnabled ? "true" : "false");

//    printf("Executable path is %s\n", executablePath);
    if(numLevels > 1){ // This process still has children to make
        for(int i = 0; i < numChildren; i++){
            pid = fork();
            if(pid < 0){
                fprintf(stderr, "Fork Failed.\n");
                exit(EXIT_FAILURE);
            }
            if(pid == 0){ //Child process code
                char newNumLevels[255];
                char strNumChildren[255]; 
                sprintf(newNumLevels, "%d", (numLevels - 1));
                sprintf(strNumChildren, "%d", numChildren);
                //execlp("/import/linux/home/escagne1/Desktop/Operating-Systems/Prog2Scagnelli_escagne1/prog2tree",
                      //"prog2tree", "-N", newNumLevels, "-M", strNumChildren, NULL); 
                
                int status = 0;
                if(sleepEnabled){
                    char strSleepTime[255];
                    sprintf(strSleepTime, "%d", sleepTime);
//                    status = execlp("/import/linux/home/escagne1/Desktop/Operating-Systems/Prog2Scagnelli_escagne1/prog2tree",
//                           "prog2tree", "-N", newNumLevels, "-M", strNumChildren, "-s", strSleepTime, NULL); 
                    status = execlp(executablePath,"prog2tree", "-N", newNumLevels, "-M", strNumChildren,
                            "-s", strSleepTime, NULL); 
                           
                    if(status == -1){
                        fprintf(stderr, "Exec call failed.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                
            }
            else{ //Parent Process
                wait(NULL);
//                printf("Child has completed.\n"); 
            }
        }
    }

    else if (numLevels == 1){ //This process is a leaf node.
//        printf("I am a leaf node. My pid is %d and my parents pid is %d\n", getpid(), getppid());
        if(sleepEnabled)
            sleep(sleepTime);
//        printf("Done Sleeping\n");
    }
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

        char *executable = "/prog2tree";
        for(int i = 0; i < 10; i++){
            cwd[lastIndex + i] = executable[i];
        }
        cwd[lastIndex + 10] = '\0'; //Put NULL character after executable name
    }
    for(int i = 0; cwd[i] != '\0'; i++){
        retcwd[i] = cwd[i];    
    }
    retcwd[lastIndex + 10] = '\0';

    return retcwd;
}

int main(int argc, char *argv[]){



    /*
    for(int i = 0; cwd[i] != '\0'; i++){
    //    printf("%c",cwd[i]);    
        printf("i is %d\n",i);
    }
    */




    
    /*
    printf("\n\n");
    printf("argc is %d\n", argc);
    for(int i = 0; i < argc; i++){
        printf("%s\t", argv[i]);
    }
    printf("\n\n");
    */
    
    char *execPath = getExecutablePath();
//    printf("IN MAIN. the exec path is %s\n", execPath);
    

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


    
    /*
    printf("numLevels is %d\n"
        "numChildren is %d\n"
        "leafPause is %s\n"
        "sleepEnabled is %s\n"
        "sleepTime is %d\n",
    numLevels, numChildren,
    leafPause ? "true" : "false",
    sleepEnabled ? "true" : "false", sleepTime);    
    */
    

    if(numLevels >= 1){
        fprintf(stdout,"ALIVE: Level %d process with pid=%d, child of ppid=%d.\n",
            numLevels - 1, getpid(), getppid());
    }

    createProcesses(numLevels, numChildren, leafPause, sleepTime, sleepEnabled, execPath); 
    
    if(numLevels >= 1){
        fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n",
                numLevels - 1, getpid(), getppid()); 
    }

    return 0;
}
