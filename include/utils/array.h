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
  ARRAY_ERR_ALLOC,       ///< Memory allocation failed (check errno)
  ARRAY_ERR_NO_CAPACITY ///< Not enough capacity 
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

/**
 * @brief Sets up an array within a preallocated memory block with proper alignment.
 *
 * This function takes a preallocated memory block and carves out a properly aligned
 * array from it, updating the remaining memory capacity. It handles alignment
 * requirements and checks if there's sufficient capacity in the preallocated block.
 *
 * @param ptr_to_mem [in/out] Pointer to the preallocated memory block. Will be
 *                 advanced by the amount of memory used (including padding).
 * @param ptr_to_mem_capacity [in/out] Pointer to the remaining capacity of the
 *                 preallocated memory block. Will be decremented by the total
 *                 memory used (including padding).
 * @param ptr_to_array [out] Pointer where the aligned array address will be stored.
 * @param size [in] Number of elements requested for the array. Must be > 0.
 * @param type_size [in] Size in bytes of each array element. Must be > 0.
 * @param alignment [in] Required memory alignment for the array. Must be a power
 *                 of 2. If 0, no alignment is performed.
 *
 * @return ArrayStatus code:
 *         - ARRAY_OK: Array was successfully set up
 *         - ARRAY_ERR_NO_CAPACITY: Not enough space in preallocated memory
 *                                 (including alignment padding)
 *
 * @note The function handles alignment in two ways:
 *       1. For power-of-2 alignment: uses bitwise operations for efficiency
 *       2. For other cases: uses modulo arithmetic
 * @note The total memory consumed includes:
 *       - The actual array data (size * type_size)
 *       - Any padding needed for alignment
 * @note After successful execution:
 *       - *ptr_to_mem points to the remaining available memory
 *       - *ptr_to_mem_capacity reflects the remaining bytes
 *       - *ptr_to_array points to the aligned array start
 */
ArrayStatus
setup_array_from_prealloc_mem(void **ptr_to_mem, size_t *ptr_to_mem_capacity,
                              void **ptr_to_array, const size_t size,
                              const size_t type_size, const size_t alignment);

#define SWAP_T(arr, type, i, j)                                                \
  do {                                                                         \
    type *__arr = (arr);                                                       \
    size_t __i = (i), __j = (j);                                               \
    type __tmp = __arr[__i];                                                   \
    __arr[__i] = __arr[__j];                                                   \
    __arr[__j] = __tmp;                                                        \
  } while (0)

#endif