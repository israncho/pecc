#pragma once

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <stddef.h>
#include <stdalign.h>
#include "../../include/utils/myrandom.h"

typedef struct {
  void *codification;
  double fitness;
} individual;

typedef struct {
  individual *population;
  individual *offspring;
  size_t *selected_parents_indexes;
  size_t codification_size;
  size_t population_size;
  size_t generations;
  size_t total_f_execs;
  size_t curr_f_execs;
  size_t n_threads;
  individual current_best;
} ga_execution;

typedef struct {
  void *crossover_workspace;
  void *selection_workspace;
  void *mutation_workspace;
  void *replacement_workspace;
  size_t crossover_workspace_capacity;
  size_t selection_workspace_capacity;
  size_t mutation_workspace_capacity;
  size_t replacement_workspace_capacity;
  xorshiftr128plus_state state;
} ga_workspace;

int genetic_algorith(individual *array_of_individuals,
                     individual *array_for_offpring,
                     ga_execution *execution_data);

static inline size_t
memory_needed_for_ga_execution(const ga_execution *exec,
                               const size_t codification_entry_size,
                               const size_t codification_entry_alignment) {
  const size_t population_size = exec->population_size;
  // bytes for array of individuals
  size_t memory_needed =
      sizeof(individual) * population_size + alignof(individual);
  // bytes for codification of each individual
  memory_needed +=
      codification_entry_size * exec->codification_size * population_size +
      codification_entry_alignment;
  // bytes for population and offspring
  memory_needed *= 2;
  // bytes for array of indexes of selected parents
  memory_needed += sizeof(size_t) * (population_size + (population_size % 2)) +
                   alignof(size_t);
  return memory_needed;
}

int setup_from_prealloc_mem_arrays_for_ga_execution(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity, ga_execution *exec,
    const size_t codification_entry_size,
    const size_t codification_entry_alignment);

#endif