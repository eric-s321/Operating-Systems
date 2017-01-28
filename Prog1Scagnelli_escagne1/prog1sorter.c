#include <unistd.h>
#include <getopt.h>
#include "timing.c"
#include "parser.c"

void printUsageAndExit(){
    fprintf(stderr, "Usage: prog1sorter [-u] [-n <num-integers>] [-m <min-int>] [-M <max-int>]\n"
            "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]\n");
    exit(EXIT_FAILURE);
}

/*
 * Returns negative value if a should be placed before b
 * Returns positive value if b should be placed before a 
 * Returns 0 if a == b.
 */
int compareInts(const void *a, const void *b){
    int val1 = *(int *)a; //Cast to a pointer to an int and then dereference
    int val2 = *(int *)b;
    return val1 - val2;
}

int getoOccurrenceArraySize(char *userName){
    int size = 0;
    for(int i = 0; userName[i] != '\0'; i++)
        size++;
    return size;
}

// Calculate how many times the ASCII value of each character in the userID appears in the input
void countUserNameOccurrences(char *userName, int nums[], int lastIndex, int occurrences[]){
    for(int i = 0; userName[i] != '\0'; i++){
        int count = 0;
        for(int j = 0; j < lastIndex; j++){
            if(nums[j] == (int)userName[i])
                count++;
        }
        occurrences[i] = count;
    }
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
    bool missingArg = false;
    setDefaults(numInts, minInt, maxInt);
    while ((arg = getopt(argc, argv, "un:m:M:i:o:c:")) != -1){
        missingArg = parseSharedFlags(arg, optarg, numInts, minInt, maxInt, outputFile);
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
            case 'i':
                checkArgument(optarg);
                *inputFile = optarg;
                break;
            case 'c':
                checkArgument(optarg);
                *countFile = optarg;
                break;
        }
    }
}

int readInput(FILE *input, int numInts, int *nums){
    char *buf = (char *) malloc(sizeof(char) * 255);
    int i = 0;
    int numLines = 0; 
    int firstLine;

    //Extract first line and check it matches the -n flag
    fgets(buf, sizeof(buf), input);
    firstLine = atoi(buf);
    if(firstLine != numInts){
        fprintf(stderr, "The first line of input and the num-integers argument do not match.\n"); 
        printUsageAndExit();
    }
    while(i < numInts && fgets(buf, sizeof(buf), input) != NULL){
        nums[i] = atoi(buf);
        i++;
        numLines++;
    }

    fclose(input);
    free(buf);
	return numLines;  //last index that should be accessed in the array
}

void writeSortedOutput(FILE *output, int numInts, int nums[]){
    for(int i = 0; i < numInts; i++){
        fprintf(output, "%d\n", nums[i]);
    } 
}

void writeCountOutput(FILE *output, char *userName, int occurrences[]){
    for(int i = 0; userName[i] != '\0'; i++){
        fprintf(output, "%c\t%d\t%d\n", userName[i], (int)userName[i], occurrences[i]);
    } 
}

int main(int argc, char *argv[]){
    char *userName = getenv("USER");
    int numInts;
    int minInt;
    int maxInt;
    char *inputFile = NULL;
    char *outputFile = NULL;
    char *countFile = NULL;

    parseInput(argc, argv, &numInts, &minInt, &maxInt,
            &inputFile, &outputFile, &countFile);

    struct timeval startTime = startTiming(); //Begin timing program after parsing

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

/*
    printf("Before sorting...\n");
    for(int i = 0; i < lastIndex; i++){
        printf("%d\t", nums[i]);
    }
    printf("\n\n");
*/

    qsort(nums, lastIndex, sizeof(int), compareInts);

    if(outputFile == NULL){ //output is stdout
        writeSortedOutput(stdout, numInts, nums);
    }
    else{ //output should be written to a file
        FILE *file = fopen(outputFile, "w");
        if(file == NULL){
            fprintf(stderr, "Error creating file %s\n", outputFile);
            exit(EXIT_FAILURE);
        }
        writeSortedOutput(file, numInts, nums);
    }

    int occurrenceArraySize = getoOccurrenceArraySize(userName);
    int *occurrences = (int *) malloc(sizeof(int) * occurrenceArraySize);

    countUserNameOccurrences(userName, nums, lastIndex, occurrences);

    if(countFile == NULL){
        writeCountOutput(stdout, userName, occurrences);
    }
    else{
        FILE *file = fopen(countFile, "w");
        if(file == NULL){
            fprintf(stderr, "Error creating file %s\n", countFile);
            exit(EXIT_FAILURE);
        }
        writeCountOutput(file, userName, occurrences);
    }

    free(occurrences);
	free(nums);

    double totalTime = endTiming(startTime);
    fprintf(stderr, "The program took %.6f seconds.\n", totalTime);
}
