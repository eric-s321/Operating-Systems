#include <stdlib.h>
#include "timing.h"

//Returns current time to the caller
struct timeval startTiming(){

    struct timeval start;
    gettimeofday(&start, NULL);
    return start;

}

//Takes the start time and returns the total ellapsed time to the caller
double endTiming(struct timeval start){
    struct timeval end;
    gettimeofday(&end, NULL);
    return ((end.tv_sec - start.tv_sec) * 1000000 + 
        (end.tv_usec - start.tv_usec)) / 1000000.0;
}
