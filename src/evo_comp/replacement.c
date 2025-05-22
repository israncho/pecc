#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../../include/evo_comp/replacement.h"
#include "../../include/evo_comp/genetic_algorithm.h"

int full_generational_replacement(ga_execution *exec,
                                  ga_workspace *thread_workspace) {
  if (exec == NULL)
    return 1;
  if (thread_workspace == NULL)
    return 2;

  const size_t codification_size = exec->codification_size;
  const size_t codification_entry_size = exec->codification_entry_size;
  const size_t thread_offspring_size = thread_workspace->thread_offspring_size;

  const size_t beginning = thread_workspace->offspring_size_of_previous_threads;
  const size_t end = beginning + thread_offspring_size;

  const size_t total_bytes_to_copy =
      codification_size * codification_entry_size * thread_offspring_size;

  individual *offspring = exec->offspring;
  individual *population = exec->population;

  const void *ptr_offspring_codification = offspring[beginning].codification;
  void *ptr_population_codification = population[beginning].codification;

  if (total_bytes_to_copy > 0)
    memcpy(ptr_population_codification, ptr_offspring_codification,
           total_bytes_to_copy);

  for (size_t i = beginning; i < end; i++)
    population[i].fitness = offspring[i].fitness;

  return 0;
}

int full_gen_replacement_elitism(ga_execution *exec,
                                 ga_workspace *thread_workspace) {
  if (exec == NULL)
    return 1;
  if (thread_workspace == NULL)
    return 2;

  const size_t codification_size = exec->codification_size;
  const size_t codification_entry_size = exec->codification_entry_size;
  size_t thread_offspring_size = thread_workspace->thread_offspring_size;

  size_t beginning = thread_workspace->offspring_size_of_previous_threads;
  const size_t end = beginning + thread_offspring_size;

  const bool thread_0 = beginning == 0;

  if (thread_0) {
    beginning++;
    thread_offspring_size--;
  }

  const size_t total_bytes_to_copy =
      codification_size * codification_entry_size * thread_offspring_size;

  individual *offspring = exec->offspring;
  individual *population = exec->population;

  const void *ptr_offspring_codification = offspring[beginning].codification;
  void *ptr_population_codification = population[beginning].codification;

  if (total_bytes_to_copy > 0)
    memcpy(ptr_population_codification, ptr_offspring_codification,
           total_bytes_to_copy);

  for (size_t i = beginning; i < end; i++)
    population[i].fitness = offspring[i].fitness;

  if (thread_0) {
    memcpy(exec->population[0].codification, exec->current_best->codification,
           codification_entry_size * codification_size);
    exec->population[0].fitness = exec->current_best->fitness;
  }

  return 0;
}