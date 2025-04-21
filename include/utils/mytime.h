#pragma once

#ifndef MYTIME_H
#define MYTIME_H

#include <time.h>

#define MEASURE_TIME(start) (((double)(clock() - (start))) / CLOCKS_PER_SEC)

#endif