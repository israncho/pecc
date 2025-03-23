#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include <stddef.h>

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
 * content pointer to NULL. It also sets the length of the file line to 0.
 *
 * @param file_line_to_free A pointer to the file_line structure to free.
 */
void free_file_line(file_line *file_line_to_free);

/**
 * @brief Frees the memory allocated for an array of file_line structures.
 *
 * This function iterates over an array of `file_line` structures, freeing the
 * memory allocated for the `content` field of each structure. Once all the
 * individual `content` fields are freed, the memory allocated for the array
 * itself is also freed.
 *
 * @param array_of_lines A pointer to an array of `file_line` structures.
 * @param array_size Size of the given array.
 */
void free_array_of_lines(file_line *array_of_lines, const size_t array_size);

/**
 * @brief Reads the contents of a file and stores each line in an array of
 * file_line structures.
 *
 * This function opens the specified file, reads its contents line by line, and
 * stores each line (along with its content and length) in an array of file_line
 * structures. It dynamically allocates memory for the array and the content of
 * each line.
 *
 * @param file_name Path to the file to read.
 * @param ptr_to_array_of_lines A pointer to the array that will hold the file
 * lines.
 * @param ptr_to_num_lines A pointer to the variable where the number of lines
 * in the file will be stored.
 */
void read_file(const char *file_name,
               file_line **ptr_to_array_of_lines,
               size_t *ptr_to_num_lines);

/**
 * @brief Writes the contents of an array of file_line structures to a file.
 *
 * This function writes each line stored in the array of file_line structures to
 * the specified file. It supports different file modes (e.g., "w" for write,
 * "a" for append), allowing the caller to overwrite or append to the file.
 *
 * @param file_name Path to the file where the lines will be written.
 * @param array_of_lines Array of file_line structures containing the lines to
 * write.
 * @param num_lines Number of lines in the array.
 * @param mode File mode to use when opening the file (e.g., "w" for write, "a"
 * for append).
 *
 * @note If the file does not exist, it will be created. If it exists, its
 * contents will be overwritten or appended based on the specified mode.
 */
void write_to_file(const char *file_name,
                   const file_line *array_of_lines,
                   const size_t num_lines,
                   const char *mode);

#endif
