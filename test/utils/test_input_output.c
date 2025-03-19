#include "../../include/test/utils/test_input_output.h"
#include "../../include/utils/input_output.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_read_file() {
  char *filename = "instances/test/input_output1.txt";

  file_line *array_of_lines = NULL;
  size_t *ptr_to_num_of_lines = (size_t *)malloc(sizeof(size_t));
  read_file(filename, &array_of_lines, ptr_to_num_of_lines);

  file_line file1[10] = {{"aaaaaaaaaa\n", 11},
                         {"\n", 1},
                         {"bbbbbbbbbb\n", 11},
                         {"\n", 1},
                         {"cccccccccc\n", 11},
                         {"\n", 1},
                         {"dddddddddd\n", 11},
                         {"\n", 1},
                         {"\n", 1},
                         {"eeeeeeeeee", 10}};

  bool test_result = equality_test_for_line_arrays(
      array_of_lines, *ptr_to_num_of_lines, file1, 10);
  assert(test_result);

  for (size_t i = 0; i < *ptr_to_num_of_lines; i++)
    free_file_line(&array_of_lines[i]);

  free(array_of_lines);
  array_of_lines = NULL;
  free(ptr_to_num_of_lines);
  ptr_to_num_of_lines = NULL;
  printf("- read_file: PASSED\n");
}

bool equality_test_for_line_arrays(file_line *array_of_lines1, size_t arr1_size,
                                   file_line *array_of_lines2,
                                   size_t arr2_size) {
  if (arr1_size != arr2_size) {
    printf("arr1_size: %zu != arr2_size %zu\n", arr1_size, arr2_size);
    return false;
  }

  for (size_t i = 0; i < arr1_size; i++) {
    if (array_of_lines1[i].length != array_of_lines2[i].length) {
      printf("array_of_lines1[%zu].length: %zu != array_of_lines2[%zu].length: "
             "%zu\n",
             i, array_of_lines1[i].length, i, array_of_lines2[i].length);
      return false;
    }
    if (array_of_lines1[i].content[array_of_lines1[i].length] != '\0') {
      printf("array_of_lines1[%zu].content[arr1_size] != \'\\0\'\n", i);
      return false;
    }
    if (array_of_lines2[i].content[array_of_lines2[i].length] != '\0') {
      printf("array_of_lines2[%zu].content[arr1_size] != \'\\0\'\n", i);
      return false;
    }
    if (strcmp(array_of_lines1[i].content, array_of_lines2[i].content) != 0) {
      printf("%s != %s", array_of_lines1[i].content,
             array_of_lines2[i].content);
      return false;
    }
  }

  return true;
}