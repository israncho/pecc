#include <stdio.h>
#include <time.h>
#include "../../include/utils/mytime.h"
#include "../../include/test/evo_comp/test_population.h"
#include "../../include/evo_comp/population.h"

void test_population() {
  printf("Testing: population\n");

  clock_t start = clock();
  test_setup_population_from_prealloc_mem();
  printf("\t- setup_population_from_prealloc_mem: PASSED [%.4f secs]\n", MEASURE_TIME(start));

  start = clock();
  test_fill_and_shuffle_population_of_permutations();
  printf("\t- fill_and_shuffle_population_of_permutations: PASSED [%.4f secs]\n", MEASURE_TIME(start));
}

void test_fill_and_shuffle_population_of_permutations() {

}

void test_setup_population_from_prealloc_mem() {

}