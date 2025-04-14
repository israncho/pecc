#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../include/test/evo_comp/test_crossover.h"
#include "../../include/evo_comp/crossover.h"
#include "../../include/utils/myrandom.h"

void test_crossover() {
  printf("Testing: crossover\n");
  test_random_subintervals();
}

void test_random_subintervals() {
  size_t *intervals_array = malloc(sizeof(size_t) * 15);
  size_t number_of_intervals = 0;
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0);

  for (size_t _ = 0; _ < 1000; _++) {
    const size_t range = randsize_t_i(15, 30, &state);
    const size_t overall_size_for_one = range / 2;
    random_subintervals(intervals_array, &state, &number_of_intervals, range,
                        overall_size_for_one);
    assert(intervals_array[0] == 0);


    size_t size_for_another = 0;
    size_t size_for_one = 0;
    size_t prev_j = 0;

    if(intervals_array[2])
      size_for_one += intervals_array[1] - intervals_array[0] + 1;
    else
      size_for_another += intervals_array[1] - intervals_array[0] + 1;

    prev_j = intervals_array[1];

    for (size_t i = 3; i < number_of_intervals * 3; i += 3) {
        size_t curr_i = intervals_array[i];
        size_t curr_j = intervals_array[i + 1];
        assert(prev_j + 1 == curr_i);
        assert(intervals_array[i + 2] == 0 || intervals_array[i + 2] == 1);
        bool for_child1 = intervals_array[i + 2];

        size_t sum_for_size = curr_j - curr_i + 1;

        if (for_child1)
          size_for_one += sum_for_size;
        else
          size_for_another += sum_for_size;

        prev_j = curr_j;
    }

    assert(size_for_another + size_for_one == range + 1);
    assert(intervals_array[(number_of_intervals - 1) * 3 + 1] == range);
  }

  free(intervals_array);
  printf("\t- random_subintervals: PASSED\n");
}