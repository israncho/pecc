#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/test/utils/test_matrix.h"
#include "../../include/utils/matrix.h"

void test_init_and_free_matrix() {
  float **matrix = NULL;
  size_t float_size = sizeof(float);
  size_t float_alignment = _Alignof(float);
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, 0, 5, float_size, float_alignment);
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 5, 0, float_size, float_alignment);
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 0, 0, float_size, float_alignment);
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 5, 5, 0, 0);
  assert(matrix == NULL);
  assert(init_status_code == 0);

  init_status_code = init_matrix((void ***)&matrix, 5, 5, float_size, float_alignment);
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

  free(matrix);
  matrix = NULL;

  int **int_matrix = NULL;
  init_status_code = init_matrix((void ***)&int_matrix, 4, 4, sizeof(int), _Alignof(int));
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

  free(int_matrix);
  int_matrix = NULL;
  printf("\t- init_matrix: PASSED\n");
  printf("\t- free_matrix: PASSED\n");
}

void test_huge_matrix() {
  size_t huge = 1 << 20; // 1 mill elems.
  float **matrix = NULL;
  size_t float_size = sizeof(float);
  size_t float_alignment = _Alignof(float);
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, huge, 1, float_size, float_alignment);
  assert(init_status_code == 0);
  assert(matrix != NULL);
  matrix[999999][0] = 10.0;
  assert(matrix[999999][0] == 10.0);
  free(matrix);
  matrix = NULL;

  init_status_code = init_matrix((void ***)&matrix, 1, huge, float_size, float_alignment);
  assert(init_status_code == 0);
  assert(matrix != NULL);
  matrix[0][999999] = 10.0;
  assert(matrix[0][999999] == 10.0);
  free(matrix);
  matrix = NULL;

  init_status_code = init_matrix((void ***)&matrix, 1000, 1000, float_size, float_alignment);
  assert(init_status_code == 0);
  assert(matrix != NULL);
  matrix[999][999] = 10.0;
  assert(matrix[999][999] == 10.0);
  free(matrix);
  matrix = NULL;

  printf("\t- huge_matrix: PASSED\n");
}

void test_complex_type() {
  typedef struct {
    double x;
    char c;
  } WeirdType;

  int init_status_code = -1;
  WeirdType **matrix = NULL;

  init_status_code = init_matrix((void ***)&matrix, 2, 2, sizeof(WeirdType), _Alignof(WeirdType));
  assert(matrix != NULL);
  assert(init_status_code == 0);
  matrix[1][1] = (WeirdType){3.14, 'X'};
  assert(matrix[1][1].x == 3.14);
  free(matrix);
  matrix = NULL;

  typedef struct {
    int id;
    char name[20];
    float *scores;
  } Student;

  Student **student_matrix = NULL;
  init_status_code = init_matrix((void ***)&student_matrix, 2, 2, sizeof(Student), _Alignof(Student));
  assert(student_matrix != NULL);
  assert(init_status_code == 0);

  float scores[3] = {9.5, 8.0, 7.5};
  student_matrix[0][0] = (Student){1, "Alice", scores};
  assert(student_matrix[0][0].id == 1);
  assert(strcmp(student_matrix[0][0].name, "Alice") == 0);
  assert(student_matrix[0][0].scores[0] == 9.5);

  free(student_matrix);
  student_matrix = NULL;

  printf("\t- complex_type: PASSED\n");
}

void test_free_safety() {
  float **matrix = NULL;
  size_t float_size = sizeof(float);
  size_t float_alignment = _Alignof(float);
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, 5, 5, float_size, float_alignment);
  assert(matrix != NULL);
  assert(init_status_code == 0);

  free(matrix);
  assert(matrix != NULL);
  matrix = NULL;
  free(matrix);
  assert(matrix == NULL);

  float **tmp = NULL;
  free(tmp);
  assert(tmp == NULL);
  printf("\t- free_safety: PASSED\n");
}

void test_memory_integrity() {
  float **matrix = NULL;
  size_t float_size = sizeof(float);
  size_t float_alignment = _Alignof(float);
  int init_status_code = -1;

  init_status_code = init_matrix((void ***)&matrix, 3, 3, float_size, float_alignment);
  assert(matrix != NULL);
  assert(init_status_code == 0);

  float pattern[9] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  memcpy(matrix[0], pattern, sizeof(pattern));
  assert(memcmp(matrix[0], pattern, sizeof(pattern)) == 0);

  free(matrix);
  matrix = NULL;
  printf("\t- memory_integrity: PASSED\n");
}

void test_double_init() {

  int **matrix = NULL;
  int **tmp_ptr = NULL;
  size_t int_size = sizeof(int);
  size_t int_alignment = _Alignof(int);
  int status = init_matrix((void ***)&matrix, 3, 3, int_size, int_alignment);
  assert(matrix != NULL);
  assert(status == 0);
  tmp_ptr = matrix;

  int val = 0;
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      matrix[i][j] = val++;

  status = init_matrix((void ***)&matrix, 3, 3, int_size, int_alignment);
  assert(status == 2); // MATRIX_ERROR_ALREADY_INIT
  assert(matrix != NULL);
  assert(matrix == tmp_ptr);

  val = 0;
  for (size_t i = 0; i < 3; i++)
    for (size_t j = 0; j < 3; j++)
      assert(matrix[i][j] == val++);

  free(matrix);
  matrix = NULL;
  printf("\t- double_init: PASSED\n");
}

void test_minimal_matrix() {
  float **matrix = NULL;
  size_t float_size = sizeof(float);
  size_t float_alignment = _Alignof(float);
  int status = init_matrix((void ***)&matrix, 1, 1, float_size, float_alignment);
  assert(matrix != NULL);
  assert(status == 0);
  matrix[0][0] = 42.0;
  assert(matrix[0][0] == 42.0);
  free(matrix);
  matrix = NULL;
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