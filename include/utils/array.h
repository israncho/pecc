#pragma once

#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <stddef.h>

/**
 * @brief Status codes for array operations.
 *
 * These codes are returned by array manipulation functions to indicate success
 * or specific failure conditions. Always check these return values.
 */
typedef enum {
  ARRAY_OK = 0,          ///< Operation completed successfully
  ARRAY_ERR_NULL,        ///< Null pointer provided where forbidden
  ARRAY_ERR_ZERO,        ///< Requested size was zero (invalid operation)
  ARRAY_ERR_TYPE_S_ZERO, ///< Requested type size was zero (invalid operation)
  ARRAY_ERR_IN_USE,      ///< Pointer already contains an allocated array
  ARRAY_ERR_ALLOC        ///< Memory allocation failed (check errno)
} ArrayStatus;

/**
 * @brief Resizes a dynamically allocated array.
 *
 * This function behaves similarly to realloc:
 * - When *ptr_to_array is NULL: acts like malloc(new_size)
 * - When new_size is 0: returns ARRAY_ERR_ZERO (does NOT free memory)
 * - On failure: original array remains unchanged
 *
 * @param ptr_to_array [in,out] Pointer to array pointer. Must be a valid
 * pointer (though *ptr_to_array may be NULL). Will be updated if reallocation
 * succeeds.
 * @param new_size [in] Requested new size for the array. Must be > 0.
 * @param type_size [in] Size in bytes that each entry in the array will have.
 *
 * @return ArrayStatus code:
 *         - ARRAY_OK: Resize succeeded, *ptr_to_array updated
 *         - ARRAY_ERR_NULL: ptr_to_array was NULL
 *         - ARRAY_ERR_ZERO: new_size was 0
 *         - ARRAY_ERR_TYPE_S_ZERO: type_size was 0
 *         - ARRAY_ERR_ALLOC: Memory allocation failed
 * @note For zero-initialized allocation, use init_array() instead.
 */
ArrayStatus resize_array(void **ptr_to_array, const size_t new_size,
                         const size_t type_size);

/**
 * @brief Initializes a new array with requested size.
 *
 * Similar to resize_array(), but specifically for initial allocation:
 * - Fails if *ptr_to_array already contains a pointer
 *
 * @param ptr_to_array [out] Pointer where new array will be stored. Must point
 *                 to NULL (or be NULL itself).
 * @param size [in] Initial array size. Must be > 0.
 * @param type_size [in] Size in bytes that each entry in the array will have.
 *
 * @return ArrayStatus code:
 *         - ARRAY_OK: Allocation succeeded
 *         - ARRAY_ERR_NULL: ptr_to_array was NULL
 *         - ARRAY_ERR_IN_USE: *ptr_to_array was not NULL
 *         - ARRAY_ERR_ZERO: size was 0
 *         - ARRAY_ERR_TYPE_S_ZERO: type_size was 0
 *         - ARRAY_ERR_ALLOC: Memory allocation failed
 *
 * @note This is safer than resize_array() for initial allocation as it prevents
 *       accidental memory leaks by verifying *ptr_to_array is NULL.
 */
ArrayStatus init_array(void **ptr_to_array, const size_t size,
                       const size_t type_size);

int generic_swap(void *array, const size_t array_size,
                        const size_t type_size, const size_t i, const size_t j);

#define SWAP_T(arr, type, i, j) do { \
    type *__arr = (arr); \
    size_t __i = (i), __j = (j); \
    type __tmp = __arr[__i]; \
    __arr[__i] = __arr[__j]; \
    __arr[__j] = __tmp; \
} while (0)

#endif