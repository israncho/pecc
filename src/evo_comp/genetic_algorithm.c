#include <stdalign.h>
#include <stddef.h>
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/utils/array.h"

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
  const size_t max_offspring_per_thead = exec->population_size / n_threads + 1;

  size_t mem_needed =
      sizeof(individual) * max_offspring_per_thead + alignof(individual);
  mem_needed +=
      codification_entry_size * codification_size * max_offspring_per_thead +
      codification_entry_alignment;
  mem_needed += crossover_workspace_size + selection_workspace_size +
                mutation_workspace_size + replacement_workspace_size;

  ga_workspace *workspace_array = *ptr_to_workspace_array;
  for (size_t i = 0; i < n_threads; i++) {

    workspace_array[i].mem = NULL;
    size_t mem_capacity = mem_needed;
    if (init_array(&workspace_array[i].mem, mem_needed, 1) != ARRAY_OK)
      return 2;
    void *mem_ = workspace_array[i].mem;

    workspace_array[i].crossover_workspace_capacity = crossover_workspace_size;
    if (crossover_workspace_size > 0)
      if (setup_array_from_prealloc_mem(
              &mem_, &mem_capacity, &workspace_array[i].crossover_workspace,
              crossover_workspace_size, 1, 1) != ARRAY_OK)
        return 3;

    workspace_array[i].selection_workspace_capacity = selection_workspace_size;
    if (selection_workspace_size > 0)
      if (setup_array_from_prealloc_mem(
              &mem_, &mem_capacity, &workspace_array[i].selection_workspace,
              selection_workspace_size, 1, 1) != ARRAY_OK)
        return 4;

    workspace_array[i].mutation_workspace_capacity = mutation_workspace_size;
    if (mutation_workspace_size > 0)
      if (setup_array_from_prealloc_mem(
              &mem_, &mem_capacity, &workspace_array[i].mutation_workspace,
              mutation_workspace_size, 1, 1) != ARRAY_OK)
        return 5;

    workspace_array[i].replacement_workspace_capacity =
        replacement_workspace_size;
    if (replacement_workspace_size > 0)
      if (setup_array_from_prealloc_mem(
              &mem_, &mem_capacity, &workspace_array[i].replacement_workspace,
              replacement_workspace_size, 1, 1) != 0)
        return 6;

    if (setup_array_from_prealloc_mem(
            &mem_, &mem_capacity, (void **)&workspace_array[i].thread_offspring,
            max_offspring_per_thead, sizeof(individual),
            alignof(individual)) != ARRAY_OK)
      return 7;

    workspace_array[i].max_offspring_size = max_offspring_per_thead;
    workspace_array[i].curr_offspring_size = 0;
    for (size_t j = 0; j < max_offspring_per_thead; j++) {
      if (setup_array_from_prealloc_mem(
              &mem_, &mem_capacity,
              &workspace_array[i].thread_offspring[j].codification,
              codification_size, codification_entry_size,
              codification_entry_alignment) != ARRAY_OK)
        return 8;
    }
  }
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
      population_size + (population_size % 2), sizeof(size_t), alignof(size_t));
  if (status_code != 0)
    return 3;

  return 0;
}
