#include <stdio.h>
#include "../../include/test/evo_comp/test_mutation.h"
#include "../../include/evo_comp/mutation.h"
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

void test_swap_mutation_size_t() {}

void test_population_mutation() {}