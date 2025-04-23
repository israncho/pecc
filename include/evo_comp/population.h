#pragma once

#ifndef POPULATION_H
#define POPULATION_H

#include "genetic_algorithm.h"

int fill_and_shuffle_population_of_permutations(individual *population,
                                                const size_t population_size,
                                                const size_t codification_size,
                                                xorshiftr128plus_state *state);

int setup_population_from_prealloc_mem(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity,
    individual **ptr_to_population, const size_t population_size,
    const size_t codification_size, const size_t codification_entry_byte_size,
    const size_t alignment);

#endif