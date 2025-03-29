#include "../../include/utils/mystring.h"
#include "../../include/utils/array.h"

int strip(char **ptr_to_str, size_t *ptr_to_str_size) {
  if (ptr_to_str == NULL)
    return 1;
  if (ptr_to_str_size == NULL)
    return 2;

  size_t char_size = sizeof(char);

  size_t str_size = *ptr_to_str_size;
  if (str_size == 0)
    return 3;

  size_t start_i = str_size;
  char val = '\0';
  for (size_t i = 0; i < str_size; i++) {
    val = (*ptr_to_str)[i];
    if (val != ' ' && val != '\n' && val != '\t' && val != '\r' &&
        val != '\v' && val != '\f') {
      start_i = i;
      break;
    }
  }

  if (start_i == str_size) {
    // result of stripping is \0
    resize_array((void **)ptr_to_str, 1, char_size);
    (*ptr_to_str)[0] = '\0';
    *ptr_to_str_size = 0;
    return 0;
  }

  // at this point we know the string has at least
  // one non whitespace char

  size_t end_i = str_size - 1;
  val = (*ptr_to_str)[end_i];
  // from what is stated before we know this will stop before consuming the
  // whole array
  while (val == ' ' || val == '\n' || val == '\t' || val == '\r' ||
         val == '\v' || val == '\f')
    val = (*ptr_to_str)[--end_i];

  size_t new_size = end_i - start_i + 1;

  if (new_size == str_size)
    return 0;

  size_t j = 0;
  for (size_t i = start_i; i <= end_i; i++)
    (*ptr_to_str)[j++] = (*ptr_to_str)[i];
  (*ptr_to_str)[j] = '\0';

  resize_array((void **)ptr_to_str, new_size + 1, char_size);
  *ptr_to_str_size = new_size;
  return 0;
}