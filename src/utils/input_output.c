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
  bool no_new_line_at_end = false;

  while (fgets(buffer, sizeof(buffer), file)) {
    no_new_line_at_end = false;
    buffer_str_len = strlen(buffer);

    if (buffer_str_len > 0 && buffer[buffer_str_len - 1] == '\n') {
      *ptr_to_num_lines += 1;
    } else {
      no_new_line_at_end = true;
    }
  }

  if (no_new_line_at_end) {
    *ptr_to_num_lines += 1;
  }

  *ptr_to_array_of_lines =
      (file_line *)malloc(sizeof(file_line) * (*ptr_to_num_lines));

  if (!*ptr_to_array_of_lines) {
    perror("Error allocating memory for array_of_lines in \'read_file\'");
    fclose(file);
    return;
  }

  rewind(file);

  for (size_t i = 0; i < *ptr_to_num_lines; i++) {
    (*ptr_to_array_of_lines)[i].content = NULL;
    (*ptr_to_array_of_lines)[i].length = 0;
    size_t previous_str_len = 0;
    size_t str_len = 0;

    do {
      if (fgets(buffer, sizeof(buffer), file) ==
          NULL) // read and check if something was read
        break;  // if file consumed then stop

      buffer_str_len = strlen(buffer);
      previous_str_len = (*ptr_to_array_of_lines)[i].length;
      str_len = buffer_str_len + previous_str_len;
      (*ptr_to_array_of_lines)[i].length = str_len;

      char *temp =
          realloc((*ptr_to_array_of_lines)[i].content, sizeof(char) * str_len);

      if (!temp) {
        perror(
            "Error while allocating memory for line.content in \'read_file\'");
        fclose(file);
        return;
      }

      (*ptr_to_array_of_lines)[i].content = temp;

      memcpy((*ptr_to_array_of_lines)[i].content + previous_str_len, buffer,
             buffer_str_len);
    } while (buffer_str_len > 0 && buffer[buffer_str_len - 1] != '\n');

    if ((*ptr_to_array_of_lines)[i].content[str_len - 1] == '\n') {
      (*ptr_to_array_of_lines)[i].content[str_len - 1] = '\0';
      (*ptr_to_array_of_lines)[i].length--;
    }
  }

  fclose(file);
}