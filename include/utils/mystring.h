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
int strip(char **ptr_to_str, size_t *ptr_to_str_size);

#endif