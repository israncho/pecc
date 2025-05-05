#pragma once

#ifndef MYTIME_H
#define MYTIME_H

#include <time.h>

static inline double get_wall_time() {
    struct timespec ts;  // struct to store secs and nanosecs 
    clock_gettime(CLOCK_MONOTONIC, &ts);  // get current time 
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

#endif