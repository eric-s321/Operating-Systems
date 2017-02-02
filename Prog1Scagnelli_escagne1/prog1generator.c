#include <getopt.h>
#include <time.h>
#include "prog1generator.h"
#include "parser.h"
#include "timing.h"


void printUsageAndExit(){
    fprintf(stderr, "prog1generator [-u] [-n <num-integers>] [-m <min-int>]"
           "[-M <max-int>] [-s <seed>] [-o <output-file-name>]");
    exit(EXIT_FAILURE);
}

int getRandomNum(int lowerBound, int upperBound){
    return rand() % (upperBound + 1 - lowerBound) + lowerBound;
}

void generateOutput(FILE *output, int numInts, int minInt, int maxInt){
    fprintf(output, "%d\n", numInts);
    for(int i = 0; i < numInts; i++){
        fprintf(output, "%d\n", getRandomNum(minInt, maxInt));
    }   
}

bool parseInput(int argc, char *argv[], int *numInts, int *minInt, int *maxInt,
    unsigned long *seed, char **outputFile){
    int arg;
    bool missingArg = false;
    bool seedFound = false;
    setDefaults(numInts, minInt, maxInt);
    while ((arg = getopt(argc, argv, "un:m:M:s:o:")) != -1){
        missingArg = parseSharedFlags(arg, optarg, numInts, minInt, maxInt, outputFile);
        bool missingSeedArg = false;
        if(missingArg)
            printUsageAndExit();
        switch(arg){
            case '?':
                fprintf(stderr, "Invalid input.\n");
                printUsageAndExit();
                break;
            case 'u':
                printUsageAndExit();
                break;
            case 's':
                missingSeedArg = checkArgument(optarg);
                if(missingSeedArg){
                    fprintf(stderr, "The -s flag requires an arugment to follow it.\n");
                    exit(EXIT_FAILURE);
                }
                *seed = strtoul(optarg, NULL, 10);
                seedFound = true;
                break;
        }
    }
    return seedFound;
}

int main(int argc, char *argv[]){
    int numInts;
    int minInt;
    int maxInt;
    unsigned long seed;
    bool seedFound = false;
    char *outputFile = NULL;

    seedFound = parseInput(argc, argv, &numInts, &minInt, &maxInt, &seed, &outputFile);

    struct timeval startTime = startTiming();

    if (maxInt < minInt){
        fprintf(stderr, "The min-int must be less then the max-int.\n");
        exit(EXIT_FAILURE);
    }

    if(seedFound){
        srand(seed);
    }
    else{
        srand(time(NULL));
    }

//void generateOutput(FILE *output, int numInts, int minInt, int maxInt){
    if(outputFile == NULL)
        generateOutput(stdout, numInts, minInt, maxInt);
    else{ //output should be written to a file
        FILE *file = fopen(outputFile, "w");
        if(file == NULL){
            fprintf(stderr, "Error creating file %s\n", outputFile);
            exit(EXIT_FAILURE);
        } 
        generateOutput(file, numInts, minInt, maxInt);
    }

    double totalTime = endTiming(startTime);
    fprintf(stderr, "The generator program took %.6f seconds.\n", totalTime);
}
