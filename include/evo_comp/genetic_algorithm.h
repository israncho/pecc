#pragma once

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <stdalign.h>
#include <stddef.h>
#include "../../include/utils/myrandom.h"

typedef struct {
  void *codification;
  double fitness;
} individual;

typedef struct {
  void *mem;
  individual *population;
  individual *offspring;
  individual *current_best;
  size_t *selected_parents_indexes;
  size_t codification_size;
  size_t codification_entry_size;
  size_t codification_entry_alignment;
  size_t population_size;
  size_t generations;
  size_t total_f_execs;
  size_t curr_f_execs;
  size_t n_threads;
  double mutation_rate;
} ga_execution;

typedef struct {
  void *mem;
  void *scratch_space;
  individual *ptr_to_thread_best;
  size_t scratch_space_capacity;
  size_t offspring_size_of_previous_threads;
  size_t thread_offspring_size;
  size_t local_search_iterations;
  size_t thread_id;
  xorshiftr128plus_state state;
} ga_workspace;

static inline size_t
memory_needed_for_ga_execution(const ga_execution *exec) {
  const size_t codification_entry_size = exec->codification_entry_size;
  const size_t codification_entry_alignment = exec->codification_entry_alignment;
  const size_t population_size = exec->population_size;
  // considering both population, offspring and current_best
  const size_t total_num_of_individuals = population_size * 2 + 1;
  // bytes for array of individuals
  size_t memory_needed =
      sizeof(individual) * total_num_of_individuals + alignof(individual);
  // bytes for codification of each individual
  memory_needed +=
      codification_entry_size * exec->codification_size * total_num_of_individuals +
      codification_entry_alignment;
  // bytes for array of indexes of selected parents
  memory_needed += sizeof(size_t) * (population_size + (population_size & 1)) +
                   alignof(size_t);
  return memory_needed;
}

int setup_dynamic_mem_for_ga_workspace(
    ga_workspace **ptr_to_workspace_array, const ga_execution *exec,
    const size_t crossover_workspace_size,
    const size_t selection_workspace_size,
    const size_t mutation_workspace_size,
    const size_t target_f_workspace_size,
    const size_t replacement_workspace_size);

int setup_dynamic_mem_for_ga_execution(ga_execution *exec);

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
                         xorshiftr128plus_state*));

int setup_from_prealloc_mem_arrays_for_ga_execution(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity, ga_execution *exec);

int population_fitness_computing(individual *population, ga_workspace *thread_workspace,
                                 void *instance_details,
                                 double (*fitness_f)(void *, void *, ga_workspace *));

int update_current_best(ga_execution *exec, ga_workspace *workspace_array, const size_t n_threads);
  
int parallel_genetic_algorithm(ga_execution *exec,
                               ga_workspace *workspace_array,
                               const size_t n_threads,
                               void *instance,
                               int (*selection)(ga_execution*, ga_workspace*), 
                               int (*crossover)(const individual *, const individual *, 
                                individual *, individual *, const size_t, ga_workspace *),
                               int (*mutation)(individual *, const size_t, ga_workspace *),
                               double (*fitness_f)(void *, void *, ga_workspace *),
                               int (*replacement)(ga_execution*, ga_workspace*));

#endif