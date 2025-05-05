#include <stddef.h>
#include <stdio.h>
#include "../include/utils/mytime.h"
#include "../include/test/utils/test_input_output.h"
#include "../include/test/utils/test_matrix.h"
#include "../include/test/utils/test_mystring.h"
#include "../include/test/utils/test_myrandom.h"
#include "../include/test/utils/test_array.h"
#include "../include/test/evo_comp/test_crossover.h"
#include "../include/test/evo_comp/test_population.h"
#include "../include/test/tsp/test_euclidean.h"

int main() {
  double start = get_wall_time();
  printf("Starting tests\n\n");

  test_input_output();
  test_matrix();
  test_mystring();
  test_euclidean();
  test_myrandom();
  test_array();
  test_crossover();
  test_population();

  double elapsed = get_wall_time() - start;
  printf("\nFinished testing in %.6f seconds\n", elapsed);
  return 0;
}