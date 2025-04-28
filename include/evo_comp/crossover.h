#pragma once

#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdalign.h>
#include "../../include/utils/myrandom.h"
#include "genetic_algorithm.h"

int order_crossover_ox1(const individual *ptr_parent1,
                        const individual *ptr_parent2, individual *ptr_child1,
                        individual *ptr_child2, const size_t chromosome_size,
                        ga_workspace *ptr_workspace);

void random_subintervals(size_t *intervals_array, xorshiftr128plus_state *state,
                         size_t *number_of_intervals, const size_t range,
                         const size_t overall_size_for_one);

static inline size_t ox1_workspace_size(const size_t individuals_size) {
  return (15 * sizeof(size_t)) + (2 * (individuals_size + 1) * sizeof(size_t)) +
         (sizeof(bool) * individuals_size) + alignof(size_t) * 3 +
         alignof(bool);
}

int population_crossover(
    ga_execution *ptr_exec_data, ga_workspace *workspace_array,
    int (*crossover)(const individual *, const individual *, individual *,
                     individual *, const size_t, ga_workspace *),
    const size_t thread_id, const size_t num_of_threads);

#endif