#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/utils/matrix.h"

int init_matrix(void ***ptr_to_matrix, const size_t rows, const size_t columns,
                const size_t type_size) {
  if (ptr_to_matrix == NULL)
    return 1;
  if (*ptr_to_matrix != NULL)
    return 2;
  if (rows == 0 || columns == 0 || type_size == 0)
    return 0; // empty matrix needs no space
  if (columns > SIZE_MAX / type_size || rows > SIZE_MAX / (columns * type_size))
    return 3; // overflow error.

  // memory for the ptrs to each row.
  void **matrix_rows = malloc(sizeof(void *) * rows);
  if (!matrix_rows)
    return 4;

  // memory for all the data in the matrix.
  void *data_block = malloc(type_size * rows * columns);
  if (!data_block) {
    free(matrix_rows);
    return 4;
  }

  for (size_t i = 0; i < rows; i++)
    // cast to char to allow ptr arithmetic in other compilers
    matrix_rows[i] = (char *)data_block + (i * columns * type_size);

  *ptr_to_matrix = matrix_rows;
  return 0;
}

void free_matrix(void ***ptr_to_matrix) {
  if (*ptr_to_matrix == NULL)
    return;
  free((*ptr_to_matrix)[0]);
  free(*ptr_to_matrix);
  *ptr_to_matrix = NULL;
}