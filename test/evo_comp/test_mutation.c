#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/test/evo_comp/test_mutation.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/mutation.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myrandom.h"
#include "../../include/utils/mytime.h"

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
  const size_t theorical_n_of_unchanged =
      (size_t)(n_execs * p_of_swaping_with_same_index);

  size_t *target_array = NULL;
  assert(init_array((void **)&target_array, array_len, sizeof(size_t)) ==
         ARRAY_OK);
  fill_array_as_simple_seq(target_array, array_len);

  individual individual;
  individual.codification = NULL;

  assert(init_array((void **)&individual.codification, array_len,
                    sizeof(size_t)) == ARRAY_OK);

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
  assert(theorical_n_of_unchanged - 100 <= num_of_unchanged &&
         num_of_unchanged <= theorical_n_of_unchanged + 100);
  assert(num_of_unchanged + changed == n_execs);

  free(target_array);
  free(individual.codification);
}

void test_population_mutation() {
  ga_execution exec_data;
  exec_data.population_size = 50000;
  exec_data.codification_size = 25;
  exec_data.population = NULL;
  exec_data.offspring = NULL;
  exec_data.selected_parents_indexes = NULL;
  exec_data.mem = NULL;

  assert(setup_dynamic_mem_for_ga_execution(&exec_data, sizeof(size_t),
                                            alignof(size_t)) == 0);
  ga_workspace workspace;
  set_up_seed(&workspace.state, 0, 0, 0);

  size_t *target_array = NULL;
  assert(init_array((void **)&target_array, exec_data.codification_size,
                    sizeof(size_t)) == ARRAY_OK);
  fill_array_as_simple_seq(target_array, exec_data.codification_size);

  const double mutation_rates[] = {0.0, 0.25, 0.50, 0.75, 1.0};

  for (size_t k = 0; k < 5; k++) {
    exec_data.mutation_rate = mutation_rates[k];
    const size_t avg_mutated =
        (size_t)(exec_data.population_size * exec_data.mutation_rate);
    const size_t avg_min = (size_t)(avg_mutated * 0.9);
    const size_t avg_max = (size_t)(avg_mutated * 1.1);

    for (size_t _ = 0; _ < 200; _++) {
      size_t changed = 0;
      for (size_t i = 0; i < exec_data.population_size; i++)
        fill_array_as_simple_seq(exec_data.offspring[i].codification,
                                 exec_data.codification_size);
      assert(population_mutation(&exec_data, &workspace,
                                 swap_mutation_size_t) == 0);
      for (size_t i = 0; i < exec_data.population_size; i++)
        if (!same_arrays(target_array, exec_data.offspring[i].codification,
                         exec_data.codification_size))
          changed++;
      assert(avg_min <= changed && changed <= avg_max);
    }
  }
  free(exec_data.mem);
  free(target_array);
}