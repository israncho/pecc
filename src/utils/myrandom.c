#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "../../include/utils/myrandom.h"
#include "../../include/utils/array.h"

void set_up_seed(xorshiftr128plus_state *state, const uint64_t seed1,
                 const uint64_t seed2) {
  if (state == NULL)
    return;

  if (seed1 == 0 && seed2 == 0) {
    // entropy sources
    uint64_t t = (uint64_t)time(NULL);
    uint64_t clock_val = (uint64_t)clock();

    state->s[0] = t ^ (clock_val << 16) ^ ((uintptr_t)state);
    state->s[1] = clock_val ^ (t >> 32) ^ ((uintptr_t)state >> 16);

    if (state->s[0] == 0 && state->s[1] == 0) {
      state->s[0] = 1;
      state->s[1] = 0x6C078967;
    }
  } else {
    state->s[0] = seed1 ? seed1 : 1;
    state->s[1] = seed2 ? seed2 : 0x6C078967;
  }

  for (int i = 0; i < 10; i++)
    xorshiftr128plus(state);
}

int shuffle_array_of_size_t(size_t *array, const size_t array_size,
                            xorshiftr128plus_state *state) {
  size_t n_2 = array_size - 2;
  size_t n_1 = array_size - 1;
  for (size_t i = 0; i < n_2; i++) {
    size_t interval_size = n_1 - i + 1;
    size_t j = randsize_t(interval_size, state) + i;
    SWAP_T(array, size_t, i, j);
  }
  return 0;
}