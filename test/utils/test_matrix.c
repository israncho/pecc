#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/test/utils/test_matrix.h"
#include "../../include/utils/matrix.h"

void test_init_and_free_matrix() {
  float **matrix = NULL;
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, 0, 5, sizeof(float));
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 5, 0, sizeof(float));
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 0, 0, sizeof(float));
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 5, 5, 0);
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 5, 5, sizeof(float));
  assert(matrix != NULL);
  assert(init_status_code == 0);

  for (size_t i = 0; i < 5; i++)
    assert(matrix[i] != NULL);

  float stack_matrix[5][5] = {{0.0, 1.0, 2.0, 3.0, 4.0},
                              {0.0, 1.0, 2.0, 3.0, 4.0},
                              {0.0, 1.0, 2.0, 3.0, 4.0},
                              {0.0, 1.0, 2.0, 3.0, 4.0},
                              {0.0, 1.0, 2.0, 3.0, 4.0}};

  for (size_t i = 0; i < 5; i++)
    for (size_t j = 0; j < 5; j++)
      matrix[i][j] = stack_matrix[i][j];

  for (size_t i = 0; i < 5; i++)
    for (size_t j = 0; j < 5; j++)
      assert(matrix[i][j] == stack_matrix[i][j]);

  free_matrix((void ***)&matrix);

  int **int_matrix = NULL;
  init_status_code = init_matrix((void ***)&int_matrix, 4, 4, sizeof(int));
  assert(init_status_code == 0);
  assert(int_matrix != NULL);

  int val = 0;
  for (size_t i = 0; i < 4; i++)
    for (size_t j = 0; j < 4; j++)
      int_matrix[i][j] = val++;

  val = 0;
  for (size_t i = 0; i < 4; i++)
    for (size_t j = 0; j < 4; j++)
      assert(int_matrix[i][j] == val++);

  free_matrix((void ***)&int_matrix);
  printf("\t- init_matrix: PASSED\n");
  printf("\t- free_matrix: PASSED\n");
}

void test_huge_matrix() {
  size_t huge = 1 << 20; // 1 mill elems.
  float **matrix = NULL;
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, huge, 1, sizeof(float));
  assert(matrix != NULL);
  assert(init_status_code == 0);
  matrix[999999][0] = 10.0;
  assert(matrix[999999][0] == 10.0);
  free_matrix((void ***)&matrix);

  init_status_code = init_matrix((void ***)&matrix, 1, huge, sizeof(float));
  assert(matrix != NULL);
  matrix[0][999999] = 10.0;
  assert(matrix[0][999999] == 10.0);
  free_matrix((void ***)&matrix);

  init_status_code = init_matrix((void ***)&matrix, 1000, 1000, sizeof(float));
  assert(matrix != NULL);
  assert(init_status_code == 0);
  matrix[999][999] = 10.0;
  assert(matrix[999][999] == 10.0);
  free_matrix((void ***)&matrix);

  printf("\t- huge_matrix: PASSED\n");
}

void test_complex_type() {
  typedef struct {
    double x;
    char c;
  } WeirdType;

  int init_status_code = -1;
  WeirdType **matrix = NULL;

  init_status_code = init_matrix((void ***)&matrix, 2, 2, sizeof(WeirdType));
  assert(matrix != NULL);
  assert(init_status_code == 0);
  matrix[1][1] = (WeirdType){3.14, 'X'};
  assert(matrix[1][1].x == 3.14);
  free_matrix((void ***)&matrix);

  typedef struct {
    int id;
    char name[20];
    float *scores;
  } Student;

  Student **student_matrix = NULL;
  init_status_code = init_matrix((void ***)&student_matrix, 2, 2, sizeof(Student));
  assert(student_matrix != NULL);
  assert(init_status_code == 0);

  float scores[3] = {9.5, 8.0, 7.5};
  student_matrix[0][0] = (Student){1, "Alice", scores};
  assert(student_matrix[0][0].id == 1);
  assert(strcmp(student_matrix[0][0].name, "Alice") == 0);
  assert(student_matrix[0][0].scores[0] == 9.5);

  free_matrix((void ***)&student_matrix);

  printf("\t- complex_type: PASSED\n");
}

void test_free_safety() {
  float **matrix = NULL;
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, 5, 5, sizeof(float));
  assert(matrix != NULL);
  assert(init_status_code == 0);

  free_matrix((void ***)&matrix);
  assert(matrix == NULL);

  free_matrix((void ***)&matrix);
  assert(matrix == NULL);

  float **tmp = NULL;
  free_matrix((void ***)&tmp);
  assert(tmp == NULL);
  printf("\t- free_safety: PASSED\n");
}

void test_memory_integrity() {
  float **matrix = NULL;
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, 3, 3, sizeof(float));
  assert(matrix != NULL);
  assert(init_status_code == 0);

  float pattern[9] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  memcpy(matrix[0], pattern, sizeof(pattern));
  assert(memcmp(matrix[0], pattern, sizeof(pattern)) == 0);

  free_matrix((void ***)&matrix);
  printf("\t- memory_integrity: PASSED\n");
}

void test_double_init() {

  int **matrix = NULL;
  int **tmp_ptr = NULL;
  int status = init_matrix((void ***)&matrix, 3, 3, sizeof(int));
  assert(matrix != NULL);
  assert(status == 0);
  tmp_ptr = matrix;

  int val = 0;
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      matrix[i][j] = val++;

  status = init_matrix((void ***)&matrix, 3, 3, sizeof(int));
  assert(status == 2); // MATRIX_ERROR_ALREADY_INIT
  assert(matrix != NULL);
  assert(matrix == tmp_ptr);

  val = 0;
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      assert(matrix[i][j] == val++);

  free_matrix((void ***)&matrix);
  printf("\t- double_init: PASSED\n");
}

void test_minimal_matrix() {
  float **matrix = NULL;
  int status = init_matrix((void ***)&matrix, 1, 1, sizeof(float));
  assert(matrix != NULL);
  assert(status == 0);
  matrix[0][0] = 42.0;
  assert(matrix[0][0] == 42.0);
  free_matrix((void ***)&matrix);
  printf("\t- minimal_matrix: PASSED\n");
}

void test_matrix() {
  printf("Testing: matrix\n");
  test_init_and_free_matrix();
  test_huge_matrix();
  test_complex_type();
  test_free_safety();
  test_memory_integrity();
  test_double_init();
  test_minimal_matrix();
}