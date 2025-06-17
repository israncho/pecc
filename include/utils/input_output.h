#pragma once

#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include <stddef.h>
#include <stdint.h>

/**
 * @enum FileReadStatus
 * @brief Status codes for the `read_file` function.
 */
typedef enum {
  FILE_READ_SUCCESS = 0,          // Succesful operation.
  FILE_READ_NULL_FILENAME,        // `file_name` is NULL.
  FILE_READ_NULL_NUM_LINES_PTR,   // `ptr_to_num_lines` is NULL.
  FILE_READ_NULL_LINES_ARRAY_PTR, // `ptr_to_array_of_lines` is NULL.
  FILE_READ_LINES_ARRAY_IN_USE, // `*ptr_to_array_of_lines` is not NULL, i.e. in
                                // use.
  FILE_READ_ERR_OPEN_FAILED,    // Could not open file.
  FILE_READ_MEMORY_ERROR,       // Could not allocate memory for the array of
                                // `file_line`.
} FileReadStatus;

/**
 * @enum FileWriteStatus
 * @brief Status codes for the `write_to_file` function.
 */
typedef enum {
  FILE_WRITE_SUCCESS = 0,       // Succesful operation.
  FILE_WRITE_NULL_FILENAME,     // `file_name` is NULL.
  FILE_WRITE_NULL_MODE,         // `mode` is NULL.
  FILE_WRITE_ERR_WRONG_MODE,    // `mode` is not "a" or "w".
  FILE_WRITE_ERR_WHILE_WRITING, // Error while writing in the given file.
  FILE_WRITE_ERR_OPEN_FAILED,   // Could not open file.
} FileWriteStatus;

/**
 * @struct file_line
 * @brief Structure to represent a line in a file.
 *
 * This structure is used to store the content of a single line from a file and
 * its length.
 */
typedef struct {
  char *content; /**< Pointer to the content of the file line */
  size_t length; /**< Length of the file line */
} file_line;

/**
 * @brief Frees the memory allocated for a file_line structure.
 *
 * This function frees the memory for the content of the file line and sets the
 * content pointer to NULL. Sets the pointer to the `file_line` to NULL. It also
 * sets the length of the file line to 0.
 *
 * @param file_line_to_free A pointer to the file_line structure to free.
 */
void free_file_line(file_line **ptr_to_file_line_to_free);

/**
 * @brief Frees the dynamically allocated 'content' field of each file_line in
 * an array.
 *
 * This function iterates over an array of `file_line` structs and frees the
 * memory allocated for their 'content' fields. It also resets each 'content'
 * pointer to NULL and sets 'length' to 0 for safety. The array itself is NOT
 * freed.
 *
 * @param array_of_lines Pointer to the array of `file_line` structs.
 * @param array_size Number of elements in the array.
 *
 * @note
 * - If `array_of_lines` is NULL, the function returns immediately.
 * - This function does not free the array container (only its 'content'
 * fields).
 * - After calling this function, the structs remain valid but with empty
 * content.
 */
void free_lines_array_content(file_line *array_of_lines,
                              const size_t array_size);

/**
 * @brief Reads the contents of a file and stores each line in an array of
 * file_line structures.
 *
 * This function opens the specified file, reads its contents line by line, and
 * stores each line (along with its content and length) in an array of file_line
 * structures. It dynamically allocates memory for the array and the content of
 * each line.
 *
 * @param[in] file_name Path to the file to read. Must be a valid non-NULL
 * string.
 * @param[out] ptr_to_array_of_lines Pointer to the array that will hold the
 * file lines. The pointer must point to NULL initially (*ptr_to_array_of_lines
 * == NULL).
 * @param[out] ptr_to_num_lines Pointer to store the number of lines read.
 *
 * @return FileReadStatus indicating operation result
 *
 * @note The caller is responsible for freeing both:
 *       - Each line's content (file_line[i].content)
 *       - The array itself (file_line*)
 * @warning If the function returns any value other than FILE_READ_SUCCESS,
 *          no memory needs to be freed by the caller as the function handles
 *          cleanup of partial allocations.
 *
 * @see FileReadStatus
 * @see file_line
 */
FileReadStatus read_file(const char *file_name,
                         file_line **ptr_to_array_of_lines,
                         size_t *ptr_to_num_lines);

FileWriteStatus write_to_file_in_specific_order(const char *file_name,
                                                const file_line *array_of_lines,
                                                const size_t num_lines,
                                                const size_t *permutation,
                                                const char *mode);

/**
 * @brief Writes the contents of an array of file_line structures to a file.
 *
 * This function writes each line stored in the array of file_line structures to
 * the specified file, with control over writing mode (overwrite/append).
 * The function handles all file operations safely and reports detailed status.
 *
 * @param[in] file_name Path to the target file. Must be a valid non-NULL
 * string.
 * @param[in] array_of_lines Array of file_line structures containing text
 * lines.
 * @param[in] num_lines Number of elements in array_of_lines.
 * @param[in] mode File access mode:
 *             - "w": Truncate and overwrite file (creates if nonexistent)
 *             - "a": Append to existing file (creates if nonexistent)
 *
 * @return FileWriteStatus indicating operation result.
 * @note Behavior details:
 *       - Empty arrays (num_lines == 0) will create/truncate the file if mode
 * is "w"
 *       - File permissions follow umask settings
 *       - Line endings are preserved as stored in file_line.content
 *       - No buffering: content is written to disk before returning
 *
 * @warning Special cases:
 *          - Concurrent access to the same file may cause race conditions
 *          - On writing errors, the file may be partially written
 *          - Large files may require streaming instead of full buffering
 * @see file_line
 * @see FileWriteStatus
 * @see fopen() for underlying file operations
 */
FileWriteStatus write_to_file(const char *file_name,
                              const file_line *array_of_lines,
                              const size_t num_lines, const char *mode);

int str_to_size_t(const char *str, size_t *result);

int str_to_double(const char *str, double *result);

void get_arguments_for_exec(size_t *generations, size_t *population_size,
                            double *mutation_rate, size_t *n_threads,
                            uint64_t *seed1, uint64_t *seed2,
                            size_t *local_search_iters,
                            char **ptr_to_instance_file_path,
                            char **ptr_to_output_file_path,
                            const char *input_file_path);

FileWriteStatus write_doubles_with_csv_format(const char *file_name,
                                              const double *array,
                                              const size_t array_len,
                                              const char *mode);

#endif
