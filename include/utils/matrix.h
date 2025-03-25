
#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

/**
 * @brief Initializes a 2D matrix with row pointers and a contiguous data block.
 *
 * @param[out] ptr_to_matrix Triple pointer where the row pointers array will be
 * stored. Must point to a NULL-initialized pointer (float** variable set to
 * NULL).
 * @param[in]  rows         Number of rows (must be > 0).
 * @param[in]  columns      Number of columns (must be > 0).
 * @param[in]  type_size    Size in bytes of each element (e.g., sizeof(float)).
 *
 * @return int              Status code:
 *                          - 0: Success
 *                          - 1: Error - Input matrix pointer is not NULL
 * (already initialized)
 *                          - 2: Error - Memory allocation failed
 *
 * @note
 * - The allocated memory is uninitialized (contents are undefined).
 * - If failed, ptr_to_matrix remains unchanged, and no memory is allocated.
 * - The caller must check the return value before using the matrix.
 *
 * @warning
 * - Always initialize the input pointer to NULL before first use:
 * - Not thread-safe during initialization.
 */
int init_matrix(void ***ptr_to_matrix, const size_t rows, const size_t columns,
                const size_t type_size);

/**
 * Frees memory allocated by init_matrix.
 *
 * @param ptr_to_matrix   Pointer to the matrix.
 *
 * @note
 * - The content of `ptr_to_matrix` is set to NULL after
 * use.
 * @warning
 * - Calling with NULL is safe (no operation performed).
 * - Accessing matrix after free results in undefined behavior.
 */
void free_matrix(void ***ptr_to_matrix);

#endif