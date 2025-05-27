#include <float.h>
#include <omp.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/crossover.h"
#include "../../include/evo_comp/mutation.h"
#include "../../include/evo_comp/population.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myalgorithms.h"
#include "../../include/utils/myrandom.h"

int setup_dynamic_mem_for_ga_execution(ga_execution *exec) {
  const size_t codification_entry_size = exec->codification_entry_size;
  const size_t codification_entry_alignment =
      exec->codification_entry_alignment;
  const size_t total_memory_needed = memory_needed_for_ga_execution(exec);
  exec->mem = NULL;
  if (init_array(&exec->mem, total_memory_needed, 1) != ARRAY_OK)
    return 1;
  size_t memory_capacity = total_memory_needed;
  void *mem_ = exec->mem;

  if (setup_from_prealloc_mem_arrays_for_ga_execution(&mem_, &memory_capacity,
                                                      exec) != 0)
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

  exec->current_best->fitness = DBL_MAX;

  return 0;
}

int init_ga_execution(ga_execution *exec,
                      const size_t n_threads,
                      const size_t generations,
                      const size_t population_size,
                      const size_t codification_size,
                      const size_t codification_entry_size,
                      const size_t codification_entry_alignment,
                      const double mutation_rate,
                      const uint64_t seed1,
                      const uint64_t seed2,
                      int(*population_initializer)
                        (individual*, 
                         const size_t,
                         const size_t,
                         xorshiftr128plus_state*)) {
  if (exec == NULL)
    return 1;

  const size_t min_population_per_thread = population_size / 2;
  exec->n_threads = (min_population_per_thread >= n_threads) ? n_threads : min_population_per_thread;
  exec->generations = generations;
  exec->population_size = population_size;
  exec->codification_size = codification_size;
  exec->codification_entry_size = codification_entry_size;
  exec->codification_entry_alignment = codification_entry_alignment;
  exec->mutation_rate = mutation_rate;

  if (setup_dynamic_mem_for_ga_execution(exec) != 0)
    return 2;

  xorshiftr128plus_state state;
  set_up_seed(&state, seed1, seed2, n_threads);

  if (population_initializer(exec->population, population_size, codification_size, &state) != 0) 
    return 3;
  if (population_initializer(exec->offspring, population_size, codification_size, &state) != 0) 
    return 4;
  return 0;
}

int setup_dynamic_mem_for_ga_workspace(
    ga_workspace **ptr_to_workspace_array, const ga_execution *exec,
    const size_t n_threads, const size_t crossover_workspace_size,
    const size_t selection_workspace_size, const size_t mutation_workspace_size,
    const size_t target_f_workspace_size,
    const size_t replacement_workspace_size) {

  if (init_array((void **)ptr_to_workspace_array, n_threads,
                 sizeof(ga_workspace)) != 0)
    return 1;

  const size_t population_size = exec->population_size;
  const size_t pop_per_thread = population_size / n_threads;
  // only even numbers
  const size_t min_offspring_per_thread = pop_per_thread - (pop_per_thread & 1);
  // always holds: remainder <= 2 * n_threads - 1
  size_t remainder =
      (population_size % n_threads) + ((pop_per_thread & 1) * n_threads);

  const size_t all_sizes[5] = {crossover_workspace_size,
                               selection_workspace_size,
                               mutation_workspace_size, 
                               target_f_workspace_size,
                               replacement_workspace_size};
  const size_t mem_for_workspace = all_sizes[index_of_the_max_val(
      all_sizes, 5, sizeof(size_t), compare_size_t)];

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
    workspace_array[i].thread_id = i;

    workspace_array[i].offspring_size_of_previous_threads =
        offspring_size_of_previous_threads;
    workspace_array[i].thread_offspring_size = thread_offspring_size;
    offspring_size_of_previous_threads += thread_offspring_size;

    if (mem_for_workspace > 0)
      if (init_array(&workspace_array[i].mem, mem_for_workspace, 1) != ARRAY_OK)
        return 2;
    void *mem_ = workspace_array[i].mem;

    size_t mem_capacity = mem_for_workspace;

    workspace_array[i].scratch_space_capacity = mem_for_workspace;
    if (mem_for_workspace > 0)
      if (setup_array_from_prealloc_mem(&mem_, &mem_capacity,
                                        &workspace_array[i].scratch_space,
                                        mem_for_workspace, 1, 1) != ARRAY_OK)
        return 3;
  }
  if (offspring_size_of_previous_threads != population_size)
    return 6;
  return 0;
}

int setup_from_prealloc_mem_arrays_for_ga_execution(void **ptr_to_mem,
                                                    size_t *ptr_to_mem_capacity,
                                                    ga_execution *exec) {
  const size_t codification_entry_size = exec->codification_entry_size;
  const size_t codification_entry_alignment =
      exec->codification_entry_alignment;
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

int population_fitness_computing(individual *population,
                                 ga_workspace *thread_workspace,
                                 void *instance_details,
                                 double (*fitness_f)(void *, void *, ga_workspace *)) {
  if (!population || !thread_workspace || !instance_details)
    return !population ? 1 : (!thread_workspace ? 2 : 3);

  const size_t beginning = thread_workspace->offspring_size_of_previous_threads;
  const size_t end = thread_workspace->thread_offspring_size + beginning;

  individual **thread_best = &thread_workspace->ptr_to_thread_best;
  double best_fitness_found = DBL_MAX;

  for (size_t i = beginning; i < end; i++) {
    individual *current = &population[i];
    const double curr_fitness =
        fitness_f(current->codification, instance_details, thread_workspace);
    current->fitness = curr_fitness;
    if (curr_fitness < best_fitness_found) {
      *thread_best = current;
      best_fitness_found = curr_fitness;
    }
  }
  return 0;
}

int update_current_best(ga_execution *exec, ga_workspace *workspace_array,
                        const size_t n_threads) {
  individual *current_best = exec->current_best;
  double fitness_current_best = current_best->fitness;
  for (size_t i = 0; i < n_threads; i++) {
    individual *thread_best = workspace_array[i].ptr_to_thread_best;
    const double fitness_thread_best = thread_best->fitness;
    if (fitness_thread_best < fitness_current_best) {
      fitness_current_best = fitness_thread_best;
      current_best = thread_best;
    }
  }
  if (current_best != exec->current_best) {
    memcpy(exec->current_best->codification, current_best->codification,
           exec->codification_size * exec->codification_entry_size);
    exec->current_best->fitness = fitness_current_best;
  }
  return 0;
}

int parallel_genetic_algorithm(
    ga_execution *exec, ga_workspace *workspace_array, const size_t n_threads,
    void *instance, int (*selection)(ga_execution *, ga_workspace *),
    int (*crossover)(const individual *, const individual *, individual *,
                     individual *, const size_t, ga_workspace *),
    int (*mutation)(individual *, const size_t, ga_workspace *),
    double (*fitness_f)(void *, void *, ga_workspace *),
    int (*replacement)(ga_execution *, ga_workspace *)) {
  if (exec == NULL)
    return 1;
  if (workspace_array == NULL)
    return 2;
  if (instance == NULL)
    return 3;

  const size_t n_generations = exec->generations;
  population_fitness_computing(exec->population, &workspace_array[0], instance,
                               fitness_f);

  update_current_best(exec, workspace_array, 1);

  #pragma omp parallel num_threads(n_threads)
  {
    const size_t thread_id = omp_get_thread_num();
    ga_workspace *thread_workspace = &workspace_array[thread_id];
    for (size_t _ = 0; _ < n_generations; _++) {
      if (thread_id == 0)
        selection(exec, thread_workspace);
      #pragma omp barrier
      population_crossover(exec, thread_workspace, crossover);
      #pragma omp barrier
      if (thread_id == 0)
        population_mutation(exec, thread_workspace, mutation);
      #pragma omp barrier
      population_fitness_computing(exec->offspring, thread_workspace, instance,
                                   fitness_f);
      #pragma omp barrier
      if (thread_id == 0)
        update_current_best(exec, workspace_array, n_threads);
      #pragma omp barrier
      replacement(exec, thread_workspace);
      #pragma omp barrier
    }
  }
  return 0;
}