#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

void setDefaults(int *numInts, int *minInts, int *maxInt);
bool checkArgument(char *optarg);

bool parseSharedFlags(int arg, char * optarg, int *numInts, int *minInt, int*maxInt, char **outputFile);

#endif
