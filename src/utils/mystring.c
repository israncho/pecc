#include <stdbool.h>
#include <stdio.h>
#include "../../include/utils/mystring.h"
#include "../../include/utils/array.h"

int strip_in_place(char **ptr_to_str, size_t *ptr_to_str_len) {
  if (ptr_to_str == NULL)
    return 1;
  if (ptr_to_str_len == NULL)
    return 2;

  size_t char_size = sizeof(char);

  size_t str_len = *ptr_to_str_len;
  if (str_len == 0)
    return 3;

  size_t start_i = str_len;
  char val = '\0';
  for (size_t i = 0; i < str_len; i++) {
    val = (*ptr_to_str)[i];
    if (val != ' ' && val != '\n' && val != '\t' && val != '\r' &&
        val != '\v' && val != '\f') {
      start_i = i;
      break;
    }
  }

  if (start_i == str_len) {
    // result of stripping is \0
    resize_array((void **)ptr_to_str, 1, char_size);
    (*ptr_to_str)[0] = '\0';
    *ptr_to_str_len = 0;
    return 0;
  }

  // at this point we know the string has at least
  // one non whitespace char

  size_t end_i = str_len - 1;
  val = (*ptr_to_str)[end_i];
  // from what is stated before we know this will stop before consuming the
  // whole array
  while (val == ' ' || val == '\n' || val == '\t' || val == '\r' ||
         val == '\v' || val == '\f')
    val = (*ptr_to_str)[--end_i];

  size_t new_size = end_i - start_i + 1;

  if (new_size == str_len)
    return 0;

  size_t j = 0;
  for (size_t i = start_i; i <= end_i; i++)
    (*ptr_to_str)[j++] = (*ptr_to_str)[i];
  (*ptr_to_str)[j] = '\0';

  resize_array((void **)ptr_to_str, new_size + 1, char_size);
  *ptr_to_str_len = new_size;
  return 0;
}

int strip_to_buffer(const char *str_to_strip,
                    const size_t str_len,
                    char **ptr_to_buffer,
                    size_t *stripped_len,
                    size_t *buffer_capacity) {
  if (str_to_strip == NULL)
    return 1;
  if (ptr_to_buffer == NULL)
    return 2;
  if (*ptr_to_buffer == NULL)
    return 3;
  if (stripped_len == NULL)
    return 4;
  if (buffer_capacity == NULL)
    return 5;
  if (*buffer_capacity == 0)
    return 6;

  size_t char_size = sizeof(char);
  size_t start_i = str_len;
  char val = '\0';
  for (size_t i = 0; i < str_len; i++) {
    val = str_to_strip[i];
    if (val != ' ' && val != '\n' && val != '\t' && val != '\r' &&
        val != '\v' && val != '\f') {
      start_i = i;
      break;
    }
  }

  if (start_i == str_len) {
    // result of stripping is \0
    // no need to resize the storing array
    (*ptr_to_buffer)[0] = '\0';
    *stripped_len = 0;
    return 0;
  }

  // at this point we know the string has at least
  // one non whitespace char

  size_t end_i = str_len - 1;
  val = str_to_strip[end_i];
  // from what is stated before we know this will stop before consuming the
  // whole array
  while (val == ' ' || val == '\n' || val == '\t' || val == '\r' ||
         val == '\v' || val == '\f')
    val = str_to_strip[--end_i];

  size_t new_size = end_i - start_i + 1;

  *stripped_len = new_size;

  // + 1 because of the extra '\0' character
  if (new_size + 1 > *buffer_capacity) {
    *buffer_capacity = new_size + 1;
    if (resize_array((void **)ptr_to_buffer, *buffer_capacity, char_size) !=
        ARRAY_OK)
      return 7;
  }

  char *str_result = *ptr_to_buffer;

  size_t j = 0;
  for (size_t i = start_i; i <= end_i; i++)
    str_result[j++] = str_to_strip[i];
  str_result[j] = '\0';

  return 0;
}

int split(const char *str, const size_t str_len, const char delimiter,
          char ***ptr_to_tokens_array, size_t *tokens_number) {
  if (str == NULL)
    return 1;
  if (str_len == 0) // all well formed strings have at least \0
    return 2;
  if (delimiter == '\0') // not well formed string was given
    return 3;
  if (ptr_to_tokens_array == NULL)
    return 4;
  if (tokens_number == NULL)
    return 5;

  char **matrix_ptrs_to_rows = *ptr_to_tokens_array;
  if (matrix_ptrs_to_rows != NULL)
    return 6;
  size_t start_i = 0, end_i = 0, curr_tokens_number = 0, total_size = 0,
         last_i = str_len - 1;
  bool processing_token = false;

  for (size_t i = 0; i < str_len; i++) {
    char curr_char = str[i];
    if (curr_char != delimiter) {
      if (!processing_token) {
        processing_token = true;
        start_i = i;
        end_i = i;
      }
      end_i++;
    }
    if ((curr_char == delimiter || i == last_i) && processing_token) {
      processing_token = false;
      curr_tokens_number++;
      total_size += end_i - start_i;
    }
  }

  const size_t char_size = sizeof(char);
  char *matrix_data = NULL;
  if (init_array((void **)&matrix_ptrs_to_rows, curr_tokens_number,
                 sizeof(char *)) != ARRAY_OK)
    return 7;
  if (init_array((void **)&matrix_data, total_size + curr_tokens_number,
                 char_size) != ARRAY_OK)
    return 7;

  *tokens_number = curr_tokens_number;
  processing_token = false;
  size_t token_i = 0;
  size_t bytes_used = 0;
  for (size_t i = 0; i < str_len; i++) {
    char curr_char = str[i];
    if (curr_char != delimiter) {
      if (!processing_token) {
        processing_token = true;
        start_i = i;
        end_i = i;
        matrix_ptrs_to_rows[token_i] = matrix_data + bytes_used * char_size;
      }
      matrix_data[bytes_used + end_i - start_i] = curr_char;
      end_i++;
    }
    if ((curr_char == delimiter || i == last_i) && processing_token) {
      matrix_data[bytes_used + end_i - start_i] = '\0';
      processing_token = false;
      token_i++;
      bytes_used += end_i - start_i + 1;
    }
  }
  *ptr_to_tokens_array = matrix_ptrs_to_rows;
  return 0;
}