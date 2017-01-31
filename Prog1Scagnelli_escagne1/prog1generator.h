#ifndef PROG1GENERATOR_H
#define PROG1GENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void printUsageAndExit();
int getRandomNum(int lowerBound, int upperBound);
void generateOutput(FILE *output, int numInts, int minInt, int maxInt);
bool parseInput(int argc, char *argv[], int *numInts, int *minInt, int *maxInt,
            unsigned long *seed, char **outputFile);
int main(int argc, char *argv[]);

#endif
