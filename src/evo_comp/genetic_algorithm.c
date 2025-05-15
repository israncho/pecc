#include <float.h>
#include <stdalign.h>
#include <stddef.h>
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myalgorithms.h"

int setup_dynamic_mem_for_ga_execution(
    ga_execution *exec, const size_t codification_entry_size,
    const size_t codification_entry_alignment) {

  const size_t total_memory_needed = memory_needed_for_ga_execution(
      exec, codification_entry_size, codification_entry_alignment);
  exec->mem = NULL;
  if (init_array(&exec->mem, total_memory_needed, 1) != ARRAY_OK)
    return 1;
  size_t memory_capacity = total_memory_needed;
  void *mem_ = exec->mem;

  if (setup_from_prealloc_mem_arrays_for_ga_execution(
          &mem_, &memory_capacity, exec, codification_entry_size,
          codification_entry_alignment) != 0)
    return 2;

  if (setup_array_from_prealloc_mem(
          &mem_, &memory_capacity, (void **)&exec->current_best, 1,
          sizeof(individual), alignof(individual)) != ARRAY_OK)
    return 3;

  if (setup_array_from_prealloc_mem(
          &mem_, &memory_capacity, &exec->current_best->codification,
          exec->codification_size, codification_entry_size,
          codification_entry_alignment) != ARRAY_OK)
    return 4;

  return 0;
}

int setup_dynamic_mem_for_ga_workspace(
    ga_workspace **ptr_to_workspace_array, const ga_execution *exec,
    const size_t n_threads, const size_t crossover_workspace_size,
    const size_t selection_workspace_size, const size_t mutation_workspace_size,
    const size_t replacement_workspace_size,
    const size_t codification_entry_size,
    const size_t codification_entry_alignment) {

  if (init_array((void **)ptr_to_workspace_array, n_threads,
                 sizeof(ga_workspace)) != 0)
    return 1;

  const size_t codification_size = exec->codification_size;
  const size_t population_size = exec->population_size;
  const size_t size_needed_per_individual =
      alignof(individual) + sizeof(individual) + codification_entry_alignment +
      codification_entry_size * codification_size;
  const size_t pop_per_thread = population_size / n_threads;
  // only even numbers
  const size_t min_offspring_per_thread = pop_per_thread - (pop_per_thread & 1);
  // always holds: remainder <= 2 * n_threads - 1
  size_t remainder =
      (population_size % n_threads) + ((pop_per_thread & 1) * n_threads);

  const size_t all_sizes[4] = {
      crossover_workspace_size, selection_workspace_size,
      mutation_workspace_size, replacement_workspace_size};
  const size_t mem_for_workspace = all_sizes[index_of_the_max_val(
      all_sizes, 4, sizeof(size_t), compare_size_t)];
  const size_t mem_needed_per_thread =
      size_needed_per_individual + mem_for_workspace;

  ga_workspace *workspace_array = *ptr_to_workspace_array;
  size_t offspring_size_of_previous_threads = 0;

  for (size_t i = 0; i < n_threads; i++) {
    size_t thread_offspring_size = min_offspring_per_thread;
    if (remainder >= 2) {
      remainder -= 2;
      thread_offspring_size += 2;
    } else if (remainder == 1 && i + 1 == n_threads) {
      remainder -= 1;
      thread_offspring_size++;
    }

    workspace_array[i].mem = NULL;

    workspace_array[i].offspring_size_of_previous_threads =
        offspring_size_of_previous_threads;
    workspace_array[i].thread_offspring_size = thread_offspring_size;
    offspring_size_of_previous_threads += thread_offspring_size;

    if (init_array(&workspace_array[i].mem, mem_needed_per_thread, 1) !=
        ARRAY_OK)
      return 2;
    void *mem_ = workspace_array[i].mem;

    size_t mem_capacity = mem_needed_per_thread;

    workspace_array[i].scratch_space_capacity = mem_for_workspace;
    if (crossover_workspace_size > 0)
      if (setup_array_from_prealloc_mem(
              &mem_, &mem_capacity, &workspace_array[i].scratch_space,
              crossover_workspace_size, 1, 1) != ARRAY_OK)
        return 3;

    if (setup_array_from_prealloc_mem(
            &mem_, &mem_capacity, (void **)&workspace_array[i].thread_best, 1,
            sizeof(individual), alignof(individual)) != ARRAY_OK)
      return 4;

    if (setup_array_from_prealloc_mem(
            &mem_, &mem_capacity, &workspace_array[i].thread_best->codification,
            codification_size, codification_entry_size,
            codification_entry_alignment) != ARRAY_OK)
      return 5;
  }
  if (offspring_size_of_previous_threads != population_size)
    return 6;
  return 0;
}

int setup_from_prealloc_mem_arrays_for_ga_execution(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity, ga_execution *exec,
    const size_t codification_entry_size,
    const size_t codification_entry_alignment) {
  const size_t population_size = exec->population_size;
  const size_t codification_size = exec->codification_size;

  int status_code = setup_population_from_prealloc_mem(
      ptr_to_mem, ptr_to_mem_capacity, &exec->population, population_size,
      codification_size, codification_entry_size, codification_entry_alignment);
  if (status_code != 0)
    return 1;

  status_code = setup_population_from_prealloc_mem(
      ptr_to_mem, ptr_to_mem_capacity, &exec->offspring, population_size,
      codification_size, codification_entry_size, codification_entry_alignment);
  if (status_code != 0)
    return 2;

  status_code = setup_array_from_prealloc_mem(
      ptr_to_mem, ptr_to_mem_capacity, (void **)&exec->selected_parents_indexes,
      population_size + (population_size & 1), sizeof(size_t), alignof(size_t));
  if (status_code != 0)
    return 3;

  return 0;
}

int population_fitness_computing(ga_execution *exec, ga_workspace *workspace,
                                 void *instance_details,
                                 int (*fitness_f)(void *, void *)) {
  if (!exec || !workspace || !instance_details)
    return !exec ? 1 : (!workspace ? 2 : 3);

  const size_t beginning = workspace->offspring_size_of_previous_threads;
  const size_t end = workspace->thread_offspring_size + beginning;

  individual *offspring = exec->offspring;
  individual *thread_best = workspace->thread_best;

  thread_best->codification = NULL;
  thread_best->fitness = DBL_MAX;
  for (size_t i = beginning; i < end; i++) {
    individual *current = &offspring[i];
    current->fitness = fitness_f(current->codification, instance_details);
    if (current->fitness < thread_best->fitness) {
      thread_best->codification = current->codification;
      thread_best->fitness = current->fitness;
    }
  }
  return 0;
}