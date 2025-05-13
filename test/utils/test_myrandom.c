#include <assert.h>
#include <math.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/test/utils/test_myrandom.h"
#include "../../include/utils/array.h"
#include "../../include/utils/matrix.h"
#include "../../include/utils/myrandom.h"
#include "../../include/utils/mytime.h"

void test_myrandom() {
  printf("Testing: myrandom\n");

  double start = get_wall_time();
  test_shuffle_array_of_size_t();
  double elapsed_time = get_wall_time() - start;
  printf("\t- shuffle_array_of_size_t: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_randsize_t();
  elapsed_time = get_wall_time() - start;
  printf("\t- randsize_t: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_randsize_t_i();
  elapsed_time = get_wall_time() - start;
  printf("\t- randsize_t_i: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_random_double();
  elapsed_time = get_wall_time() - start;
  printf("\t- random_double: PASSED [%.6f secs]\n", elapsed_time);
}

void test_randsize_t() {
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);
  const size_t max_val = 10000;
  size_t prev = randsize_t(max_val, &state);
  size_t prev_prev = randsize_t(max_val, &state);

  for (size_t _ = 0; _ < 1000000; _++) {
    const size_t curr = randsize_t(max_val, &state);
    assert(!(curr == prev && curr == prev_prev));
    assert(0 <= prev && prev < max_val);
    prev_prev = prev;
    prev = curr;
  }

  for (size_t _ = 0; _ < 1000000; _++) {
    const size_t curr = randsize_t(2, &state);
    assert(curr == 0 || curr == 1);
  }

  for (size_t max = 1; max < 100; max++) {
    const size_t val = randsize_t(max, &state);
    assert(val < max);
  }
}

void test_randsize_t_i() {
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);

  for (size_t i = 0; i < 1000; i++) {
    const size_t start = randsize_t_i(i, i + 1, &state);
    assert(start == i || start == i + 1);
    const size_t end = start + 100;
    assert(start < end);
    for (size_t _ = 0; _ < 10000; _++) {
      const size_t curr = randsize_t_i(start, end, &state);
      assert(start <= curr && curr <= end);
    }
  }

  for (size_t i = 0; i < 10000; i++) {
    const size_t start = i;
    const size_t end = i + 1 + (i % 100);
    const size_t curr = randsize_t_i(start, end, &state);
    assert(start <= curr && curr <= end);
  }

  for (size_t i = 1000; i < 10000; i += 1000) {
    const size_t start = i;
    const size_t end = i * 10;
    for (size_t _ = 0; _ < 10000; _++) {
      const size_t curr = randsize_t_i(start, end, &state);
      assert(start <= curr && curr <= end);
    }
  }
}

void test_random_double() {
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);
  double prev = random_double(&state);
  double prev_prev = random_double(&state);
  for (size_t _ = 0; _ < 10000000; _++) {
    const double curr = random_double(&state);
    assert(0.0 <= curr && curr < 1.0);
    assert(!(memcmp(&curr, &prev, sizeof(double)) == 0 &&
             memcmp(&curr, &prev_prev, sizeof(double)) == 0));
    prev_prev = prev;
    prev = curr;
  }

  size_t bins[10] = {0};

  for (size_t _ = 0; _ < 1000000; _++) {
    const double val = random_double(&state);
    bins[(int)(val * 10)]++;
  }

  for (size_t i = 0; i < 10; i++)
    assert(bins[i] > 95000 && bins[i] < 105000);
}

inline static void fill_array_simple(size_t *array, size_t array_size) {
  for (size_t i = 0; i < array_size; i++)
    array[i] = i;
}

void test_shuffle_array_of_size_t() {
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);

  const size_t N = 250;
  size_t *array = NULL;
  size_t **histogram = NULL;

  size_t size_t_size = sizeof(size_t);
  size_t size_t_alignment = alignof(size_t);

  assert(init_array((void **)&array, N, size_t_size) == ARRAY_OK);
  assert(init_matrix((void ***)&histogram, N, N, size_t_size,
                     size_t_alignment) == 0);

  for (size_t i = 0; i < N; i++)
    for (size_t j = 0; j < N; j++)
      histogram[i][j] = 0;

  const size_t TRIALS = 5000;

  for (size_t i = 0; i < TRIALS; i++) {
    fill_array_simple(array, N);
    assert(shuffle_array_of_size_t(array, N, &state) == 0);

    for (size_t position = 0; position < N; position++) {
      size_t elem = array[position];
      histogram[elem][position]++;
    }
  }

  // H stands for entropy
  const double maximum_H = log2((double)N);
  double total_H = 0.0;

  for (size_t elem_i = 0; elem_i < N; elem_i++) {
    double H_elem = 0.0;
    for (size_t pos_i = 0; pos_i < N; pos_i++) {
      double p_i = (double)histogram[elem_i][pos_i] / TRIALS;
      if (p_i > 0)
        H_elem -= p_i * log2(p_i);
    }
    total_H += H_elem;
  }

  free(array);
  array = NULL;
  free(histogram);
  histogram = NULL;

  double avg_H = total_H / N;
  double tolerance = maximum_H * 0.005;
  //printf("tolerance: %f\n", tolerance);
  //printf("avg_H: %f, maximum_H: %f\n", avg_H, maximum_H);
  //printf("diff: %f\n", maximum_H - avg_H);
  assert(maximum_H - avg_H <= tolerance);

  array = NULL;
}