
#include <stdbool.h>

void signalHandler(int signo);
void printUsageAndExit();
char *getExecutablePath();
void parent(int unnamedPipe[], int numProcs, bool topLevelProcess, 
        const char *name, int shmPtr[]);
void child(int unnamedPipe[], int numProcs, int shmPtr[], char *fifo);

