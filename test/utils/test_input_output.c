#include "../../include/test/utils/test_input_output.h"
#include "../../include/utils/input_output.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
                         {"eeeeeeeeee", 10}
                    };

  assert(*ptr_to_num_of_lines == 10);
  for (size_t i = 0; i < *ptr_to_num_of_lines; i++) {
    assert(file1[i].length == array_of_lines[i].length);
    assert(array_of_lines[i].content[array_of_lines[i].length] == '\0');
    assert(file1[i].content[file1[i].length] == '\0');
    assert(strcmp(file1[i].content, array_of_lines[i].content) == 0);
    free_file_line(&array_of_lines[i]);
  }
  free(array_of_lines);
  free(ptr_to_num_of_lines);

  printf("- read_file: PASSED\n");
}