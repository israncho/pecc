#include <stddef.h>
#include <stdio.h>
#include "../include/test/utils/test_input_output.h"
#include "../include/test/utils/test_matrix.h"
#include "../include/test/utils/test_mystring.h"

int main() {
  printf("Starting tests\n\n");
  test_input_output();
  test_matrix();
  test_mystring();
  printf("\nFinished testing\n");
  return 0;
}