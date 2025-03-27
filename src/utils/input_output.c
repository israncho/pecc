#include "../../include/utils/input_output.h"
#include "../../include/utils/array.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_file_line(file_line **ptr_to_file_line_to_free) {
  if (*ptr_to_file_line_to_free == NULL)
    return;
  free((*ptr_to_file_line_to_free)->content);
  (*ptr_to_file_line_to_free)->content = NULL;
  (*ptr_to_file_line_to_free)->length = 0;
  *ptr_to_file_line_to_free = NULL;
}

void free_lines_array_content(file_line *array_of_lines,
                              const size_t array_size) {
  if (array_of_lines == NULL)
    return;
  for (size_t i = 0; i < array_size; i++) {
    free(array_of_lines[i].content);
    array_of_lines[i].content = NULL;
    array_of_lines[i].length = 0;
  }
}

static int cleanup_resources(FILE *file, file_line **ptr_to_array_of_lines,
                             const size_t array_size_in_use,
                             const int exit_code) {
  if (*ptr_to_array_of_lines != NULL) {
    if (array_size_in_use > 0)
      free_lines_array_content(*ptr_to_array_of_lines, array_size_in_use);
    free(*ptr_to_array_of_lines);
    *ptr_to_array_of_lines = NULL;
  }
  if (file)
    fclose(file);
  return exit_code;
}

FileReadStatus read_file(const char *file_name,
                         file_line **ptr_to_array_of_lines,
                         size_t *ptr_to_num_lines) {
  if (file_name == NULL)
    return FILE_READ_NULL_FILENAME;
  if (ptr_to_num_lines == NULL)
    return FILE_READ_NULL_NUM_LINES_PTR;
  if (ptr_to_array_of_lines == NULL)
    return FILE_READ_NULL_LINES_ARRAY_PTR;
  if (*ptr_to_array_of_lines != NULL)
    return FILE_READ_LINES_ARRAY_IN_USE;

  FILE *file = fopen(file_name, "r");

  if (!file)
    return FILE_READ_ERR_OPEN_FAILED;

  *ptr_to_num_lines = 0;

  char buffer[256];
  const size_t file_line_size = sizeof(file_line);
  const size_t char_size = sizeof(char);
  size_t buffer_str_len = 0;
  size_t array_of_lines_capacity = 10;
  size_t previous_str_len = 0;
  size_t str_len = 0;
  bool just_processed_one_line = true;

  if (init_array((void **)ptr_to_array_of_lines, array_of_lines_capacity,
                 file_line_size) != ARRAY_OK)
    return cleanup_resources(file, ptr_to_array_of_lines, 0,
                             FILE_READ_MEMORY_ERROR);

  while (fgets(buffer, sizeof(buffer), file)) {
    size_t i = *ptr_to_num_lines;

    if (i == array_of_lines_capacity) {
      // ran out of space in the array of lines, increment needed.
      array_of_lines_capacity *= 2;
      if (resize_array((void **)ptr_to_array_of_lines, array_of_lines_capacity,
                       file_line_size) != ARRAY_OK)
        return cleanup_resources(file, ptr_to_array_of_lines, i,
                                 FILE_READ_MEMORY_ERROR);
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

    if (resize_array((void **)&((*ptr_to_array_of_lines)[i].content), str_len,
                     char_size) != ARRAY_OK)
      return cleanup_resources(file, ptr_to_array_of_lines, i,
                               FILE_READ_MEMORY_ERROR);

    memcpy((*ptr_to_array_of_lines)[i].content + previous_str_len * char_size,
           buffer, buffer_str_len);

    // add the empty char
    (*ptr_to_array_of_lines)[i].content[str_len - 1] = '\0';
    if (just_processed_one_line)
      *ptr_to_num_lines += 1;
  }

  if (!just_processed_one_line)
    *ptr_to_num_lines += 1;

  if (*ptr_to_num_lines == 0)
    return cleanup_resources(file, ptr_to_array_of_lines, 0, FILE_READ_SUCCESS);

  if (resize_array((void **)ptr_to_array_of_lines, *ptr_to_num_lines,
                   file_line_size) != ARRAY_OK)
    return cleanup_resources(file, ptr_to_array_of_lines, *ptr_to_num_lines, 6);

  fclose(file);
  return FILE_READ_SUCCESS;
}

FileWriteStatus write_to_file(const char *file_name, const file_line *array_of_lines,
                   const size_t num_lines, const char *mode) {
  if (file_name == NULL)
    return FILE_WRITE_NULL_FILENAME;
  if (mode == NULL)
    return FILE_WRITE_NULL_MODE;
  if (strcmp("w", mode) != 0 && strcmp("a", mode) != 0)
    return FILE_WRITE_ERR_WRONG_MODE;

  FILE *file = fopen(file_name, mode);

  if (!file)
    return FILE_WRITE_ERR_OPEN_FAILED;

  if (num_lines == 0) {
    fclose(file);
    return FILE_WRITE_SUCCESS;
  }

  assert(array_of_lines != NULL);

  for (size_t i = 0; i < num_lines; i++) {
    if (fputs(array_of_lines[i].content, file) == EOF) {
      fclose(file);
      return FILE_WRITE_ERR_WHILE_WRITING;
    }
  }
  fclose(file);
  return FILE_WRITE_SUCCESS;
}