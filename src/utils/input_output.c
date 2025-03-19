#include "../../include/utils/input_output.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_file_line(file_line *file_line_to_free) {
  if (file_line_to_free == NULL)
    return;
  free(file_line_to_free->content);
  file_line_to_free->content = NULL;
  file_line_to_free->length = 0;
}

void read_file(char *file_name, file_line **ptr_to_array_of_lines,
               size_t *ptr_to_num_lines) {
  assert(file_name != NULL);
  assert(ptr_to_num_lines != NULL);
  assert(*ptr_to_array_of_lines == NULL);

  FILE *file = fopen(file_name, "r");

  if (!file) {
    perror("Error while opening file in \'read_file\'");
    return;
  }

  *ptr_to_num_lines = 0;

  char buffer[256];
  size_t buffer_str_len = 0;
  size_t array_of_lines_capacity = 10;
  size_t previous_str_len = 0;
  size_t str_len = 0;
  bool just_processed_one_line = false;
  file_line *tmp_ptr_to_arr_lines = NULL;

  tmp_ptr_to_arr_lines = malloc(array_of_lines_capacity * sizeof(file_line));
  if (!tmp_ptr_to_arr_lines) {
    perror("Error while reallocating memory for ptr_to_array_of_lines in "
           "\'read_file\' at the beginning.");
    fclose(file);
    return;
  }
  *ptr_to_array_of_lines = tmp_ptr_to_arr_lines;
  tmp_ptr_to_arr_lines = NULL;

  while (fgets(buffer, sizeof(buffer), file)) {
    size_t i = *ptr_to_num_lines;

    if (i + 1 >= array_of_lines_capacity) { // out of space in the array of lines
      array_of_lines_capacity *= 2;
      tmp_ptr_to_arr_lines = realloc(
          *ptr_to_array_of_lines, array_of_lines_capacity * sizeof(file_line));
      if (!tmp_ptr_to_arr_lines) {
        perror("Error while reallocating memory for ptr_to_array_of_lines in "
               "\'read_file\' while reading file.");
        fclose(file);
        return;
      }
      *ptr_to_array_of_lines = tmp_ptr_to_arr_lines;
      tmp_ptr_to_arr_lines = NULL;
    }

    if (just_processed_one_line) {
      (*ptr_to_array_of_lines)[i].content = NULL;
      (*ptr_to_array_of_lines)[i].length = 0;
      previous_str_len = 0;
      str_len = 0;
      just_processed_one_line = false;
    }

    buffer_str_len = strlen(buffer);
    previous_str_len = (*ptr_to_array_of_lines)[i].length;
    str_len = previous_str_len + buffer_str_len;
    (*ptr_to_array_of_lines)[i].length = str_len;

    if (buffer_str_len > 0 && buffer[buffer_str_len - 1] == '\n') {
      just_processed_one_line = true;
      str_len++; // reaching end of line, adding space for empty (\0) char
    }

    char *tmp =
        realloc((*ptr_to_array_of_lines)[i].content, str_len * sizeof(char));

    if (!tmp) {
      perror(
          "Error while reallocating memory for line.content in \'read_file\'.");
      fclose(file);
      return;
    }

    (*ptr_to_array_of_lines)[i].content = tmp;

    memcpy((*ptr_to_array_of_lines)[i].content + previous_str_len, buffer,
           buffer_str_len);

    if (just_processed_one_line) {
      (*ptr_to_array_of_lines)[i].content[str_len - 1] =
          '\0'; // add the empty char
      *ptr_to_num_lines += 1;
    }
  }

  if (!just_processed_one_line) {
    char *tmp = realloc((*ptr_to_array_of_lines)[*ptr_to_num_lines - 1].content,
                        str_len + 1);
    if (!tmp) {
      perror("Error while reallocating memory for line.content in "
             "\'read_file\' at the end.");
      fclose(file);
      return;
    }
    (*ptr_to_array_of_lines)[*ptr_to_num_lines - 1].content = tmp;
    (*ptr_to_array_of_lines)[*ptr_to_num_lines - 1].content[str_len] = '\0';
    *ptr_to_num_lines += 1;
  }

  tmp_ptr_to_arr_lines =
      realloc(*ptr_to_array_of_lines, *ptr_to_num_lines * sizeof(file_line));

  if (!tmp_ptr_to_arr_lines) {
    perror("Error while reallocating memory for ptr_to_array_of_lines in "
           "\'read_file\' at the end");
  }
  *ptr_to_array_of_lines = tmp_ptr_to_arr_lines;
  tmp_ptr_to_arr_lines = NULL;

  fclose(file);
}