
#include <stdbool.h>

void printUsageAndExit();
char *getExecutablePath();
void parent(int unnamedPipe[], int numProcs, bool topLevelProcess, const char *name);
void child(int unnamedPipe[], int numProcs, int shmPtr[]);

