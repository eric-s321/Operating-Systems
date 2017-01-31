#include <stdlib.h>
#include <stdio.h>
//#include <stdbool.h>
#include "parser.h"

void setDefaults(int *numInts, int *minInt, int *maxInt){
    *numInts = 100;
    *minInt = 1;
    *maxInt = 255;
}
bool checkArgument(char *optarg){
    if(optarg[0] == '-'){
        printf("Missing an argument.\n");
        return true;
    }
    return false;
}

bool parseSharedFlags(int arg, char * optarg, int *numInts, int *minInt, int*maxInt, char **outputFile){
    bool missingArg = false;
    switch(arg){
        case 'n':
            missingArg = checkArgument(optarg);
            *numInts = atoi(optarg);
            if (*numInts < 0 || *numInts > 1000000){
                fprintf(stderr, "The argument following -n must between the values of 0 and 1000000 inclusive.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'm':
            missingArg = checkArgument(optarg);
            *minInt = atoi(optarg);
            if(*minInt < 1 && !missingArg){
                fprintf(stderr, "The minimum value for the -m flag is 1.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'M':
            missingArg = checkArgument(optarg);
            *maxInt = atoi(optarg);
            if(*maxInt > 1000000){
                fprintf(stderr, "The maximum value for the -M flag is 1000000.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'o':
            missingArg = checkArgument(optarg);
            *outputFile = optarg;
            break;
    }
    return missingArg;
}
