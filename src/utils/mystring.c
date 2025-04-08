#include <stdbool.h>
#include <stdio.h>
#include "../../include/utils/mystring.h"
#include "../../include/utils/array.h"

int strip_in_place(char **ptr_to_str, size_t *ptr_to_str_len) {
  if (ptr_to_str == NULL)
    return 1;
  char *str = *ptr_to_str;
  if (str == NULL)
    return 2;

  if (ptr_to_str_len == NULL)
    return 3;

  size_t str_len = *ptr_to_str_len;
  if (str_len == 0)
    return 0;

  size_t start_i = str_len;
  char val = '\0';
  for (size_t i = 0; i < str_len; i++) {
    val = str[i];
    if (val != ' ' && val != '\n' && val != '\t' && val != '\r' &&
        val != '\v' && val != '\f') {
      start_i = i;
      break;
    }
  }

  if (start_i == str_len) {
    // result of stripping is \0
    str[0] = '\0';
    *ptr_to_str_len = 0;
    return 0;
  }

  // at this point we know the string has at least
  // one non whitespace char

  size_t end_i = str_len - 1;
  val = str[end_i];
  // from what is stated before we know this will stop before consuming the
  // whole array
  while (val == ' ' || val == '\n' || val == '\t' || val == '\r' ||
         val == '\v' || val == '\f')
    val = str[--end_i];

  size_t new_size = end_i - start_i + 1;

  if (new_size == str_len)
    return 0;

  size_t j = 0;
  for (size_t i = start_i; i <= end_i; i++)
    str[j++] = str[i];
  str[j] = '\0';

  *ptr_to_str_len = new_size;
  return 0;
}

int strip_to_buffer(const char *str_to_strip, const size_t str_len,
                    char **ptr_to_buffer, size_t *stripped_len,
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

int split_to_buffer(const char *str, const size_t str_len, const char delimiter,
                    char ***ptr_to_tokens_array,
                    size_t *ptr_to_tokens_array_capacity,
                    size_t *ptr_to_num_of_tokens, char **ptr_to_buffer,
                    size_t *ptr_to_buffer_capacity) {
  if (str == NULL)
    return 1;
  if (str_len == 0) // the result would be an empty array
    return 2;
  if (delimiter == '\0') // wrong delimiter
    return 3;
  if (ptr_to_tokens_array == NULL)
    return 4;
  char **tokens_array = *ptr_to_tokens_array;
  if (tokens_array == NULL)
    return 5;
  if (ptr_to_tokens_array_capacity == NULL)
    return 6;
  if (ptr_to_num_of_tokens == NULL)
    return 7;
  if (ptr_to_buffer == NULL)
    return 8;
  char *buffer = *ptr_to_buffer;
  if (buffer == NULL)
    return 9;
  if (ptr_to_buffer_capacity == NULL)
    return 10;
  if (*ptr_to_buffer_capacity == 0)
    return 11;

  size_t start_i = 0, end_i = 0, num_of_tokens = 0, total_size = 0,
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
      num_of_tokens++;
      total_size += end_i - start_i;
    }
  }

  if (num_of_tokens > *ptr_to_tokens_array_capacity) {
    if (resize_array((void **)&tokens_array, num_of_tokens, sizeof(char *)) !=
        ARRAY_OK)
      return 12;
    *ptr_to_tokens_array = tokens_array; // after resize the addr may change
    *ptr_to_tokens_array_capacity = num_of_tokens;
  }

  *ptr_to_num_of_tokens = num_of_tokens;

  // considering the '\0' char at the end of each token
  const size_t buffer_capacity_needed = total_size + num_of_tokens;
  const size_t char_size = sizeof(char);

  if (buffer_capacity_needed > *ptr_to_buffer_capacity) {
    if (resize_array((void **)&buffer, buffer_capacity_needed, char_size) !=
        ARRAY_OK)
      return 13;
    *ptr_to_buffer = buffer; // after resize the addr may change
    *ptr_to_buffer_capacity = buffer_capacity_needed;
  }

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
        tokens_array[token_i] = buffer + bytes_used * char_size;
      }
      buffer[bytes_used + end_i - start_i] = curr_char;
      end_i++;
    }
    if ((curr_char == delimiter || i == last_i) && processing_token) {
      buffer[bytes_used + end_i - start_i] = '\0';
      processing_token = false;
      token_i++;
      bytes_used += end_i - start_i + 1;
    }
  }

  return 0;
}

int split_in_place(char **ptr_to_str, const size_t str_len,
                   const char delimiter, char ***ptr_to_tokens_array,
                   size_t *ptr_to_tokens_array_capacity, size_t *ptr_to_number_of_tokens) {
  if (ptr_to_str == NULL)
    return 1;
  char *str = *ptr_to_str;
  if (str == NULL)
    return 2;
  if (delimiter == '\0')
    return 3;
  if (ptr_to_tokens_array == NULL)
    return 4;
  char **tokens_array = *ptr_to_tokens_array;
  if (tokens_array == NULL)
    return 5;
  if (ptr_to_tokens_array_capacity == NULL)
    return 6;
  if (ptr_to_number_of_tokens == NULL)
    return 7;

  size_t tokens_array_capacity = *ptr_to_tokens_array_capacity,
         token_i = 0;
  bool processing_token = false;

  for (size_t i = 0; i < str_len; i++) {
    if (token_i == tokens_array_capacity) {
      tokens_array_capacity *= 2;
      if (resize_array((void **)&tokens_array, tokens_array_capacity,
                       sizeof(char *)) != ARRAY_OK)
        return 8;
      *ptr_to_tokens_array_capacity = tokens_array_capacity;
      *ptr_to_tokens_array = tokens_array;
    }
    char curr_char = str[i];
    if (curr_char != delimiter)
      if (!processing_token) {
        processing_token = true;
        tokens_array[token_i++] = &(str[i]);
      }
    if (curr_char == delimiter && processing_token) {
      processing_token = false;
      str[i] = '\0';
    }
  }
  *ptr_to_number_of_tokens = token_i;
  return 0;
}