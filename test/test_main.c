#include <stddef.h>
#include <stdio.h>
#include "../include/test/evo_comp/test_crossover.h"
#include "../include/test/evo_comp/test_population.h"
#include "../include/test/evo_comp/test_selection.h"
#include "../include/test/tsp/test_euclidean.h"
#include "../include/test/utils/test_array.h"
#include "../include/test/utils/test_input_output.h"
#include "../include/test/utils/test_matrix.h"
#include "../include/test/utils/test_myalgorithms.h"
#include "../include/test/utils/test_myrandom.h"
#include "../include/test/utils/test_mystring.h"
#include "../include/utils/mytime.h"

int main() {
  double start = get_wall_time();
  printf("Starting tests\n\n");

  test_input_output();
  test_matrix();
  test_mystring();
  test_euclidean();
  test_myrandom();
  test_array();
  test_myalgorithms();
  test_crossover();
  test_population();
  test_selection();

  double elapsed = get_wall_time() - start;
  printf("\nFinished testing in %.6f seconds\n", elapsed);
  return 0;
}