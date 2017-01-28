#include <stdlib.h>
#include <sys/time.h>

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
    /*
    printf("The start time in seconds: %d\tmicroseconds: %ld\n", start.tv_sec, start.tv_usec);
    printf("The end time in seconds: %d\tmicroseconds: %ld\n", end.tv_sec, end.tv_usec);
    printf("end seconds - start seconds is: %d\n", end.tv_sec - start.tv_sec);
    printf("end microseconds - start microseconds is %ld\n", end.tv_usec - start.tv_usec);
    */
    return ((end.tv_sec - start.tv_sec) * 1000000 + 
        (end.tv_usec - start.tv_usec)) / 1000000.0;
}
