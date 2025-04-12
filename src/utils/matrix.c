#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils/matrix.h"

int init_matrix(void ***ptr_to_matrix, const size_t rows, const size_t columns,
                const size_t type_size, const size_t alignment) {
  if (ptr_to_matrix == NULL)
    return 1;
  if (*ptr_to_matrix != NULL)
    return 2;
  if (rows == 0 || columns == 0 || type_size == 0)
    return 0; // empty matrix needs no space
  if (alignment == 0)
    return 3;
  if (columns > SIZE_MAX / type_size || rows > SIZE_MAX / (columns * type_size))
    return 4; // overflow error.

  size_t header_size = rows * sizeof(void *);
  size_t data_size = type_size * rows * columns;

  if (header_size > SIZE_MAX - (alignment - 1) ||
      header_size + (alignment - 1) > SIZE_MAX - data_size) {
    return 4; // Overflow
  }

  size_t total_size = header_size + (alignment - 1) + data_size;

  void *memory_block = malloc(total_size);
  if (!memory_block)
    return 5;
  void **matrix_rows = (void **)memory_block;

  void *data_block = memory_block;
  uintptr_t data_start = (uintptr_t)memory_block + header_size;
  if (!(alignment & (alignment - 1)))
    data_start = (data_start + (alignment - 1)) & ~(alignment - 1);
  else {
    uintptr_t offset = alignment - (data_start % alignment);
    data_start += (offset == alignment) ? 0 : offset;
  }

  data_block = (void *)data_start;

  for (size_t i = 0; i < rows; i++)
    // cast to char to allow ptr arithmetic in other compilers
    matrix_rows[i] = (char *)data_block + (i * columns * type_size);

  *ptr_to_matrix = matrix_rows;
  return 0;
}