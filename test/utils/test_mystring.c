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

static void verify_strip_in_place_operation(const char *unstripped_str,
                                            const size_t unstripped_str_len,
                                            const char *stripped_str,
                                            const size_t stripped_str_len,
                                            char **ptr_to_buffer) {
  assert(unstripped_str != NULL);
  assert(stripped_str != NULL);
  assert(ptr_to_buffer != NULL);
  assert(*ptr_to_buffer != NULL);
  assert(resize_array((void **)ptr_to_buffer, unstripped_str_len + 1,
                      sizeof(char)) == ARRAY_OK);

  size_t computed_str_len = unstripped_str_len;

  char *buffer = *ptr_to_buffer;
  strcpy(buffer, unstripped_str);
  assert(strcmp(buffer, unstripped_str) == 0);
  assert(buffer[unstripped_str_len] == '\0');

  assert(strip_in_place(&buffer, &computed_str_len) == 0);

  assert(computed_str_len == stripped_str_len);
  assert(buffer[stripped_str_len] == '\0');
  assert(strcmp(buffer, stripped_str) == 0);
}

void test_strip_in_place() {
  size_t char_size = sizeof(char);

  char *buffer = NULL;
  assert(init_array((void **)&buffer, 1, char_size) == ARRAY_OK);

  verify_strip_in_place_operation("   hola   ", 10, "hola", 4, &buffer);

  verify_strip_in_place_operation("   Mundo", 8, "Mundo", 5, &buffer);

  verify_strip_in_place_operation("Mundo   ", 8, "Mundo", 5, &buffer);

  verify_strip_in_place_operation("\t\n \v\f\rTexto\f \n\t\r\v", 17, "Texto", 5,
                                  &buffer);

  verify_strip_in_place_operation("\t\n \v\f\r\f \n\t\r\v", 12, "", 0, &buffer);

  verify_strip_in_place_operation("", 0, "", 0, &buffer);

  verify_strip_in_place_operation("Hola mundo!", 11, "Hola mundo!", 11,
                                  &buffer);

  verify_strip_in_place_operation(
      "\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v", 39,
      "H o l a\f \n\t\r\v M u n d o !", 25, &buffer);

  verify_strip_in_place_operation("\t\n \v\f\rA\f \n\t\r\v", 13, "A", 1,
                                  &buffer);

  free(buffer);
  printf("\t- strip_in_place: PASSED\n");
}

static void verify_strip_to_buffer_operation(
    const char *unstripped_str, const size_t unstripped_str_len,
    const char *stripped_str, const size_t stripped_str_len,
    char **ptr_to_buffer, const size_t initial_buffer_capacity) {
  assert(unstripped_str != NULL);
  assert(stripped_str != NULL);
  assert(ptr_to_buffer != NULL);
  assert(*ptr_to_buffer != NULL);

  size_t buffer_capacity = initial_buffer_capacity;
  assert(resize_array((void **)ptr_to_buffer, initial_buffer_capacity,
                      sizeof(char)) == ARRAY_OK);
  size_t computed_str_len = 0;

  assert(strip_to_buffer(unstripped_str, unstripped_str_len, ptr_to_buffer,
                         &computed_str_len, &buffer_capacity) == 0);

  char *buffer = *ptr_to_buffer;
  assert(computed_str_len == stripped_str_len);
  assert(buffer[stripped_str_len] == '\0');
  assert(strcmp(buffer, stripped_str) == 0);
  assert(buffer_capacity >= stripped_str_len + 1);
}

void test_strip_to_buffer() {
  size_t char_size = sizeof(char);

  char *buffer = NULL;

  assert(init_array((void **)&buffer, 1, char_size) == 0);

  verify_strip_to_buffer_operation("   hola   ", 10, "hola", 4, &buffer, 2);

  verify_strip_to_buffer_operation("   Mundo", 8, "Mundo", 5, &buffer, 6);

  verify_strip_to_buffer_operation("Mundo   ", 8, "Mundo", 5, &buffer, 1);

  verify_strip_to_buffer_operation("\t\n \v\f\rTexto\f \n\t\r\v", 17, "Texto",
                                   5, &buffer, 10);

  verify_strip_to_buffer_operation("\t\n \v\f\r\f \n\t\r\v", 12, "", 0, &buffer,
                                   1);

  verify_strip_to_buffer_operation("", 0, "", 0, &buffer, 2);

  verify_strip_to_buffer_operation("Hola mundo!", 11, "Hola mundo!", 11,
                                   &buffer, 2);

  verify_strip_to_buffer_operation(
      "\t\n  \v\f\rH o l a\f \n\t\r\v M u n d o !\f  \n\t\r\v", 39,
      "H o l a\f \n\t\r\v M u n d o !", 25, &buffer, 5);

  verify_strip_to_buffer_operation("\t\n \v\f\rA\f \n\t\r\v", 13, "A", 1,
                                   &buffer, 1);

  free(buffer);
  printf("\t- strip_to_buffer: PASSED\n");
}

static void verify_split_to_buffer_operation(
    const char *target[], const char *str, const size_t str_len,
    const char delimiter, char ***ptr_to_tokens_array,
    const size_t array_of_tokens_initial_capacity, const size_t expected_tokens,
    char **ptr_to_buffer, const size_t buffer_initial_capacity,
    const size_t minimum_expected_buffer_capacity) {
  assert(target != NULL);
  assert(str != NULL);
  assert(ptr_to_tokens_array != NULL);
  assert(*ptr_to_tokens_array != NULL);
  assert(ptr_to_buffer != NULL);
  assert(*ptr_to_buffer != NULL);

  size_t computed_number_of_tokens = 0;
  size_t tokens_array_capacity = array_of_tokens_initial_capacity;
  size_t buffer_capacity = buffer_initial_capacity;

  assert(resize_array((void **)ptr_to_tokens_array, tokens_array_capacity,
                      sizeof(char *)) == ARRAY_OK);
  assert(resize_array((void **)ptr_to_buffer, buffer_capacity, sizeof(char)) ==
         ARRAY_OK);
  assert(split_to_buffer(str, str_len, delimiter, ptr_to_tokens_array,
                         &tokens_array_capacity, &computed_number_of_tokens,
                         ptr_to_buffer, &buffer_capacity) == 0);
  char **tokens_array = *ptr_to_tokens_array;
  assert(computed_number_of_tokens == expected_tokens);
  assert(tokens_array_capacity >= computed_number_of_tokens);
  assert(buffer_capacity >= minimum_expected_buffer_capacity);
  for (size_t i = 0; i < expected_tokens; i++)
    assert(strcmp(target[i], tokens_array[i]) == 0);
}

void test_split_to_buffer() {
  size_t char_size = sizeof(char);

  char **array_of_tokens = NULL;
  assert(init_array((void **)&array_of_tokens, 1, sizeof(char *)) == ARRAY_OK);

  char *buffer = NULL;
  assert(init_array((void **)&buffer, 1, char_size) == ARRAY_OK);

  // subtest 1
  char *test_split[] = {"hola", "amigo", "como", "estas"};

  verify_split_to_buffer_operation((const char **)test_split,
                                   "hola amigo como estas", 21, ' ',
                                   &array_of_tokens, 1, 4, &buffer, 1, 22);

  verify_split_to_buffer_operation(
      (const char **)test_split, "   hola   amigo   como    estas   ", 34, ' ',
      &array_of_tokens, 2, 4, &buffer, 22, 22);

  verify_split_to_buffer_operation(
      (const char **)test_split, "hola      amigo como      estas   ", 34, ' ',
      &array_of_tokens, 4, 4, &buffer, 50, 22);

  verify_split_to_buffer_operation(
      (const char **)test_split, "          hola    amigo como estas", 34, ' ',
      &array_of_tokens, 10, 4, &buffer, 1, 22);

  // subtest 2
  char *test_split2[] = {"a"};
  verify_split_to_buffer_operation((const char **)test_split2, "a", 1, ' ',
                                   &array_of_tokens, 1, 1, &buffer, 1, 2);

  verify_split_to_buffer_operation((const char **)test_split2, "   a", 4, ' ',
                                   &array_of_tokens, 2, 1, &buffer, 2, 2);

  verify_split_to_buffer_operation((const char **)test_split2, "a   ", 4, ' ',
                                   &array_of_tokens, 4, 1, &buffer, 4, 2);

  verify_split_to_buffer_operation((const char **)test_split2, "  a ", 4, ' ',
                                   &array_of_tokens, 1, 1, &buffer, 1, 2);

  verify_split_to_buffer_operation((const char **)test_split2, "a", 1, ',',
                                   &array_of_tokens, 1, 1, &buffer, 1, 2);

  // subtest 3
  char *test_split3[] = {"1", "2", "3", "4"};
  verify_split_to_buffer_operation((const char **)test_split3, "1,2,3,4", 7,
                                   ',', &array_of_tokens, 1, 4, &buffer, 1, 8);

  verify_split_to_buffer_operation((const char **)test_split3,
                                   ",,,1,,2,3,,4,,,", 15, ',', &array_of_tokens,
                                   4, 4, &buffer, 8, 8);

  verify_split_to_buffer_operation((const char **)test_split3, "1-2-3-4", 7,
                                   '-', &array_of_tokens, 8, 4, &buffer, 16, 8);

  verify_split_to_buffer_operation((const char **)test_split3, "1\n2\n3\n4", 7,
                                   '\n', &array_of_tokens, 1, 4, &buffer, 1, 8);

  // subtest 4
  char *test_split4[] = {"hola_amigo_como_estas"};
  verify_split_to_buffer_operation((const char **)test_split4,
                                   "hola_amigo_como_estas", 21, ',',
                                   &array_of_tokens, 1, 1, &buffer, 1, 22);

  verify_split_to_buffer_operation((const char **)test_split4,
                                   "hola_amigo_como_estas", 21, ' ',
                                   &array_of_tokens, 2, 1, &buffer, 22, 22);

  verify_split_to_buffer_operation((const char **)test_split4,
                                   "hola_amigo_como_estas,", 22, ',',
                                   &array_of_tokens, 4, 1, &buffer, 30, 22);

  verify_split_to_buffer_operation((const char **)test_split4,
                                   ",hola_amigo_como_estas", 22, ',',
                                   &array_of_tokens, 1, 1, &buffer, 5, 22);

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