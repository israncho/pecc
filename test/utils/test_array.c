#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/utils/mytime.h"
#include "../../include/test/utils/test_array.h"
#include "../../include/utils/array.h"

void test_array() {
  printf("Testing: array\n");

  double start = get_wall_time();
  test_setup_array_from_prealloc_mem();
  double elapsed_time = get_wall_time() - start;
  printf("\t- setup_array_from_prealloc_mem: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_reverse_segment_size_t_arr();
  elapsed_time = get_wall_time() - start;
  printf("\t- reverse_segment_size_t_arr: PASSED [%.6f secs]\n", elapsed_time);
}

void test_setup_array_from_prealloc_mem() {
  const size_t int_size = sizeof(int);
  const size_t double_size = sizeof(double);
  const size_t size_in_bytes = 8 * 512;
  void *mem = malloc(size_in_bytes);
  void *free_memory = mem;

  size_t mem_capacity = size_in_bytes;
  // printf("mem_capacity: %zu\n", mem_capacity);

  // adding and testing for int

  int *int_array = NULL;
  const size_t int_array_size = 10;
  int target[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  assert(setup_array_from_prealloc_mem(&free_memory, &mem_capacity,
                                       (void **)&int_array, int_array_size,
                                       int_size, alignof(int)) == ARRAY_OK);
  size_t expected_remainging = size_in_bytes - int_array_size * int_size;
  assert(mem_capacity <= expected_remainging);

  memcpy(int_array, target, int_array_size * int_size);
  for (size_t i = 0; i < int_array_size; i++)
    assert(int_array[i] == target[i]);

  // adding and testing for double

  double *double_array = NULL;
  const size_t double_array_size = 7;
  double double_target[7] = {
      0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,
  };
  assert(setup_array_from_prealloc_mem(
             &free_memory, &mem_capacity, (void **)&double_array,
             double_array_size, double_size, alignof(int)) == ARRAY_OK);
  expected_remainging -= double_array_size * double_size;
  assert(mem_capacity <= expected_remainging);
  memcpy(double_array, double_target, double_array_size * double_size);
  for (size_t i = 0; i < double_array_size; i++)
    assert(double_array[i] == double_target[i]);

  // adding and testing for struct

  typedef struct {
    int id;
    double value;
    char name[5];
  } TestStruct;

  const size_t struct_size = sizeof(TestStruct);
  //printf("struct_size: %zu\n", struct_size);
  //printf("struct_align: %zu\n", alignof(TestStruct));
  const size_t struct_array_size = 5;
  TestStruct *struct_array = NULL;
  TestStruct struct_target[5] = {{1, 1.1, "item1"},
                                 {2, 2.2, "item2"},
                                 {3, 3.3, "item3"},
                                 {4, 4.4, "item4"},
                                 {5, 5.5, "item5"}};
  assert(setup_array_from_prealloc_mem(
             &free_memory, &mem_capacity, (void **)&struct_array,
             struct_array_size, struct_size, alignof(TestStruct)) == ARRAY_OK);
  expected_remainging -= struct_size * struct_array_size;
  assert(mem_capacity <= expected_remainging);
  memcpy(struct_array, struct_target, struct_size * struct_array_size);
  for (size_t i = 0; i < struct_array_size; i++) {
    assert(struct_array[i].id == struct_target[i].id);
    assert(struct_array[i].value == struct_target[i].value);
    assert(strcmp(struct_array[i].name, struct_target[i].name) == 0);
  }

  // adding test for char

  const size_t char_size = sizeof(char);
  const size_t char_array_size = 10;
  char *char_array = NULL;
  char char_target[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', '\0'};
  assert(setup_array_from_prealloc_mem(
             &free_memory, &mem_capacity, (void **)&char_array,
             char_array_size, char_size, alignof(char)) == ARRAY_OK);
  expected_remainging -= char_size * char_array_size;
  assert(mem_capacity <= expected_remainging);
  memcpy(char_array, char_target, char_size * char_array_size);
  for (size_t i = 0; i < char_array_size; i++)
    assert(char_array[i] == char_target[i]);


  // final check

  for (size_t i = 0; i < int_array_size; i++)
    assert(int_array[i] == target[i]);

  for (size_t i = 0; i < double_array_size; i++)
    assert(double_array[i] == double_target[i]);

  for (size_t i = 0; i < struct_array_size; i++) {
    assert(struct_array[i].id == struct_target[i].id);
    assert(struct_array[i].value == struct_target[i].value);
    assert(strcmp(struct_array[i].name, struct_target[i].name) == 0);
  }

  for (size_t i = 0; i < char_array_size; i++)
    assert(char_array[i] == char_target[i]);

  free(mem);
}

static inline void fill_array_as_simple_seq(size_t *array,
                                            const size_t array_len) {
  for (size_t i = 0; i < array_len; i++)
    array[i] = i;
}

static inline bool same_arrays(size_t *array1, size_t *array2,
                               const size_t array_len) {
  for (size_t i = 0; i < array_len; i++)
    if (array1[i] != array2[i])
      return false;
  return true;
}

void test_reverse_segment_size_t_arr() {
  const size_t array_len = 1000;
  size_t unchanged[array_len];
  fill_array_as_simple_seq(unchanged, array_len);
  size_t arr[array_len];
  fill_array_as_simple_seq(arr, array_len);

  for (size_t i = 0; i < array_len; i++) {
    for (size_t j = i + 1; j < array_len; j++) { 
      reverse_segment_size_t_arr(arr, i, j);
      size_t tmp_i = i;
      size_t tmp_j = j;
      while (tmp_i < tmp_j) {
        assert(unchanged[tmp_i] == arr[tmp_j]);
        tmp_i++;
        tmp_j--;
      }
      reverse_segment_size_t_arr(arr, i, j);
      assert(same_arrays(arr, unchanged, array_len));
    }
  }
}