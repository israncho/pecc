#ifndef TEST_INPUT_OUTPUT_H
#define TEST_INPUT_OUTPUT_H

#include "../../utils/input_output.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Orchestrates and executes all available module tests for
 * `input_output`.
 *
 * This function serves as the main test runner for the `input_output` module.
 * It coordinates and runs all individual test cases.
 */
void test_input_output();

/**
 * @brief Tests the read_file function to verify its correctness.
 *
 * This function performs a series of test cases on the read_file function,
 * checking whether it correctly reads a file into an array of file_line
 * structures. The results of the test are displayed on the standard output.
 */
void test_read_file();

/**
 * @brief Tests the write_to_file function to verify its correctness.
 *
 * This function performs a series of test cases on the write_to_file function,
 * checking whether it correctly writes an array of file_line structures to a
 * file. The results of the test are displayed on the standard output.
 */
void test_write_to_file();

/**
 * @brief Compares two arrays of file_line structures for equality.
 *
 * This function checks if two arrays of file_line structures are equal in terms
 * of size and content. It compares both the number of elements in the arrays
 * and the content of each line.
 *
 * @param array_of_lines1 Pointer to the first array of file_line structures.
 * @param arr1_size The number of elements in the first array.
 * @param array_of_lines2 Pointer to the second array of file_line structures.
 * @param arr2_size The number of elements in the second array.
 * @return true if both arrays are identical in size and content, false
 * otherwise.
 */
bool equality_test_for_line_arrays(const file_line *array_of_lines1,
                                   const size_t arr1_size,
                                   const file_line *array_of_lines2,
                                   const size_t arr2_size);

#endif