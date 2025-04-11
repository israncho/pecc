#pragma once

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <stddef.h>
#include "../../include/utils/myrandom.h"

typedef struct {
  void *codification;
  double fitness;
} individual;

typedef struct {
  size_t population_size;
  size_t generations;
  size_t total_f_execs;
  size_t curr_f_execs;
  size_t n_threads;
  individual current_best;
  xorshiftr128plus_state *random_state;
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
} ga_workspace;

int genetic_algorith(individual *array_of_individuals,
                     individual *array_for_offpring,
                     ga_execution *execution_data);

#endif