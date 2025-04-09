#pragma once

#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

/**
 * @brief Runs all matrix module tests
 */
void test_matrix();

/**
 * @brief Tests matrix initialization and freeing
 */
void test_init_and_free_matrix();

/**
 * @brief Tests large matrix allocation and access
 */
void test_huge_matrix();

/**
 * @brief Tests matrices with complex data types
 */
void test_complex_type();

/**
 * @brief Tests safe matrix freeing (NULL, double free)
 */
void test_free_safety();

/**
 * @brief Tests memory operations integrity
 */
void test_memory_integrity();

/**
 * @brief Tests double initialization protection
 */
void test_double_init();

/**
 * @brief Tests minimal 1x1 matrix case
 */
void test_minimal_matrix();

#endif
