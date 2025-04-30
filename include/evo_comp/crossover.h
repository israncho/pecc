#pragma once

#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include "../../include/utils/myrandom.h"
#include "genetic_algorithm.h"

int order_crossover_ox1(const individual *ptr_parent1,
                        const individual *ptr_parent2, individual *ptr_child1,
                        individual *ptr_child2, const size_t chromosome_size,
                        ga_workspace *ptr_workspace);

void random_subintervals(size_t *intervals_array, xorshiftr128plus_state *state,
                         size_t *number_of_intervals, const size_t range,
                         const size_t overall_size_for_one);

static inline size_t ox1_workspace_size(const size_t codification_size,
                                        const size_t n_threads,
                                        const size_t population_size) {
  const size_t mem_needed_to_perform_ox1 =
      (15 * sizeof(size_t)) + (2 * (codification_size + 1) * sizeof(size_t)) +
      (sizeof(bool) * codification_size) + alignof(size_t) * 3 + alignof(bool);
  if (n_threads == 1)
    return mem_needed_to_perform_ox1;
  const size_t childs_per_thread = population_size / n_threads + 1;
  return codification_size * sizeof(size_t) * childs_per_thread +
         mem_needed_to_perform_ox1 + alignof(size_t);
}

int population_crossover(
    ga_execution *ptr_exec_data, ga_workspace *workspace_array,
    int (*crossover)(const individual *, const individual *, individual *,
                     individual *, const size_t, ga_workspace *),
    const size_t thread_id, const size_t num_of_threads);

#endif