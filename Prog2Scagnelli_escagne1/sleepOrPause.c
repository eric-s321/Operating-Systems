#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include "sleepOrPause.h"


void parseInput(int argc, char *argv[], int *numLevels, bool *pauseEnabled, bool *sleepEnabled, int *sleepTime){
    int arg;
    while((arg = getopt(argc, argv, "N:ps:")) != -1){
        switch(arg){
            case 'N':
                *numLevels = atoi(optarg);
                break;
            case 'p':
                *pauseEnabled = true;
                break;
            case 's':
                *sleepTime = atoi(optarg);
                *sleepEnabled = true;
                break;
        }
    }
}


int main(int argc, char *argv[]){

    int sleepTime;
    int numLevels;
    bool pauseEnabled = false;
    bool sleepEnabled = false;

    parseInput(argc, argv, &numLevels, &pauseEnabled, &sleepEnabled, &sleepTime);

    if(sleepEnabled)
        sleep(sleepTime);
    else if(pauseEnabled)
        pause();
    fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d.\n",
                           numLevels, getpid(), getppid());
}
