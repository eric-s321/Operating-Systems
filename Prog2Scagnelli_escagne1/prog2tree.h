#include <stdbool.h>


void printUsageAndExit();
bool checkArgument(char *optarg);
void parseInput(int argc, char *argv[], int *numLevels, int *numChildren,
                bool *leafPause, int *sleepTime, bool *sleepEnabled);
void checkRangeOfInputs(int numLevels, int numChildren);
void createProcesses(int numLevels, int numChildren,
                bool leafPause, int sleepTime, bool sleepEnabled);
char * getExecutablePath(int path);
int main(int argc, char *argv[]);


