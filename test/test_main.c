#include <stddef.h>
#include <stdio.h>
#include "../include/test/utils/test_input_output.h"
#include "../include/test/utils/test_matrix.h"
#include "../include/test/utils/test_mystring.h"
#include "../include/test/utils/test_myrandom.h"
#include "../include/test/utils/test_array.h"
#include "../include/test/tsp/test_euclidean.h"

int main() {
  printf("Starting tests\n\n");
  test_input_output();
  test_matrix();
  test_mystring();
  test_euclidean();
  test_myrandom();
  test_array();
  printf("\nFinished testing\n");
  return 0;
}