#include <stddef.h>
#include "../../include/evo_comp/mutation.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/myrandom.h"

int population_mutation(ga_execution *ptr_exec_data,
                        ga_workspace *ptr_workspace,
                        int (*mutation)(individual *, const size_t,
                                        ga_workspace *)) {
  const size_t population_size = ptr_exec_data->population_size;
  const size_t codification_size = ptr_exec_data->codification_size;
  const double mutation_rate = ptr_exec_data->mutation_rate;
  xorshiftr128plus_state *state = &ptr_workspace->state;
  for (size_t i = 0; i < population_size; i++) {
    if (random_double(state) < mutation_rate) {
      individual *individual_to_mutate = &ptr_exec_data->offspring[i];
      mutation(individual_to_mutate, codification_size, ptr_workspace);
    }
  }
  return 0;
}