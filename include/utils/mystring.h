#ifndef MYSTRING_H
#define MYSTRING_H

#include <stddef.h>

/**
 * @brief Removes leading and trailing whitespace characters from a string.
 *
 * This function modifies the input string in-place by removing all leading and
 * trailing whitespace characters (space, tab, newline, carriage return,
 * vertical tab, form feed). The string's memory buffer is resized to fit the
 * stripped content.
 *
 * @param[in,out] ptr_to_str Pointer to the string pointer to be stripped. The
 * pointer may be modified if reallocation occurs.
 * @param[in,out] ptr_to_str_size Pointer to the size of the string (excluding
 * null-terminator). Will be updated with the new size after stripping.
 *
 * @retval 0 Success
 * @retval 1 Null ptr_to_str or *ptr_to_str
 * @retval 2 Null ptr_to_str_size
 * @retval 3 Empty string (size = 0)
 *
 * @note The function handles these whitespace characters:
 *       - ' ' (space)
 *       - '\t' (horizontal tab)
 *       - '\n' (newline)
 *       - '\r' (carriage return)
 *       - '\v' (vertical tab)
 *       - '\f' (form feed)
 *
 * @warning The input string must be:
 *          - Null-terminated
 *          - Allocated with malloc/calloc/realloc
 *          - Sized appropriately in ptr_to_str_size
 *
 * @example
 *   char *s = strdup("   Hello   ");
 *   size_t len = strlen(s);
 *   strip(&s, &len);
 *   // s now contains "Hello", len = 5
 *   free(s);
 */
int strip(char **ptr_to_str, size_t *ptr_to_str_size);

/**
 * @brief Splits a string into tokens based on a single-character delimiter.
 *
 * This function divides the input string into substrings (tokens) separated by
 * the specified delimiter. The results are stored in a dynamically allocated
 * matrix of strings. Memory is allocated in two contiguous blocks for
 * efficiency:
 *   1. An array of char pointers (one per token)
 *   2. A single buffer containing all string data (with null terminators)
 *
 * @param[in] str The input string to split. Must be null-terminated within
 * str_size.
 * @param[in] str_size Length of the input string including null terminator.
 * @param[in] delimiter The character used to split the string (cannot be '\0').
 * @param[out] ptr_to_char_matrix Pointer to store the resulting string matrix.
 *                                Will contain pointers to each token.
 * @param[out] matrix_rows Pointer to store the number of tokens found.
 *
 * @return int Status code:
 *   - 0: Success
 *   - 1: str is NULL
 *   - 2: str_size is 0
 *   - 3: delimiter is '\0'
 *   - 4: ptr_to_char_matrix is NULL
 *   - 5: matrix_rows is NULL
 *   - 6: Output matrix pointer is not NULL, i.e. pointer already in use
 * (potential memory leak)
 *   - 7: Memory allocation failed
 *
 * @note
 * - The function ignores leading, trailing, and consecutive delimiters.
 *   Example: split(",,a,,b,,", ',') → {"a", "b"}
 * - The caller must free both levels of allocation using free_matrix().
 * - For empty strings or no tokens found, returns success with:
 *   - *matrix_rows = 0
 *   - *ptr_to_char_matrix = NULL
 *
 * @warning
 * - str must be null-terminated within str_size.
 * - Not thread-safe during memory allocation.
 */
int split(const char *str, const size_t str_size, const char delimiter,
          char ***ptr_to_char_matrix, size_t *matrix_rows);

#endif