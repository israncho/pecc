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
      start = size_t_array[j] + 1;
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
      start = float_array[j] + 1.0;
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