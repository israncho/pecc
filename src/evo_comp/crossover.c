#include <omp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/evo_comp/crossover.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/array.h"

static inline void _random_subintervals(size_t *intervals_array,
                                        xorshiftr128plus_state *state,
                                        size_t *number_of_intervals,
                                        const size_t range,
                                        const size_t overall_size_for_one) {
  const size_t fst_size = randsize_t_i(1, overall_size_for_one - 1, state);
  const size_t snd_size = overall_size_for_one - fst_size;

  const size_t fst_i = randsize_t_i(0, (range - fst_size) + 1, state);
  const size_t fst_j = fst_i + (fst_size - 1);

  size_t snd_i = -1;

  if (snd_size <= fst_i)
    snd_i = randsize_t_i(0, fst_i - snd_size, state);
  else
    snd_i = randsize_t_i(fst_j + 1, (range - snd_size) + 1, state);

  const size_t snd_j = snd_i + (snd_size - 1);

  size_t tmp_arr[4];

  if (fst_i < snd_i) {
    tmp_arr[0] = fst_i;
    tmp_arr[1] = fst_j;
    tmp_arr[2] = snd_i;
    tmp_arr[3] = snd_j;
  } else {
    tmp_arr[0] = snd_i;
    tmp_arr[1] = snd_j;
    tmp_arr[2] = fst_i;
    tmp_arr[3] = fst_j;
  }

  size_t curr = 0;
  size_t n_of_intervals = 0;
  size_t intervals_arr_i = 0;
  for (size_t i = 0; i < 3; i += 2) {
    size_t fst = tmp_arr[i];
    size_t snd = tmp_arr[i + 1];
    if (curr < fst) {
      n_of_intervals++;
      intervals_array[intervals_arr_i++] = curr;
      intervals_array[intervals_arr_i++] = fst - 1;
      // indicator to separate genes from one parent or another.
      intervals_array[intervals_arr_i++] = 0;
    }
    n_of_intervals++;
    intervals_array[intervals_arr_i++] = fst;
    intervals_array[intervals_arr_i++] = snd;
    intervals_array[intervals_arr_i++] = 1;
    curr = snd + 1;
  }

  if (curr <= range) {
    n_of_intervals++;
    intervals_array[intervals_arr_i++] = curr;
    intervals_array[intervals_arr_i++] = range;
    intervals_array[intervals_arr_i++] = 0;
  }
  *number_of_intervals = n_of_intervals;
}

void random_subintervals(size_t *intervals_array, xorshiftr128plus_state *state,
                         size_t *number_of_intervals, const size_t range,
                         const size_t overall_size_for_one) {
  _random_subintervals(intervals_array, state, number_of_intervals, range,
                       overall_size_for_one);
}

int order_crossover_ox1(const individual *ptr_parent1,
                        const individual *ptr_parent2, individual *ptr_child1,
                        individual *ptr_child2, const size_t chromosome_size,
                        ga_workspace *ptr_workspace) {
  if (ptr_parent1 == NULL || ptr_parent2 == NULL)
    return 1;
  if (ptr_parent1->codification == NULL || ptr_parent2->codification == NULL)
    return 2;

  size_t *parent1 = ptr_parent1->codification;
  size_t *parent2 = ptr_parent2->codification;

  if (ptr_child1 == NULL || ptr_child1->codification == NULL)
    return 3;

  size_t *child1 = ptr_child1->codification, *child2 = NULL;
  bool two_childs = false;

  if (ptr_child2 != NULL && ptr_child2->codification != NULL) {
    two_childs = true;
    child2 = ptr_child2->codification;
  }

  if (ptr_workspace == NULL || ptr_workspace->crossover_workspace == NULL)
    return 4;

  void *workspace = ptr_workspace->crossover_workspace;
  size_t workspace_capacity = ptr_workspace->crossover_workspace_capacity;

  xorshiftr128plus_state *state = &ptr_workspace->state;

  const size_t inheritance_p1 = chromosome_size / 2;
  const size_t size_t_size = sizeof(size_t);
  const size_t size_t_alignment = _Alignof(size_t);

  size_t *intervals_array = NULL;
  setup_array_from_prealloc_mem(&workspace, &workspace_capacity,
                                (void **)&intervals_array, 15, size_t_size,
                                size_t_alignment);

  size_t *missing_indexes_child1 = NULL;
  size_t missing_ind_c1_i = 0;
  size_t *missing_indexes_child2 = NULL;
  size_t missing_ind_c2_i = 0;
  setup_array_from_prealloc_mem(
      &workspace, &workspace_capacity, (void **)&missing_indexes_child1,
      inheritance_p1 + 1, size_t_size, size_t_alignment);

  if (two_childs)
    setup_array_from_prealloc_mem(
        &workspace, &workspace_capacity, (void **)&missing_indexes_child2,
        inheritance_p1 + 1, size_t_size, size_t_alignment);

  // boolset
  bool *missing_for_child1 = NULL;
  setup_array_from_prealloc_mem(&workspace, &workspace_capacity,
                                (void **)&missing_for_child1, chromosome_size,
                                sizeof(bool), _Alignof(bool));

  for (size_t i = 0; i < chromosome_size; i++)
    missing_for_child1[i] = false;

  size_t number_of_intervals = 0;

  _random_subintervals(intervals_array, state, &number_of_intervals,
                       chromosome_size - 1, inheritance_p1);

  const size_t intervals_array_size = number_of_intervals * 3;
  for (size_t i = 0; i < intervals_array_size; i += 3) {
    const size_t a = intervals_array[i];
    const size_t b = intervals_array[i + 1];
    const bool for_child1 = intervals_array[i + 2];

    for (size_t j = a; j <= b; j++) {
      size_t gene = parent1[j];
      if (for_child1) {
        child1[j] = gene;
        if (two_childs)
          missing_indexes_child2[missing_ind_c2_i++] = j;
      } else {
        if (two_childs)
          child2[j] = gene;
        missing_indexes_child1[missing_ind_c1_i++] = j;
        missing_for_child1[gene] = true;
      }
    }
  }

  missing_ind_c1_i = 0;
  missing_ind_c2_i = 0;

  for (size_t i = 0; i < chromosome_size; i++) {
    const size_t gene_from_p2 = parent2[i];
    if (missing_for_child1[gene_from_p2])
      child1[missing_indexes_child1[missing_ind_c1_i++]] = gene_from_p2;
    else if (two_childs)
      child2[missing_indexes_child2[missing_ind_c2_i++]] = gene_from_p2;
  }

  return 0;
}

int population_crossover(
    ga_execution *ptr_exec_data, ga_workspace *workspace_array,
    int (*crossover)(const individual *, const individual *, individual *,
                     individual *, const size_t, ga_workspace *)) {
  size_t num_of_threads = ptr_exec_data->n_threads;
  size_t population_size = ptr_exec_data->population_size;
  size_t *selected_parents_indexes = ptr_exec_data->selected_parents_indexes;
  size_t codification_size = ptr_exec_data->codification_size;
  individual *population = ptr_exec_data->population;
  individual *offspring = ptr_exec_data->offspring;
#pragma omp parallel num_threads(num_of_threads)
  {
    size_t id = omp_get_thread_num();
    for (size_t i = id * 2; i < population_size; i += num_of_threads * 2) {
      size_t parent1_i = selected_parents_indexes[i];
      size_t parent2_i = selected_parents_indexes[i + 1];
      individual parent1 = population[parent1_i];
      individual parent2 = population[parent2_i];
      individual *child1 = &offspring[i];
      individual *child2 = NULL;
      if (i + 1 < population_size)
        child2 = &offspring[i + 1];
      crossover(&parent1, &parent2, child1, child2, codification_size,
                &workspace_array[id]);
    }
  }
  return 0;
}