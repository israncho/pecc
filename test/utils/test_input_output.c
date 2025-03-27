#include "../../include/test/utils/test_input_output.h"
#include "../../include/utils/input_output.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_read_file() {
  char *filename = "instances/test/input_output1.txt";
  FileReadStatus read_status = -1;
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  read_status = read_file(filename, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);

  file_line content_of_file1[10] = {{"aaaaaaaaaa\n", 11},
                                    {"\n", 1},
                                    {"bbbbbbbbbb\n", 11},
                                    {"\n", 1},
                                    {"cccccccccc\n", 11},
                                    {"\n", 1},
                                    {"dddddddddd\n", 11},
                                    {"\n", 1},
                                    {"\n", 1},
                                    {"eeeeeeeeee", 10}};

  bool test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                                   content_of_file1, 10);
  assert(test_result);

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  // sub-test 2
  num_of_lines = 0;
  filename = "instances/test/input_output2.txt";
  read_status = read_file(filename, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);

  file_line content_of_file2[35] = {
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
  };

  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              content_of_file2, 35);
  assert(test_result);

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  // sub-test 3
  num_of_lines = 0;
  filename = "instances/test/input_output3.txt";
  read_status = read_file(filename, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);
  file_line content_of_file3[50] = {
      {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
       "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
       "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
       "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n",
       261},
      {"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
       "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
       "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
       "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n",
       261},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1}, {"\n", 1},
      {"\n", 1}, {"\n", 1},
      {"ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
       "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
       "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
       "ccccccccccccccccccccccccccccccccccccccccccccccc\n",
       261}};

  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              content_of_file3, 50);
  assert(test_result);

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  // sub-test 4 - empty file
  num_of_lines = 0;
  filename = "instances/test/input_output4.txt";
  read_status = read_file(filename, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);
  file_line *content_of_file4 = NULL;
  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              content_of_file4, 0);
  assert(test_result);
  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  printf("\t- read_file: PASSED\n");
}

void test_write_to_file() {
  char *filename4 = "instances/test/input_output4.txt";
  bool test_result = false;
  FileReadStatus read_status = -1;

  // sub-test 1
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  file_line content_of_file1[10] = {{"aaaaaaaaaa\n", 11},
                                    {"\n", 1},
                                    {"bbbbbbbbbb\n", 11},
                                    {"\n", 1},
                                    {"cccccccccc\n", 11},
                                    {"\n", 1},
                                    {"dddddddddd\n", 11},
                                    {"\n", 1},
                                    {"\n", 1},
                                    {"eeeeeeeeee", 10}};
  write_to_file(filename4, content_of_file1, 10, "w");

  read_status = read_file(filename4, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);
  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              content_of_file1, 10);
  assert(test_result);
  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  // subtest 2
  num_of_lines = 0;
  file_line content_of_file2[35] = {
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
      {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3}, {"aa\n", 3},
  };
  write_to_file(filename4, content_of_file2, 35, "w");
  read_status = read_file(filename4, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);
  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              content_of_file2, 35);
  assert(test_result);
  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  // subtest 4 - append
  num_of_lines = 0;
  file_line appended_content[5] = {{"abcd\n", 5},
                                   {"abcd\n", 5},
                                   {"abcd\n", 5},
                                   {"abcd\n", 5},
                                   {"abcd\n", 5}};
  write_to_file(filename4, NULL, 0, "w"); // cleaning file
  for (size_t i = 0; i < 5; i++)
    write_to_file(filename4, appended_content, 1, "a");
  read_status = read_file(filename4, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);
  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              appended_content, 5);
  assert(test_result);
  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  // subtest 3 - cleaning file
  file_line *content_of_file4 = NULL;
  num_of_lines = 0;
  write_to_file(filename4, array_of_lines, 0, "w");
  read_status = read_file(filename4, &array_of_lines, &num_of_lines);
  assert(read_status == FILE_READ_SUCCESS);
  test_result = equality_test_for_line_arrays(array_of_lines, num_of_lines,
                                              content_of_file4, 0);
  assert(test_result);
  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;

  printf("\t- write_to_file: PASSED\n");
}

bool equality_test_for_line_arrays(const file_line *array_of_lines1,
                                   const size_t arr1_size,
                                   const file_line *array_of_lines2,
                                   const size_t arr2_size) {
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

void test_input_output() {
  printf("Testing: input_output\n");
  test_read_file();
  test_write_to_file();
}