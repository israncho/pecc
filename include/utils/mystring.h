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
 * @param[in,out] ptr_to_str_len Pointer to the size of the string (excluding
 * null-terminator). Will be updated with the new size after stripping.
 *
 * @retval 0 Success
 * @retval 1 Null ptr_to_str or *ptr_to_str
 * @retval 2 Null ptr_to_str_len
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
 *          - Sized appropriately in ptr_to_str_len
 *
 * @example
 *   char *s = strdup("   Hello   ");
 *   size_t len = strlen(s);
 *   strip_in_place(&s, &len);
 *   // s now contains "Hello", len = 5
 *   free(s);
 */
int strip_in_place(char **ptr_to_str, size_t *ptr_to_str_len);

/**
 * @brief Removes leading and trailing whitespace from a string and stores the
 * result in a provided buffer.
 *
 * The function allocates or resizes the destination buffer if necessary to fit
 * the stripped string. Whitespace characters include: space (' '), newline
 * ('\n'), tab ('\t'), carriage return ('\r'), vertical tab ('\v'), and form
 * feed ('\f').
 *
 * @param[in]  str_to_strip      Input string to strip (must be non-NULL and
 * nul-terminated).
 * @param[in]  str_len          Length of the input string (must be > 0).
 * @param[out] ptr_to_buffer     Pointer to the destination buffer where the
 * stripped string will be stored. The buffer must be pre-allocated.
 * @param[out] stripped_len   Pointer to store the length of the stripped
 * string (excluding nul-terminator).
 * @param[out] buffer_capacity       Pointer to the current capacity of the
 * destination buffer. Updated if resized.
 *
 * @retval 0                     Success. The stripped string is stored in
 * `*ptr_to_buffer`.
 * @retval 1                     Error: `str_to_strip` is NULL.
 * @retval 2                     Error: `ptr_to_buffer` is NULL.
 * @retval 3                     Error: `*ptr_to_buffer` is NULL (invalid
 * buffer).
 * @retval 4                     Error: `stripped_len` is NULL.
 * @retval 5                     Error: `buffer_capacity` is NULL.
 * @retval 6                     Error: `*buffer_capacity` is 0 (invalid
 * capacity).
 * @retval 7                     Error: Failed to resize the destination buffer.
 *
 * @note If the input string contains only whitespace, the result will be an
 * empty string (`*stripped_len = 0`).
 * @warning The function may reallocate `*ptr_to_buffer` if the stripped string
 * exceeds `*buffer_capacity`.
 */
int strip_to_buffer(const char *str_to_strip, const size_t str_len,
                    char **ptr_to_buffer, size_t *stripped_len,
                    size_t *buffer_capacity);

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
 * @param[in] str The input string to split_to_buffer. Must be null-terminated within
 * str_len.
 * @param[in] str_len Length of the input string.
 * @param[in] delimiter The character used to split_to_buffer the string (cannot be '\0').
 * @param[out] ptr_to_tokens_array Pointer to store the resulting string matrix.
 *                                Will contain pointers to each token.
 * @param[out] num_of_tokens Pointer to store the number of tokens found.
 *
 * @return int Status code:
 *   - 0: Success
 *   - 1: str is NULL
 *   - 2: str_len is 0
 *   - 3: delimiter is '\0'
 *   - 4: ptr_to_char_matrix is NULL
 *   - 5: matrix_rows is NULL
 *   - 6: Output matrix pointer is not NULL, i.e. pointer already in use
 * (potential memory leak)
 *   - 7: Memory allocation failed
 *
 * @note
 * - The function ignores leading, trailing, and consecutive delimiters.
 *   Example: split_to_buffer(",,a,,b,,", ',') → {"a", "b"}
 * - The caller must free both levels of allocation using free_matrix().
 * - For empty strings or no tokens found, returns success with:
 *   - *matrix_rows = 0
 *   - *ptr_to_char_matrix = NULL
 *
 * @warning
 * - str must be null-terminated within str_len.
 * - Not thread-safe during memory allocation.
 */
int split_to_buffer(const char *str, const size_t str_len, const char delimiter,
                    char ***ptr_to_tokens_array,
                    size_t *ptr_to_tokens_array_capacity,
                    size_t *ptr_to_num_of_tokens, char **ptr_to_buffer,
                    size_t *ptr_to_buffer_capacity);

#endif