#pragma once

#ifndef MYALGORITHMS_H
#define MYALGORITHMS_H

#include <stddef.h>

size_t bisect_left(const void *array,
                   const size_t array_len,
                   const size_t array_entry_byte_size,
                   const void *x,
                   int (*compare)(const void *, const void *));

static inline int compare_float(const void *a, const void *b) {
    const float a_as_float = *(float *)a;
    const float b_as_float = *(float *)b;
    return (a_as_float > b_as_float) - (a_as_float < b_as_float);
}

static inline int compare_size_t(const void *a, const void *b) {
    const size_t a_as_float = *(size_t *)a;
    const size_t b_as_float = *(size_t *)b;
    return (a_as_float > b_as_float) - (a_as_float < b_as_float);
}
#endif