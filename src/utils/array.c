#include <stdlib.h>
#include <string.h>
#include "../../include/utils/array.h"

ArrayStatus resize_array(void **ptr_to_array, const size_t new_size, const size_t type_size) {
  if (ptr_to_array == NULL)
    return ARRAY_ERR_NULL;
  if (new_size == 0)
    return ARRAY_ERR_ZERO;
  if (type_size == 0)
    return ARRAY_ERR_TYPE_S_ZERO;
  void *tmp_ptr = realloc(*ptr_to_array, new_size * type_size);
  if (!tmp_ptr)
    return ARRAY_ERR_ALLOC;
  *ptr_to_array = tmp_ptr;
  tmp_ptr = NULL;
  return ARRAY_OK;
}

ArrayStatus init_array(void **ptr_to_array, const size_t size, const size_t type_size) {
  if (ptr_to_array == NULL)
    return ARRAY_ERR_NULL;
  if (*ptr_to_array != NULL)
    return ARRAY_ERR_IN_USE;
  return resize_array(ptr_to_array, size, type_size);
}

int generic_swap(void *array, const size_t array_size, const size_t type_size,
                const size_t i, const size_t j) {
  if (array == NULL)
    return 1;
  if (array_size == 0)
    return 2;
  if (type_size == 0)
    return 3;
  if (i == j)
    return 0;
  if (i < 0 || j < 0 || i >= array_size || j >= array_size)
    return 5;

  char *ptr = (char *)array;
  char *a = ptr + i * type_size;
  char *b = ptr + j * type_size;

  char tmp_stack_mem[128];
  void *tmp = (type_size <= 128) ? tmp_stack_mem : malloc(type_size);
  if (tmp == NULL && type_size > 128)
    return 6;

  memcpy(tmp, a, type_size);
  memcpy(a, b, type_size);
  memcpy(b, tmp, type_size);

  if (tmp != tmp_stack_mem)
    free(tmp);

  return 0;
}