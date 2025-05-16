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
  individual *thread_best;
  size_t scratch_space_capacity;
  size_t offspring_size_of_previous_threads;
  size_t thread_offspring_size;
  xorshiftr128plus_state state;
} ga_workspace;

int genetic_algorith(individual *array_of_individuals,
                     individual *array_for_offpring,
                     ga_execution *execution_data);

static inline size_t
memory_needed_for_ga_execution(const ga_execution *exec,
                               const size_t codification_entry_size,
                               const size_t codification_entry_alignment) {
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
    const size_t n_threads,
    const size_t crossover_workspace_size,
    const size_t selection_workspace_size,
    const size_t mutation_workspace_size,
    const size_t target_f_workspace_size,
    const size_t replacement_workspace_size,
    const size_t codification_entry_size,
    const size_t codification_entry_alignment);

int setup_dynamic_mem_for_ga_execution(
    ga_execution *exec, const size_t codification_entry_size,
    const size_t codification_entry_alignment);

int setup_from_prealloc_mem_arrays_for_ga_execution(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity, ga_execution *exec,
    const size_t codification_entry_size,
    const size_t codification_entry_alignment);

int copy_thread_offspring_to_ga_exec_size_t(ga_execution *exec, const ga_workspace *workspace_array);

int population_fitness_computing(ga_execution *exec, ga_workspace *thread_workspace,
                                 void *instance_details,
                                 double (*fitness_f)(void *, void *, ga_workspace *));

#endif