#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/utils/input_output.h"
#include "../../include/utils/array.h"
#include "../../include/utils/mystring.h"

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

FileWriteStatus write_to_file_in_specific_order(const char *file_name,
                                                const file_line *array_of_lines,
                                                const size_t num_lines,
                                                const size_t *permutation,
                                                const char *mode) {
  if (file_name == NULL)
    return FILE_WRITE_NULL_FILENAME;
  if (mode == NULL)
    return FILE_WRITE_NULL_MODE;
  if (strcmp("w", mode) != 0 && strcmp("a", mode) != 0)
    return FILE_WRITE_ERR_WRONG_MODE;

  bool default_order = permutation == NULL;

  FILE *file = fopen(file_name, mode);

  if (!file)
    return FILE_WRITE_ERR_OPEN_FAILED;

  if (num_lines == 0) {
    fclose(file);
    return FILE_WRITE_SUCCESS;
  }

  assert(array_of_lines != NULL);

  for (size_t i = 0; i < num_lines; i++) {
    size_t k = (default_order) ? i : permutation[i];
    if (fputs(array_of_lines[k].content, file) == EOF) {
      fclose(file);
      return FILE_WRITE_ERR_WHILE_WRITING;
    }
  }
  fclose(file);
  return FILE_WRITE_SUCCESS;
}

FileWriteStatus write_to_file(const char *file_name,
                              const file_line *array_of_lines,
                              const size_t num_lines, const char *mode) {
  return write_to_file_in_specific_order(file_name, array_of_lines, num_lines,
                                         NULL, mode);
}

int str_to_size_t(const char *str, size_t *result) {
  if (str == NULL)
    return 1;
  if (result == NULL)
    return 2;

  errno = 0;
  char *endptr = NULL;
  unsigned long long tmp = strtoull(str, &endptr, 10);

  if (errno == ERANGE)
    return 3;
  if (endptr == str)
    return 4;
  if (*endptr != '\0')
    return 5;

#if SIZE_MAX < ULLONG_MAX
  if (tmp > SIZE_MAX)
    return 6;
#endif

  *result = (size_t)tmp;
  return 0;
}

int str_to_double(const char *str, double *result) {
  if (str == NULL)
    return 1;
  if (result == NULL)
    return 2;

  errno = 0;
  char *endptr = NULL;
  double tmp = strtod(str, &endptr);

  if (errno == ERANGE)
    return 3;
  if (endptr == str)
    return 4;
  if (*endptr != '\0')
    return 5;
  if (isnan(tmp) || isinf(tmp))
    return 7;
  *result = tmp;
  return 0;
}

void get_arguments_for_exec(size_t *generations, size_t *population_size,
                            double *mutation_rate, size_t *n_threads,
                            uint64_t *seed1, uint64_t *seed2,
                            size_t *local_search_iters,
                            char **ptr_to_instance_file_path,
                            char **ptr_to_output_file_path,
                            const char *input_file_path) {
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  assert(read_file(input_file_path, &array_of_lines, &num_of_lines) ==
         FILE_READ_SUCCESS);
  for (size_t i = 0; i < num_of_lines; i += 2) {
    strip_in_place(&array_of_lines[i].content, &array_of_lines[i].length);
    char *label = array_of_lines[i].content;
    strip_in_place(&array_of_lines[i + 1].content,
                   &array_of_lines[i + 1].length);
    char *content = array_of_lines[i + 1].content;
    const size_t content_len = array_of_lines[i + 1].length;

    if (strcmp(label, "INSTANCE_FILE_PATH:") == 0) {
      assert(init_array((void **)ptr_to_instance_file_path, content_len + 1,
                        sizeof(char)) == ARRAY_OK);
      strcpy(*ptr_to_instance_file_path, content);
      printf("INSTANCE_FILE_PATH: %s\n", *ptr_to_instance_file_path);
      continue;
    }

    if (strcmp(label, "OUTPUT_FILE_PATH:") == 0) {
      assert(init_array((void **)ptr_to_output_file_path, content_len + 1,
                        sizeof(char)) == ARRAY_OK);
      strcpy(*ptr_to_output_file_path, content);
      printf("OUTPUT_FILE_PATH: %s\n", *ptr_to_output_file_path);
      continue;
    }

    if (strcmp(label, "SEED1:") == 0) {
      str_to_size_t(content, seed1);
      printf("SEED1: %lu\n", *seed1);
      continue;
    }

    if (strcmp(label, "SEED2:") == 0) {
      str_to_size_t(content, seed2);
      printf("SEED2: %lu\n", *seed2);
      continue;
    }

    if (strcmp(label, "N_GENERATIONS:") == 0) {
      str_to_size_t(content, generations);
      printf("N_GENERATIONS: %zu\n", *generations);
      continue;
    }

    if (strcmp(label, "POPULATION_SIZE:") == 0) {
      str_to_size_t(content, population_size);
      printf("POPULATION_SIZE: %zu\n", *population_size);
      continue;
    }

    if (strcmp(label, "MUTATION_RATE:") == 0) {
      str_to_double(content, mutation_rate);
      printf("MUTATION_RATE: %f\n", *mutation_rate);
      continue;
    }

    if (strcmp(label, "L_SEARCH_ITERATIONS:") == 0) {
      str_to_size_t(content, local_search_iters);
      printf("L_SEARCH_ITERATIONS: %zu\n", *local_search_iters);
      continue;
    }

    if (strcmp(label, "THREADS:") == 0) {
      str_to_size_t(content, n_threads);
      printf("THREADS: %zu\n", *n_threads);
      continue;
    }
  }

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
}