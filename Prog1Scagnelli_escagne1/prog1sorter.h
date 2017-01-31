#ifndef PROG1SORTER_H
#define PROG1SORTER_H

#include <stdlib.h>
#include <stdio.h>

void printUsageAndExit();
int compareInts(const void *a, const void *b);
int getOccurenceArraySize(char *userName);
void countUserNameOccurrences(char *userName, int nums[], int lastIndex, int occurrences[]);
void checkIntRanges(int minInt, int maxInt, int *nums, int lastIndex);
void parseInput(int argc, char *argv[], int *numInts, int * minInt,
            int *maxInt, char **inputFile, char **outputFile, char **countFile);
int readInput(FILE *input, int numInts, int *nums);
void writeSortedOutput(FILE *output, int numInts, int nums[]);
void writeCountOutput(FILE *output, char *userName, int occurrences[]);

#endif
