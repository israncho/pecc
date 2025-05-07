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