#pragma once


#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

/**
 * @brief Initializes a 2D matrix with row pointers and an aligned contiguous data block.
 *
 * @param[out] ptr_to_matrix Triple pointer where the row pointers array will be
 * stored. Must point to a NULL-initialized pointer (float** variable set to NULL).
 * @param[in] rows Number of rows (must be > 0 for non-empty matrix).
 * @param[in] columns Number of columns (must be > 0 for non-empty matrix).
 * @param[in] type_size Size in bytes of each element (e.g., sizeof(float)).
 * @param[in] alignment Memory alignment requirement in bytes (must be > 0 and
 * preferably a power of 2 for best performance).
 *
 * @return int Status code:
 *             - 0: Success (matrix initialized or empty matrix requested)
 *             - 1: Error - ptr_to_matrix is NULL
 *             - 2: Error - Target matrix pointer is not NULL (already initialized)
 *             - 3: Error - Empty matrix requested (rows/columns/type_size = 0)
 *             - 4: Error - Invalid alignment (alignment = 0)
 *             - 5: Error - Size overflow detected (rows*columns*type_size > SIZE_MAX)
 *             - 6: Error - Memory allocation failed
 *
 * @note
 * - The allocated memory is uninitialized (contents are undefined).
 * - If failed, ptr_to_matrix remains unchanged, and no memory is allocated.
 * - The caller must check the return value before using the matrix.
 * - For best performance, use power-of-2 alignment values (1, 2, 4, 8, 16...).
 * - The function handles both power-of-2 and arbitrary alignment requirements.
 * - Memory layout: [row pointers][padding][aligned data block].
 *
 * @warning
 * - Always initialize the input pointer to NULL before first use.
 * - Not thread-safe during initialization.
 * - The caller is responsible for freeing the matrix with a single free() call.
 * - Alignment values that are not powers of 2 may incur performance overhead.
 */
int init_matrix(void ***ptr_to_matrix, const size_t rows, const size_t columns,
                const size_t type_size, const size_t alignment);


#endif