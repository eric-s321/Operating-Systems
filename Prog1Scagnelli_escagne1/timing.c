#include <time.h>

//Returns current time to the caller
time_t startTiming(){
    time_t start;
    time(&start);
    return start;
}

//Takes the start time and returns the total ellapsed time to the caller
double endTiming(time_t start){
    time_t end;
    time(&end);
    return difftime(end,start);
}
