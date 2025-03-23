#include <stddef.h>
#include <stdio.h>
#include "../include/test/utils/test_input_output.h"

int main() {
  printf("Starting tests\n\n");
  test_read_file();
  test_write_to_file();
  printf("\nFinished testing\n");
  return 0;
}