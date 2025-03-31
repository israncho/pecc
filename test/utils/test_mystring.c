#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/test/utils/test_mystring.h"
#include "../../include/utils/array.h"
#include "../../include/utils/matrix.h"
#include "../../include/utils/mystring.h"

void test_mystring() {
  printf("Testing: mystring\n");
  test_strip();
  test_split();
}

void test_strip() {
  size_t char_size = sizeof(char);

  char *str = NULL;

  //subtest 1
  size_t str_size = 10;
  assert(init_array((void **)&str, 11, char_size) == ARRAY_OK);
  strcpy(str, "   hola   ");
  assert(strcmp(str, "   hola   ") == 0);

  assert(str[10] == '\0');
  strip(&str, &str_size);
  assert(str[4] == '\0');
  assert(strcmp(str, "hola") == 0 && str_size == 4);

  //subtest 2
  str_size = 8;
  assert(resize_array((void **)&str, 9, char_size) == ARRAY_OK);
  strcpy(str, "   Mundo");
  assert(strcmp(str, "   Mundo") == 0);

  assert(str[8] == '\0');
  strip(&str, &str_size);
  assert(str[5] == '\0');
  assert(strcmp(str, "Mundo") == 0 && str_size == 5);

  //subtest 3
  str_size = 8;
  assert(resize_array((void **)&str, 9, char_size) == ARRAY_OK);
  strcpy(str, "Mundo   ");
  assert(strcmp(str, "Mundo   ") == 0);

  assert(str[8] == '\0');
  strip(&str, &str_size);
  assert(str[5] == '\0');
  assert(strcmp(str, "Mundo") == 0 && str_size == 5);

  //subtest 4
  str_size = 17;
  assert(resize_array((void **)&str, 18, char_size) == ARRAY_OK);
  strcpy(str, "\t\n \v\f\rTexto\f \n\t\r\v");
  assert(strcmp(str, "\t\n \v\f\rTexto\f \n\t\r\v") == 0);

  assert(str[17] == '\0');
  strip(&str, &str_size);
  assert(str[5] == '\0');
  assert(strcmp(str, "Texto") == 0 && str_size == 5);

  //subtest 5
  str_size = 12;
  assert(resize_array((void **)&str, 13, char_size) == ARRAY_OK);
  strcpy(str, "\t\n \v\f\r\f \n\t\r\v");
  assert(strcmp(str, "\t\n \v\f\r\f \n\t\r\v") == 0);

  assert(str[12] == '\0');
  strip(&str, &str_size);
  assert(str[0] == '\0');
  assert(strcmp(str, "") == 0 && str_size == 0);

  //subtest 6
  str_size = 0;
  assert(resize_array((void **)&str, 1, char_size) == ARRAY_OK);
  strcpy(str, "");
  assert(strcmp(str, "") == 0);

  assert(str[0] == '\0');
  strip(&str, &str_size);
  assert(str[0] == '\0');
  assert(strcmp(str, "") == 0 && str_size == 0);

  //subtest 7
  str_size = 11;
  assert(resize_array((void **)&str, 12, char_size) == ARRAY_OK);
  strcpy(str, "Hola mundo!");
  assert(strcmp(str, "Hola mundo!") == 0);

  assert(str[11] == '\0');
  strip(&str, &str_size);
  assert(str[11] == '\0');
  assert(strcmp(str, "Hola mundo!") == 0 && str_size == 11);

  //subtest 8
  str_size = 39;
  assert(resize_array((void **)&str, 40, char_size) == ARRAY_OK);
  strcpy(str, "\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v");
  assert(strcmp(str, "\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v") == 0);

  assert(str[39] == '\0');
  strip(&str, &str_size);
  assert(str[25] == '\0');
  assert(strcmp(str, "H o l a\f \n\t\r\v M u n d o !") == 0 && str_size == 25);

  //subtest 9
  str_size = 13;
  assert(resize_array((void **)&str, 14, char_size) == ARRAY_OK);
  strcpy(str, "\t\n \v\f\rA\f \n\t\r\v");
  assert(strcmp(str, "\t\n \v\f\rA\f \n\t\r\v") == 0);

  assert(str[13] == '\0');
  strip(&str, &str_size);
  assert(str[1] == '\0');
  assert(strcmp(str, "A") == 0 && str_size == 1);

  free(str);
  printf("\t- strip: PASSED\n");
}

void test_split() {
  char **str_matrix = NULL;
  size_t matrix_rows = 0;
  int status_code = -1;

  // subtest 1
  char *test_split[] = {"hola", "amigo", "como", "estas"};
  status_code = split("hola amigo como estas", 21, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 1.1
  status_code = split("   hola   amigo   como    estas   ", 34, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 1.2
  status_code = split("hola      amigo como      estas   ", 34, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 1.3
  status_code = split("          hola    amigo como estas", 34, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 2
  char *test_split2[] = {"a"};
  status_code = split("a", 1, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split2[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 2.1
  status_code = split("   a", 4, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split2[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 2.2
  status_code = split("a   ", 4, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split2[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 2.3
  status_code = split("  a ", 4, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split2[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 2.4
  status_code = split("a", 1, ',', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split2[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);


  // subtest 3
  char *test_split3[] = {"1", "2", "3", "4"};
  status_code = split("1,2,3,4", 7, ',', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 3.1
  status_code = split(",,,1,2,3,4,,,", 13, ',', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 3.2
  status_code = split("1-2-3-4", 7, '-', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 3.3
  status_code = split("1\n2\n3\n4", 7, '\n', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 4);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], str_matrix[i]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 4
  char *test_split4[] = {"hola_amigo_como_estas"};
  status_code = split("hola_amigo_como_estas", 21, ',', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split4[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 4.1
  status_code = split("hola_amigo_como_estas", 21, ' ', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split4[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 4.2
  status_code = split("hola_amigo_como_estas,", 22, ',', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split4[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  // subtest 4.2
  status_code = split(",hola_amigo_como_estas", 22, ',', &str_matrix, &matrix_rows);
  assert(status_code == 0);
  assert(matrix_rows == 1);
  assert(strcmp(test_split4[0], str_matrix[0]) == 0);
  free_matrix((void ***)&str_matrix);

  printf("\t- split: PASSED\n");
}