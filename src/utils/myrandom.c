#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/utils/myrandom.h"
#include "../../include/utils/array.h"

void set_up_seed(unsigned int *seed) {
  if (seed == NULL)
    srand(time(NULL));
  else
    srand(*seed);
}

int shuffle_array_of_size_t(size_t *array, const size_t array_size) {
  if (array == NULL)
    return 1;
  if (array_size <= 1)
    return 0;
  int n_2 = array_size - 2;
  int n_1 = array_size - 1;
  for (int i = 0; i < n_2; i++) {
    int j = RAND_INT(i, n_1);
    SWAP_T(array, size_t, i, j);
  }
  return 0;
}