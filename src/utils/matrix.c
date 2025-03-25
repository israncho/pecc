#include "../../include/utils/matrix.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int init_matrix(void ***ptr_to_matrix,
                const size_t rows,
                const size_t columns,
                const size_t type_size) {
  if (*ptr_to_matrix != NULL)
    return 1;

  if (rows == 0 || columns == 0 || type_size == 0)
    return 0; // empty matrix needs no space

  // memory for the ptrs to each row.
  void *tmp_ptr = malloc(sizeof(void *) * rows);
  if (!tmp_ptr)
    return 2;
  *ptr_to_matrix = tmp_ptr;

  // memory for all the data in the matrix.
  tmp_ptr = malloc(type_size * rows * columns);
  if (!tmp_ptr) {
    free(*ptr_to_matrix);
    return 2;
  }

  for (size_t i = 0; i < rows; i++)
    (*ptr_to_matrix)[i] = tmp_ptr + (i * columns * type_size);
  return 0;
}

void free_matrix(void ***ptr_to_matrix) {
  if (*ptr_to_matrix == NULL)
    return;
  free((*ptr_to_matrix)[0]);
  free(*ptr_to_matrix);
  *ptr_to_matrix = NULL;
}