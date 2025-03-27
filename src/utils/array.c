#include <stdlib.h>
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
  return resize_array(ptr_to_array , size, type_size);
}