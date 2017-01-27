#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

void printUsageAndExit(){
    fprintf(stderr, "Usage: prog1sorter [-u] [-n <num-integers>] [-m <min-int>] [-M <max-int>]\n"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]\n");
    exit(EXIT_FAILURE);
}

//make sure all input is within range and exit on first one out of range
void checkIntRanges(int minInt, int maxInt, int *nums, int lastIndex){
    if (maxInt < minInt){
        fprintf(stderr, "The maximum integer cannot be larger then the minimum integer.\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < lastIndex; i++){
        if(nums[i] < minInt || nums[i] > maxInt){
            fprintf(stderr, "Error: the number %d is out of the specified range of %d-%d\n",
                    nums[i], minInt, maxInt);
            exit(EXIT_FAILURE);
        }
    }

}

void parseInput(int argc, char *argv[], int *numInts, int * minInt, 
    int *maxInt, char **inputFile, char **outputFile, char **countFile){
    int arg;
    while ((arg = getopt(argc, argv, "un:m:M:i:o:c:")) != -1){
		if(arg != 'u' && optarg[1] == '-'){
			printf("Missing an argument.\n");
			printUsageAndExit();
		}
        switch(arg){
			case '?':
				printf("An argmument was expected.\n");
				printUsageAndExit();	
				break;
            case 'u':
                printUsageAndExit(); 
                break;
            case 'n': 
                *numInts = atoi(optarg);
                if (*numInts < 0 || *numInts > 1000000){
                    fprintf(stderr, "The argument following -n must between the values of 0 and 1000000 inclusive.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                *minInt = atoi(optarg);
                if(*minInt < 1){
                    fprintf(stderr, "The minimum value for the -m flag is 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'M':
                *maxInt = atoi(optarg);
                if(*maxInt > 1000000){
                    fprintf(stderr, "The maximum value for the -M flag is 1000000.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'i':
                *inputFile = optarg;
                break;
            case 'o':
                *outputFile = optarg;
                break;
            case 'c':
                *countFile = optarg;
                break;
            default:
                printUsageAndExit();
                break;
        }
    }
}

int readInput(FILE *input, int numInts, int *nums){
    char buf[255];
    int i = 0;
    int numLines = 0;

    while(fgets(buf, sizeof(buf), input) != NULL){
        nums[i] = atoi(buf);
        i++;
        numLines++;
    }

    fclose(input);
	return numLines;  //last index that should be accessed in the array
}

int main(int argc, char *argv[]){
    //Set default values
	printf("%s\n",getenv("USER"));
    int numInts = 100;  //integers to read and sort
    int minInt = 1;
    int maxInt = 255;
    char *inputFile = NULL;
    char *outputFile = NULL;
    char *countFile = NULL;

    parseInput(argc, argv, &numInts, &minInt, &maxInt,
            &inputFile, &outputFile, &countFile);

    //Get Input
    int *nums = (int *) malloc(sizeof(int) * numInts);
    int lastIndex;
    if(inputFile == NULL){ //input is in stdin
		lastIndex = readInput(stdin, numInts, nums);
    }
    else{ //input is in file
		FILE *file = fopen(inputFile,"r");
		if(file == NULL){
			fprintf(stderr, "Error opening file %s\n", inputFile);
			exit(EXIT_FAILURE);
		} 
		lastIndex = readInput(file, numInts, nums);
    }

    checkIntRanges(minInt, maxInt, nums, lastIndex);
    
    printf("Printing nums...\n");
    for(int i = 0; i < lastIndex; i++){
        printf("%d\t", nums[i]);
    }
    printf("\n");
	free(nums);
/*
    if(outputFile != NULL)
        writeOutputFile = true;
    if(countFile != NULL)
        writeCountFile = true;
*/
/*
    printf("BACK IN MAIN\n");
    printf("numInts is %d\n"
            "minInt is %d\n"
            "maxInt is %d\n"
            "inputFile is %s\n"
            "outputFile is %s\n"
            "countFile is %s\n",
            numInts, minInt, maxInt, 
            inputFile, outputFile, countFile);
    if(readInputFile)
        printf("read is true\n");
    if(writeOutputFile)
        printf("output is true\n");
    if(writeCountFile)
        printf("count is true\n");
*/
}
