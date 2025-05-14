#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/test/evo_comp/test_mutation.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/mutation.h"
#include "../../include/utils/array.h"
#include "../../include/utils/mytime.h"
#include "../../include/utils/myrandom.h"

void test_mutation() {

  printf("Testing: mutation\n");
  double start = get_wall_time();
  test_swap_mutation_size_t();
  double elapsed_time = get_wall_time() - start;
  printf("\t- swap_mutation_size_t: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_population_mutation();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_mutation: PASSED [%.6f secs]\n", elapsed_time);
}

static inline void fill_array_as_simple_seq(size_t *array,
                                            const size_t array_len) {
  for (size_t i = 0; i < array_len; i++)
    array[i] = i;
}

static inline bool same_arrays(size_t *array1, size_t *array2,
                               const size_t array_len) {
  for (size_t i = 0; i < array_len; i++)
    if (array1[i] != array2[i])
      return false;
  return true;
}

void test_swap_mutation_size_t() {
  const size_t array_len = 1500;
  const double p_of_swaping_with_same_index = 1.0 / (double)array_len;
  const size_t n_execs = 1000000;
  const size_t theorical_n_of_unchanged = (size_t) (n_execs * p_of_swaping_with_same_index);

  size_t *target_array = NULL;
  assert(init_array((void **)&target_array, array_len, sizeof(size_t)) ==
         ARRAY_OK);
  fill_array_as_simple_seq(target_array, array_len);

  individual individual;
  individual.codification = NULL;

  assert(init_array((void **)&individual.codification, array_len, sizeof(size_t)) == ARRAY_OK);

  ga_workspace workspace;
  set_up_seed(&workspace.state, 0, 0, 0);

  size_t num_of_unchanged = 0;
  size_t changed = 0;

  for (size_t _ = 0; _ < n_execs; _++) {
    fill_array_as_simple_seq(individual.codification, array_len);
    swap_mutation_size_t(&individual, array_len, &workspace);
    if (same_arrays(individual.codification, target_array, array_len))
      num_of_unchanged++;
    else
     changed++;
  }
  assert(theorical_n_of_unchanged - 100 <= num_of_unchanged && num_of_unchanged <= theorical_n_of_unchanged + 100);
  assert(num_of_unchanged + changed == n_execs);

  free(target_array);
  free(individual.codification);
}

void test_population_mutation() {}