#pragma once

#ifndef MUTATION_H
#define MUTATION_H

#include "genetic_algorithm.h"
#include <stddef.h>
#include "../../include/utils/array.h"
#include "../../include/utils/myrandom.h"

static inline int swap_mutation_size_t(individual *individual,
                                       const size_t codification_size,
                                       ga_workspace *ptr_workspace) {
  if (individual == NULL) return 1;
  if (ptr_workspace == NULL) return 2;
  xorshiftr128plus_state *state = &ptr_workspace->state;
  const size_t i = randsize_t_i(0, codification_size - 1, state);
  const size_t j = randsize_t_i(0, codification_size - 1, state);
  size_t *codification = (size_t *)individual->codification;
  SWAP_T(codification, size_t, i, j);
  return 0;
}

int population_mutation(ga_execution *ptr_exec_data,
                        ga_workspace *workspace_array,
                        int (*mutation)(individual *, const size_t, ga_workspace *));

#endif