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
  if (columns > SIZE_MAX / type_size || rows > SIZE_MAX / (columns * type_size))
    return 3; // overflow error.

  // memory for the ptrs to each row.

  size_t header_size = rows * sizeof(void *);
  size_t data_size = type_size * rows * columns;
  size_t total_size = header_size + (alignment - 1) + data_size;

  void *memory_block = malloc(total_size);
  if (!memory_block)
    return 4;
  void **matrix_rows = (void **)memory_block;

  // memory for all the data in the matrix.
  void *data_block = memory_block;
  uintptr_t data_start = (uintptr_t)memory_block + header_size;
  if (alignment != 0 && (alignment & (alignment - 1)))
    data_start = (data_start + (alignment - 1)) & ~(alignment - 1);
  else
    data_start = data_start + (alignment - data_start % alignment) % alignment;

  data_block = (void *)data_start;

  for (size_t i = 0; i < rows; i++)
    // cast to char to allow ptr arithmetic in other compilers
    matrix_rows[i] = (char *)data_block + (i * columns * type_size);

  *ptr_to_matrix = matrix_rows;
  return 0;
}