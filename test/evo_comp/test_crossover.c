#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "../../include/utils/mytime.h"
#include "../../include/test/evo_comp/test_crossover.h"
#include "../../include/evo_comp/crossover.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myrandom.h"

void test_crossover() {
  printf("Testing: crossover\n");

  clock_t start = clock();
  test_random_subintervals();
  printf("\t- random_subintervals: PASSED [%.4f secs]\n", MEASURE_TIME(start));

  start = clock();
  test_order_crossover_ox1();
  printf("\t- order_crossover_ox1: PASSED [%.4f secs]\n", MEASURE_TIME(start));
}

void test_random_subintervals() {
  size_t *intervals_array = malloc(sizeof(size_t) * 15);
  size_t number_of_intervals = 0;
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0);

  for (size_t _ = 0; _ < 1000; _++) {
    const size_t range = randsize_t_i(15, 30, &state);
    const size_t overall_size_for_one = range / 2;
    random_subintervals(intervals_array, &state, &number_of_intervals, range,
                        overall_size_for_one);
    assert(intervals_array[0] == 0);

    size_t size_for_another = 0;
    size_t size_for_one = 0;
    size_t prev_j = 0;

    if (intervals_array[2])
      size_for_one += intervals_array[1] - intervals_array[0] + 1;
    else
      size_for_another += intervals_array[1] - intervals_array[0] + 1;

    prev_j = intervals_array[1];

    for (size_t i = 3; i < number_of_intervals * 3; i += 3) {
      size_t curr_i = intervals_array[i];
      size_t curr_j = intervals_array[i + 1];
      assert(prev_j + 1 == curr_i);
      assert(intervals_array[i + 2] == 0 || intervals_array[i + 2] == 1);
      bool for_child1 = intervals_array[i + 2];

      size_t sum_for_size = curr_j - curr_i + 1;

      if (for_child1)
        size_for_one += sum_for_size;
      else
        size_for_another += sum_for_size;

      prev_j = curr_j;
    }

    assert(size_for_another + size_for_one == range + 1);
    assert(intervals_array[(number_of_intervals - 1) * 3 + 1] == range);
  }

  free(intervals_array);
}

static inline void fill_parents(const size_t codification_size,
                                size_t *p1_codification,
                                size_t *p2_codification) {
  for (size_t i = 0; i < codification_size; i++) {
    p1_codification[i] = i;
    p2_codification[i] = i;
  }
}

static inline void clean_children(const size_t codification_size,
                                  size_t *c1_codification,
                                  size_t *c2_codification) {
  for (size_t i = 0; i < codification_size; i++) {
    c1_codification[i] = 0;
    c2_codification[i] = 0;
  }
}

static inline bool all_elements_present(bool *boolset, const size_t set_size,
                                        const size_t *test_set) {
  for (size_t i = 0; i < set_size; i++)
    boolset[i] = false;
  for (size_t i = 0; i < set_size; i++)
    boolset[test_set[i]] = true;
  for (size_t i = 0; i < set_size; i++)
    if (!boolset[i])
      return false;
  return true;
}

/*
static inline void print_array(const size_t *array, const size_t array_len) {
  printf("[");
  if (array_len >= 1)
    printf("%zu", array[0]);
  for (size_t i = 1; i < array_len; i++)
    printf(" ,%zu", array[i]);
  printf("]\n");
}
*/

void test_order_crossover_ox1() {
  individual parent1, parent2, child1, child2;
  parent1.codification = NULL;
  parent2.codification = NULL;
  child1.codification = NULL;
  child2.codification = NULL;

  bool *boolset = NULL;
  size_t max_codification_size = 50;
  size_t size_t_size = sizeof(size_t);
  assert(init_array((void **)&boolset, max_codification_size, sizeof(bool)) ==
         ARRAY_OK);
  assert(init_array(&parent1.codification, max_codification_size,
                    size_t_size) == ARRAY_OK);
  assert(init_array(&parent2.codification, max_codification_size,
                    size_t_size) == ARRAY_OK);
  assert(init_array(&child1.codification, max_codification_size, size_t_size) ==
         ARRAY_OK);
  assert(init_array(&child2.codification, max_codification_size, size_t_size) ==
         ARRAY_OK);

  ga_workspace workspace;
  workspace.crossover_workspace = NULL;
  size_t crossover_workspace_size =
      (15 * size_t_size) + (2 * (max_codification_size + 1) * size_t_size) +
      (sizeof(bool) * max_codification_size) + _Alignof(size_t) * 3 +
      _Alignof(bool);
  workspace.crossover_workspace_capacity = crossover_workspace_size;
  init_array(&workspace.crossover_workspace, crossover_workspace_size, 1);

  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0);

  for (size_t _ = 0; _ < 2000; _++) {
    const size_t codification_size =
        randsize_t_i(10, max_codification_size, &state);

    fill_parents(codification_size, parent1.codification, parent2.codification);
    clean_children(codification_size, child1.codification, child2.codification);

    shuffle_array_of_size_t(parent1.codification, codification_size, &state);
    shuffle_array_of_size_t(parent2.codification, codification_size, &state);
    assert(
        all_elements_present(boolset, codification_size, parent1.codification));
    assert(
        all_elements_present(boolset, codification_size, parent2.codification));

    order_crossover_ox1(&parent1, &parent2, &child1, &child2, codification_size,
                        &workspace, &state);
    assert(
        all_elements_present(boolset, codification_size, child1.codification));
    assert(
        all_elements_present(boolset, codification_size, child2.codification));
    for (size_t i = 0; i < codification_size; i++) {
      size_t p1_gene_i = ((size_t *)parent1.codification)[i];
      size_t c1_gene_i = ((size_t *)child1.codification)[i];
      size_t c2_gene_i = ((size_t *)child2.codification)[i];
      assert(p1_gene_i == c1_gene_i || p1_gene_i == c2_gene_i);
    }
  }

  free(parent1.codification);
  free(parent2.codification);
  free(child1.codification);
  free(child2.codification);
  free(workspace.crossover_workspace);
  free(boolset);
}
