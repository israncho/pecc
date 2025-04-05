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
  test_strip_in_place();
  test_strip_to_buffer();
  test_split_to_buffer();
  test_split_in_place();
}

void test_strip_in_place() {
  size_t char_size = sizeof(char);

  char *str = NULL;

  // subtest 1
  size_t str_len = 10;
  assert(init_array((void **)&str, 11, char_size) == ARRAY_OK);
  strcpy(str, "   hola   ");
  assert(strcmp(str, "   hola   ") == 0);

  assert(str[10] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[4] == '\0');
  assert(strcmp(str, "hola") == 0 && str_len == 4);

  // subtest 2
  str_len = 8;
  assert(resize_array((void **)&str, 9, char_size) == ARRAY_OK);
  strcpy(str, "   Mundo");
  assert(strcmp(str, "   Mundo") == 0);

  assert(str[8] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[5] == '\0');
  assert(strcmp(str, "Mundo") == 0 && str_len == 5);

  // subtest 3
  str_len = 8;
  assert(resize_array((void **)&str, 9, char_size) == ARRAY_OK);
  strcpy(str, "Mundo   ");
  assert(strcmp(str, "Mundo   ") == 0);

  assert(str[8] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[5] == '\0');
  assert(strcmp(str, "Mundo") == 0 && str_len == 5);

  // subtest 4
  str_len = 17;
  assert(resize_array((void **)&str, 18, char_size) == ARRAY_OK);
  strcpy(str, "\t\n \v\f\rTexto\f \n\t\r\v");
  assert(strcmp(str, "\t\n \v\f\rTexto\f \n\t\r\v") == 0);

  assert(str[17] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[5] == '\0');
  assert(strcmp(str, "Texto") == 0 && str_len == 5);

  // subtest 5
  str_len = 12;
  assert(resize_array((void **)&str, 13, char_size) == ARRAY_OK);
  strcpy(str, "\t\n \v\f\r\f \n\t\r\v");
  assert(strcmp(str, "\t\n \v\f\r\f \n\t\r\v") == 0);

  assert(str[12] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[0] == '\0');
  assert(strcmp(str, "") == 0 && str_len == 0);

  // subtest 6
  str_len = 0;
  assert(resize_array((void **)&str, 1, char_size) == ARRAY_OK);
  strcpy(str, "");
  assert(strcmp(str, "") == 0);

  assert(str[0] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[0] == '\0');
  assert(strcmp(str, "") == 0 && str_len == 0);

  // subtest 7
  str_len = 11;
  assert(resize_array((void **)&str, 12, char_size) == ARRAY_OK);
  strcpy(str, "Hola mundo!");
  assert(strcmp(str, "Hola mundo!") == 0);

  assert(str[11] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[11] == '\0');
  assert(strcmp(str, "Hola mundo!") == 0 && str_len == 11);

  // subtest 8
  str_len = 39;
  assert(resize_array((void **)&str, 40, char_size) == ARRAY_OK);
  strcpy(str, "\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v");
  assert(strcmp(str,
                "\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v") == 0);

  assert(str[39] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[25] == '\0');
  assert(strcmp(str, "H o l a\f \n\t\r\v M u n d o !") == 0 && str_len == 25);

  // subtest 9
  str_len = 13;
  assert(resize_array((void **)&str, 14, char_size) == ARRAY_OK);
  strcpy(str, "\t\n \v\f\rA\f \n\t\r\v");
  assert(strcmp(str, "\t\n \v\f\rA\f \n\t\r\v") == 0);

  assert(str[13] == '\0');
  strip_in_place(&str, &str_len);
  assert(str[1] == '\0');
  assert(strcmp(str, "A") == 0 && str_len == 1);

  free(str);
  printf("\t- strip_in_place: PASSED\n");
}

void test_strip_to_buffer() {
  size_t char_size = sizeof(char);

  char *buffer = NULL;

  size_t stripped_len = 0;
  size_t buffer_capacity = 2;
  assert(init_array((void **)&buffer, buffer_capacity, char_size) == 0);

  // subtest 1
  assert(strip_to_buffer("   hola   ", 10, &buffer, &stripped_len,
                         &buffer_capacity) == 0);
  assert(buffer[4] == '\0');
  assert(strcmp(buffer, "hola") == 0 && stripped_len == 4 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 2
  assert(strip_to_buffer("   Mundo", 8, &buffer, &stripped_len,
                         &buffer_capacity) == 0);
  assert(buffer[5] == '\0');
  assert(strcmp(buffer, "Mundo") == 0 && stripped_len == 5 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 3
  assert(strip_to_buffer("Mundo   ", 8, &buffer, &stripped_len,
                         &buffer_capacity) == 0);
  assert(buffer[5] == '\0');
  assert(strcmp(buffer, "Mundo") == 0 && stripped_len == 5 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 4
  assert(strip_to_buffer("\t\n \v\f\rTexto\f \n\t\r\v", 17, &buffer,
                         &stripped_len, &buffer_capacity) == 0);
  assert(buffer[5] == '\0');
  assert(strcmp(buffer, "Texto") == 0 && stripped_len == 5 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 5
  assert(strip_to_buffer("\t\n \v\f\r\f \n\t\r\v", 12, &buffer, &stripped_len,
                         &buffer_capacity) == 0);
  assert(buffer[0] == '\0');
  assert(strcmp(buffer, "") == 0 && stripped_len == 0 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 6
  assert(strip_to_buffer("", 0, &buffer, &stripped_len, &buffer_capacity) == 0);
  assert(buffer[0] == '\0');
  assert(strcmp(buffer, "") == 0 && stripped_len == 0 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 7
  assert(strip_to_buffer("Hola mundo!", 11, &buffer, &stripped_len,
                         &buffer_capacity) == 0);
  assert(buffer[11] == '\0');
  assert(strcmp(buffer, "Hola mundo!") == 0 && stripped_len == 11 &&
         buffer_capacity >= stripped_len + 1);

  // subtest 8
  assert(
      strip_to_buffer("\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v",
                      39, &buffer, &stripped_len, &buffer_capacity) == 0);
  assert(buffer[25] == '\0');
  assert(strcmp(buffer, "H o l a\f \n\t\r\v M u n d o !") == 0 &&
         stripped_len == 25 && buffer_capacity >= stripped_len + 1);

  // subtest 9
  assert(strip_to_buffer("\t\n \v\f\rA\f \n\t\r\v", 13, &buffer, &stripped_len,
                         &buffer_capacity) == 0);
  assert(buffer[1] == '\0');
  assert(strcmp(buffer, "A") == 0 && stripped_len == 1 &&
         buffer_capacity >= stripped_len + 1);

  free(buffer);
  printf("\t- strip_to_buffer: PASSED\n");
}

void test_split_to_buffer() {
  size_t number_of_tokens = 0;
  size_t char_size = sizeof(char);
  int status_code = -1;

  char **array_of_tokens = NULL;
  size_t array_of_tokens_capacity = 1;
  assert(init_array((void **)&array_of_tokens, 1, sizeof(char *)) == ARRAY_OK);

  char *buffer = NULL;
  size_t buffer_capacity = 1;
  assert(init_array((void **)&buffer, 1, char_size) == ARRAY_OK);

  // subtest 1
  char *test_split[] = {"hola", "amigo", "como", "estas"};
  status_code = split_to_buffer("hola amigo como estas", 21, ' ',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);

  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 1.1
  status_code = split_to_buffer("   hola   amigo   como    estas   ", 34, ' ',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 1.2
  status_code = split_to_buffer("hola      amigo como      estas   ", 34, ' ',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 1.3
  status_code = split_to_buffer("          hola    amigo como estas", 34, ' ',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 2
  assert(resize_array((void **)&buffer, 1, char_size) == ARRAY_OK);
  buffer_capacity = 1;
  assert(resize_array((void **)&array_of_tokens, 1, sizeof(char *)) ==
         ARRAY_OK);
  array_of_tokens_capacity = 1;
  char *test_split2[] = {"a"};
  status_code =
      split_to_buffer("a", 1, ' ', &array_of_tokens, &array_of_tokens_capacity,
                      &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 2);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.1
  status_code = split_to_buffer("   a", 4, ' ', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 2);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.2
  status_code = split_to_buffer("a   ", 4, ' ', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 2);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.3
  status_code = split_to_buffer("  a ", 4, ' ', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 2);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.4
  status_code =
      split_to_buffer("a", 1, ',', &array_of_tokens, &array_of_tokens_capacity,
                      &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 2);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 3
  assert(resize_array((void **)&buffer, 1, char_size) == ARRAY_OK);
  buffer_capacity = 1;
  assert(resize_array((void **)&array_of_tokens, 1, sizeof(char *)) ==
         ARRAY_OK);
  array_of_tokens_capacity = 1;
  char *test_split3[] = {"1", "2", "3", "4"};
  status_code = split_to_buffer("1,2,3,4", 7, ',', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 8);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 3.1
  status_code = split_to_buffer(",,,1,2,3,4,,,", 13, ',', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 8);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 3.2
  status_code = split_to_buffer("1-2-3-4", 7, '-', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 8);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 3.3
  status_code = split_to_buffer("1\n2\n3\n4", 7, '\n', &array_of_tokens,
                                &array_of_tokens_capacity, &number_of_tokens,
                                &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 4);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 8);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 4
  assert(resize_array((void **)&buffer, 1, char_size) == ARRAY_OK);
  buffer_capacity = 1;
  assert(resize_array((void **)&array_of_tokens, 1, sizeof(char *)) ==
         ARRAY_OK);
  array_of_tokens_capacity = 1;

  char *test_split4[] = {"hola_amigo_como_estas"};
  status_code = split_to_buffer("hola_amigo_como_estas", 21, ',',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  // subtest 4.1
  status_code = split_to_buffer("hola_amigo_como_estas", 21, ' ',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  // subtest 4.2
  status_code = split_to_buffer("hola_amigo_como_estas,", 22, ',',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  // subtest 4.2
  status_code = split_to_buffer(",hola_amigo_como_estas", 22, ',',
                                &array_of_tokens, &array_of_tokens_capacity,
                                &number_of_tokens, &buffer, &buffer_capacity);
  assert(status_code == 0);
  assert(number_of_tokens == 1);
  assert(number_of_tokens == array_of_tokens_capacity);
  assert(buffer_capacity == 22);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  free_matrix((void ***)&array_of_tokens);

  printf("\t- split_to_buffer: PASSED\n");
}

void test_split_in_place() {
  size_t char_size = sizeof(char);
  size_t ptr_to_char_size = sizeof(char *);

  char *buffer = NULL;
  size_t buffer_capacity = 40;
  assert(init_array((void **)&buffer, buffer_capacity, char_size) == ARRAY_OK);

  char **array_of_tokens = NULL;
  size_t array_of_tokens_capacity = 1;
  assert(init_array((void **)&array_of_tokens, array_of_tokens_capacity,
                    ptr_to_char_size) == ARRAY_OK);

  size_t number_of_tokens = 0;

  // subtest 1
  char *test_split[] = {"hola", "amigo", "como", "estas"};
  strcpy(buffer, "hola amigo como estas");
  assert(split_in_place(&buffer, 21, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < number_of_tokens; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 1.2
  strcpy(buffer, "   hola   amigo   como    estas   ");
  assert(split_in_place(&buffer, 34, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < number_of_tokens; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 1.3
  strcpy(buffer, "hola      amigo como      estas   ");
  assert(split_in_place(&buffer, 34, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < number_of_tokens; i++)
    assert(strcmp(test_split[i], array_of_tokens[i]) == 0);

  // subtest 2
  char *test_split2[] = {"a"};
  assert(resize_array((void **)&array_of_tokens, 1, ptr_to_char_size) ==
         ARRAY_OK);
  array_of_tokens_capacity = 1;

  strcpy(buffer, "a");
  assert(split_in_place(&buffer, 1, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.2
  strcpy(buffer, "   a");
  assert(split_in_place(&buffer, 4, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.3
  strcpy(buffer, "   a");
  assert(split_in_place(&buffer, 4, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.4
  strcpy(buffer, "  a ");
  assert(split_in_place(&buffer, 4, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 2.5
  strcpy(buffer, "a");
  assert(split_in_place(&buffer, 1, ',', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split2[0], array_of_tokens[0]) == 0);

  // subtest 3
  char *test_split3[] = {"1", "2", "3", "4"};
  assert(resize_array((void **)&array_of_tokens, 1, ptr_to_char_size) ==
         ARRAY_OK);
  array_of_tokens_capacity = 1;

  strcpy(buffer, "1,2,3,4");
  assert(split_in_place(&buffer, 7, ',', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 3.1
  strcpy(buffer, ",,,1,2,3,4,,,");
  assert(split_in_place(&buffer, 13, ',', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 3.2
  strcpy(buffer, "1-2-3-4");
  assert(split_in_place(&buffer, 7, '-', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 3.3
  strcpy(buffer, "1\n2\n3\n4");
  assert(split_in_place(&buffer, 7, '\n', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);
  assert(number_of_tokens == 4);
  assert(array_of_tokens_capacity >= number_of_tokens);
  for (size_t i = 0; i < 4; i++)
    assert(strcmp(test_split3[i], array_of_tokens[i]) == 0);

  // subtest 4
  char *test_split4[] = {"hola_amigo_como_estas"};
  assert(resize_array((void **)&array_of_tokens, 1, ptr_to_char_size) ==
         ARRAY_OK);
  array_of_tokens_capacity = 1;

  strcpy(buffer, "hola_amigo_como_estas");
  assert(split_in_place(&buffer, 21, ',', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);

  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  // subtest 4.1
  assert(split_in_place(&buffer, 21, ' ', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);

  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  // subtest 4.2
  strcpy(buffer, ",hola_amigo_como_estas");
  assert(split_in_place(&buffer, 22, ',', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);

  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  // subtest 4.2
  strcpy(buffer, "hola_amigo_como_estas,");
  assert(split_in_place(&buffer, 22, ',', &array_of_tokens,
                        &array_of_tokens_capacity, &number_of_tokens) == 0);

  assert(number_of_tokens == 1);
  assert(array_of_tokens_capacity >= number_of_tokens);
  assert(strcmp(test_split4[0], array_of_tokens[0]) == 0);

  free(buffer);
  free(array_of_tokens);
  printf("\t- split_in_place: PASSED\n");
}