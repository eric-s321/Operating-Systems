#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>



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

/*
void createProcesses(int numLevels, int numChildren, bool leafPause, int sleepTime){
        

}
*/



int main(int argc, char *argv[]){
    //Set default values
    int numLevels = 0;
    int numChildren = 1;
    bool leafPause = false;
    int sleepTime = 1;

    parseInput(argc, argv, &numLevels, &numChildren, &leafPause, &sleepTime);

    checkRangeOfInputs(numLevels, numChildren);

    printf("numLevels is %d\n"
        "numChildren is %d\n"
        "leafPause is %s\n"
        "sleepTime is %d\n",
    numLevels, numChildren,
    leafPause ? "true" : "false", sleepTime);    

}
