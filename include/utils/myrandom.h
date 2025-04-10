#pragma once

#ifndef MYRANDOM_H
#define MYRANDOM_H

#include <stdlib.h>
#include <stddef.h>

void set_up_seed(unsigned int *seed);

#define RAND_INT(min, max) ((min) + rand() / (RAND_MAX / ((max) - (min) + 1) + 1))

#define RAND_DOUBLE() (rand() / ((double)RAND_MAX + 1.0))

int shuffle_array_of_size_t(size_t *array, const size_t array_size);

#endif