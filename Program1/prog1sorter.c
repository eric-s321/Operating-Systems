#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void printUsageAndExit(){
    fprintf(stderr, "Usage: prog1sorter [-u] [-n <num-integers>] [-m <min-int>] [-M <max-int>]\n"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]\n");
    exit(EXIT_FAILURE);
}

//make sure all input is within range and exit on first one out of range
void checkIntRanges(int minInt, int maxInt){
    if (maxInt < minInt){
        fprintf(stderr, "The maximum integer cannot be larger then the minimum integer.\n");
        exit(EXIT_FAILURE);
    }
}

int parseInput(int argc, char *argv[], int *numInts, int * minInt, 
        int *maxInt, char **inputFile, char **outputFile, char **countFile){
    int index = 1; //skip the ./prog1sorter index of argv
    int arg;
    while ((arg = getopt(argc, argv, "un:m:M:i:o:c:")) != -1){
//        printf("arg is %d\n", arg);
        switch(arg){
            case 'u':
                index += 1;
                printUsageAndExit(); 
                break;
            case 'n': 
                index += 2;
                *numInts = atoi(optarg);
                if (*numInts < 0 || *numInts > 1000000){
                    fprintf(stderr, "The argument following -n must between the values of 0 and 1000000 inclusive.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                index += 2;
                *minInt = atoi(optarg);
                if(*minInt < 1){
                    fprintf(stderr, "The minimum value for the -m flag is 1.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'M':
                index += 2;
                *maxInt = atoi(optarg);
                if(*maxInt > 1000000){
                    fprintf(stderr, "The maximum value for the -M flag is 1000000.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'i':
                index += 2;
                *inputFile = optarg;
                break;
            case 'o':
                index += 2;
                *outputFile = optarg;
                break;
            case 'c':
                index += 2;
                *countFile = optarg;
                break;
            default:
                printUsageAndExit();
                break;
        }
    }
    return index;
}

int * readStdin(char *argv[], int index, int numInts, int *nums){
    int originalIndex = index;
    for(int i = 0; index < numInts + originalIndex; index++, i++){
        nums[i] = atoi(argv[index]);
    }
    return nums;
}

int * readFromFile(char *inputFile, int numInts, int *nums){
    FILE *file = fopen(inputFile, "r");  
    if(file == NULL){
        fprintf(stderr, "Error opening file %s\n", inputFile);
        exit(EXIT_FAILURE);
    } 
    char buf[10];
    int i = 0;
  /*  int num;
    while(!feof(file)){
        printf("i is %d and numInts is %d\n",i,numInts);
        fscanf(file,"%d",&num);
        nums[i] = num;
        i++;
    }*/
    while(fgets(buf, sizeof(buf), file) != NULL){
        nums[i] = atoi(buf);
        i++;
    }
    for(int j = 0; j < i; j++){
        printf("%d\t",nums[j]);
    }
    printf("\n");
    fclose(file);
    return nums;
}

int main(int argc, char *argv[]){
    //Set default values
    int numInts = 100;  //integers to read and sort
    int minInt = 1;
    int maxInt = 255;
    char *inputFile = NULL;
    char *outputFile = NULL;
    char *countFile = NULL;

    int index = parseInput(argc, argv, &numInts, &minInt, &maxInt,
            &inputFile, &outputFile, &countFile);

    //Get Input
    int *nums = (int *) malloc(sizeof(int) * numInts);
    if(inputFile == NULL){ //input is in stdin
        nums = readStdin(argv, index, numInts, nums);
    }
    else{ //input is in file
        nums = readFromFile(inputFile, numInts, nums);
    }
    
    printf("Printing nums...\n");
    for(int i = 0; i < numInts; i++){
        printf("%d\t", nums[i]);
    }
    printf("\n");
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
