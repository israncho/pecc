#include <stddef.h>
#include "../../include/evo_comp/mutation.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/myrandom.h"

int population_mutation(ga_execution *ptr_exec_data,
                        ga_workspace *thread_workspace,
                        int (*mutation)(individual *, const size_t,
                                        ga_workspace *)) {
  const size_t codification_size = ptr_exec_data->codification_size;
  const double mutation_rate = ptr_exec_data->mutation_rate;

  const size_t beginning = thread_workspace->offspring_size_of_previous_threads;
  const size_t end = thread_workspace->thread_population_size + beginning;

  xorshiftr128plus_state *state = &thread_workspace->state;

  for (size_t i = beginning; i < end; i++) {
    if (random_double(state) < mutation_rate) {
      individual *individual_to_mutate = &ptr_exec_data->offspring[i];
      mutation(individual_to_mutate, codification_size, thread_workspace);
    }
  }
  return 0;
}