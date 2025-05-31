#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/tsp/memetic_algorithm_euc_tsp.h"

int main(int argc, char *argv[]) {
  assert(2 <= argc && argc < 4);
  printf("Algorithm: %s\n", argv[1]);
  printf("input file: %s\n", argv[2]);

  if (strcmp(argv[1], "memetic_euc_tsp") == 0)
    exec_memetic_algorithm_for_euc_tsp(argv[2]);
  else
    printf("Unknown algorithm.\n");

  return 0;
}