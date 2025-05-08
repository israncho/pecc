#include <assert.h>
#include <stdalign.h>
#include <stdio.h>
#include "../../include/test/utils/test_myalgorithms.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myalgorithms.h"
#include "../../include/utils/myrandom.h"

void test_myalgorithms() {
  printf("Testing: myalgorithms\n");
  test_bisect_left();
  printf("\t- test_bisect_left: PASSED\n");
  test_index_of_the_max_val();
  printf("\t- index_of_the_max_val: PASSED\n");
}

void test_bisect_left() {
  const size_t size_t_array_max_size = 500;
  const size_t float_array_max_size = 500;
  const size_t total_size =
      size_t_array_max_size * sizeof(size_t) + alignof(size_t) +
      float_array_max_size * sizeof(float) + alignof(float);
  void *mem = NULL;
  assert(init_array(&mem, total_size, 1) == ARRAY_OK);

  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);

  for (size_t i = 1; i <= 500; i++) {
    void *mem_ = mem;
    size_t capacity = total_size;
    size_t *size_t_array = NULL;
    const size_t array_size = randsize_t_i(1, i, &state);
    assert(setup_array_from_prealloc_mem(
               &mem_, &capacity, (void **)&size_t_array, array_size,
               sizeof(size_t), alignof(size_t)) == ARRAY_OK);

    size_t start = 10;
    for (size_t j = 0; j < array_size; j++) {
      size_t_array[j] = randsize_t_i(start, start + 10, &state);
      start = size_t_array[j] + randsize_t_i(1, 50, &state);
    }

    const size_t index = randsize_t_i(0, array_size - 1, &state);
    size_t elem = size_t_array[index];
    assert(bisect_left(size_t_array, array_size, sizeof(size_t), &elem,
                       compare_size_t) == index);

    elem = 0;
    assert(bisect_left(size_t_array, array_size, sizeof(size_t), &elem,
                       compare_size_t) == 0);

    elem = size_t_array[array_size - 1] + 10;
    assert(bisect_left(size_t_array, array_size, sizeof(size_t), &elem,
                       compare_size_t) == array_size);
  }

  for (size_t i = 1; i <= 500; i++) {
    void *mem_ = mem;
    size_t capacity = total_size;
    float *float_array = NULL;
    const size_t array_size = randsize_t_i(1, i, &state);
    assert(setup_array_from_prealloc_mem(
               &mem_, &capacity, (void **)&float_array, array_size,
               sizeof(float), alignof(float)) == ARRAY_OK);

    size_t start = 0;
    for (size_t j = 0; j < array_size; j++) {
      float_array[j] = randsize_t_i(start, start + 10, &state) * 1.0;
      start = float_array[j] + randsize_t_i(1, 50, &state) * 1.0;
    }

    const size_t index = randsize_t_i(0, array_size - 1, &state);
    float elem = float_array[index];
    assert(bisect_left(float_array, array_size, sizeof(float), &elem,
                       compare_float) == index);
    elem = -1.0;
    assert(bisect_left(float_array, array_size, sizeof(float), &elem,
                       compare_float) == 0);

    elem = float_array[array_size - 1] + 10.0;
    assert(bisect_left(float_array, array_size, sizeof(float), &elem,
                       compare_float) == array_size);
  }
  free(mem);
}

void test_index_of_the_max_val() {
  const size_t size_t_array_len = 100;
  const size_t total_size = size_t_array_len * sizeof(size_t) + alignof(size_t);
  void *mem = NULL;
  assert(init_array(&mem, total_size, 1) == ARRAY_OK);

  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);
  for (size_t _ = 0; _ < 500; _++) {
    size_t *size_t_array = NULL;
    void *mem_ = mem;
    size_t mem_capacity = total_size;
    assert(setup_array_from_prealloc_mem(
               &mem_, &mem_capacity, (void **)&size_t_array, size_t_array_len,
               sizeof(size_t), alignof(size_t)) == ARRAY_OK);

    size_t aux = 0;
    for (size_t i = 0; i < size_t_array_len; i++) {
      size_t_array[i] = randsize_t_i(aux, aux + 10, &state);
      aux = size_t_array[i] + randsize_t_i(1, 50, &state);
    }

    const size_t max_val = size_t_array[size_t_array_len - 1];
    shuffle_array_of_size_t(size_t_array, size_t_array_len, &state);
    const size_t index_of_max = index_of_the_max_val(
        size_t_array, size_t_array_len, sizeof(size_t), compare_size_t);
    assert(max_val == size_t_array[index_of_max]);
    size_t_array[0] = max_val + 10;
    assert(index_of_the_max_val(size_t_array, size_t_array_len, sizeof(size_t),
                                compare_size_t) == 0);
    size_t_array[size_t_array_len - 1] = max_val + 20;
    assert(index_of_the_max_val(size_t_array, size_t_array_len, sizeof(size_t),
                                compare_size_t) == size_t_array_len - 1);
  }

  free(mem);
}