#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <stddef.h>

typedef struct {
  void *codification;
  double fitness;
} individual;

typedef struct {
  size_t population_size;
  size_t generations;
  size_t total_f_execs;
  size_t curr_f_execs;
  individual current_best;
} ga_execution;

int genetic_algorith(individual *array_of_individuals,
                     individual *array_for_offpring,
                     ga_execution *execution_data);

#endif