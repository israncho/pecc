#include <stddef.h>
#include <stdio.h>
#include "../include/test/utils/test_input_output.h"
#include "../include/test/utils/test_matrix.h"

int main() {
  printf("Starting tests\n\n");
  test_input_output();
  test_matrix();
  printf("\nFinished testing\n");
  return 0;
}