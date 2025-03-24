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

void free_array_of_lines(file_line *array_of_lines, const size_t array_size) {
  if (array_of_lines == NULL)
    return;
  for (size_t i = 0; i < array_size; i++) {
    free(array_of_lines[i].content);
    array_of_lines[i].content = NULL;
    array_of_lines[i].length = 0;
  }
}

void read_file(const char *file_name, file_line **ptr_to_array_of_lines,
               size_t *ptr_to_num_lines) {
  assert(file_name != NULL);
  assert(ptr_to_num_lines != NULL);
  assert(*ptr_to_array_of_lines == NULL);

  FILE *file = fopen(file_name, "r");

  if (!file) {
    perror("Error while opening file in \'read_file\'\n");
    return;
  }

  *ptr_to_num_lines = 0;

  char buffer[256];
  size_t buffer_str_len = 0;
  size_t array_of_lines_capacity = 10;
  size_t previous_str_len = 0;
  size_t str_len = 0;
  bool just_processed_one_line = true;
  file_line *tmp_ptr_to_arr_lines = NULL;

  tmp_ptr_to_arr_lines = malloc(array_of_lines_capacity * sizeof(file_line));
  if (!tmp_ptr_to_arr_lines) {
    perror("Error while allocating memory for ptr_to_array_of_lines in "
           "\'read_file\' at the beginning.\n");
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
               "\'read_file\' while reading file.\n");
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
    str_len++; // adding space for empty (\0) char

    if (buffer_str_len > 0 && buffer[buffer_str_len - 1] == '\n')
      just_processed_one_line = true;

    char *tmp_ptr_to_char =
        realloc((*ptr_to_array_of_lines)[i].content, str_len * sizeof(char));

    if (!tmp_ptr_to_char) {
      perror(
          "Error while reallocating memory for line.content in \'read_file\'.\n");
      fclose(file);
      return;
    }

    (*ptr_to_array_of_lines)[i].content = tmp_ptr_to_char;
    tmp_ptr_to_char = NULL;

    memcpy((*ptr_to_array_of_lines)[i].content + previous_str_len, buffer,
           buffer_str_len);


    (*ptr_to_array_of_lines)[i].content[str_len - 1] = '\0'; // add the empty char
    if (just_processed_one_line)
      *ptr_to_num_lines += 1;
  }

  if (!just_processed_one_line)
    *ptr_to_num_lines += 1;

  if (*ptr_to_num_lines == 0) {
    free(*ptr_to_array_of_lines);
    *ptr_to_array_of_lines = NULL;
    fclose(file);
    return;
  }

  tmp_ptr_to_arr_lines =
      realloc(*ptr_to_array_of_lines, *ptr_to_num_lines * sizeof(file_line));

  if (!tmp_ptr_to_arr_lines) {
    perror("Error while reallocating memory for ptr_to_array_of_lines in "
           "\'read_file\' at the end.\n");
  }
  *ptr_to_array_of_lines = tmp_ptr_to_arr_lines;
  tmp_ptr_to_arr_lines = NULL;

  fclose(file);
}

void write_to_file(const char *file_name, const file_line *array_of_lines,
                   const size_t num_lines, const char *mode) {
  assert(file_name != NULL);
  assert(mode != NULL);

  if (strcmp("w", mode) != 0 && strcmp("a", mode) != 0) {
    char error_msg[100];
    snprintf(error_msg, sizeof(error_msg),
             "Wrong mode given as argument for \'write_to_file\':%s\n", mode);
    perror(error_msg);
    return;
  }

  FILE *file = fopen(file_name, mode);

  if (!file) {
    perror("Error while opening file in \'read_file\'.\n");
    return;
  }

  if (num_lines == 0) {
    fclose(file);
    return;
  }

  assert(array_of_lines != NULL);

  for (size_t i = 0; i < num_lines; i++) {
    if (fputs(array_of_lines[i].content, file) == EOF) {
      char error_msg[100];
      snprintf(error_msg, sizeof(error_msg),
               "Error while trying to write to the file\'%s\'.\n", file_name);
      perror(error_msg);
      return;
    }
  }
  fclose(file);
}