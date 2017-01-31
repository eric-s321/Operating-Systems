#ifndef TIMING_H
#define TIMING_H

#include <sys/time.h>

struct timeval startTiming();
double endTiming(struct timeval start);

#endif
