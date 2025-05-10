#include <stddef.h>
#include "../../include/evo_comp/selection.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/myrandom.h"

int random_selection(ga_execution *ptr_exec, ga_workspace *ptr_workspace) {
  if (ptr_exec == NULL)
    return 1;
  if (ptr_exec->selected_parents_indexes == NULL)
    return 2;
  if (ptr_workspace == NULL)
    return 3;
  const size_t population_size = ptr_exec->population_size;
  const size_t selected_parents_indexes_len =
      population_size + (population_size & 1);
  size_t *selected_parents_indexes = ptr_exec->selected_parents_indexes;
  xorshiftr128plus_state *state = &ptr_workspace->state;
  for (size_t i = 0; i < selected_parents_indexes_len; i++)
    selected_parents_indexes[i] = randsize_t(population_size, state);
  return 0;
}