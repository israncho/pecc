#include <stddef.h>
#include "../../include/utils/myalgorithms.h"

size_t bisect_left(const void *array,
                   const size_t array_len,
                   const size_t array_entry_byte_size,
                   const void *x,
                   int (*compare)(const void *, const void *)) {
  size_t left = 0;
  size_t right = array_len;
  while (left < right) {
    size_t mid = left + (right - left) / 2;
    const void *elem = (const char *)array + mid * array_entry_byte_size;
    if (compare(elem, x) < 0)  // elem < x
        left = mid + 1;
    else
        right = mid;
  }
  return left;
}

size_t index_of_the_max_val(const void *array,
                            const size_t array_len,
                            const size_t array_entry_byte_size,
                            int (*compare)(const void *, const void *)) {
  size_t index_of_max = 0;
  for (size_t i = 1; i < array_len; i++) {
    const void *curr_max_val = (const char *)array + index_of_max * array_entry_byte_size;
    const void *curr_elem = (const char *)array + i * array_entry_byte_size;
    if (compare(curr_max_val, curr_elem) < 0) // curr_max_val < curr_elem
      index_of_max = i;
  }
  return index_of_max;
}