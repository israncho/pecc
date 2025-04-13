#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/utils/array.h"

ArrayStatus resize_array(void **ptr_to_array, const size_t new_size,
                         const size_t type_size) {
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

ArrayStatus init_array(void **ptr_to_array, const size_t size,
                       const size_t type_size) {
  if (ptr_to_array == NULL)
    return ARRAY_ERR_NULL;
  if (*ptr_to_array != NULL)
    return ARRAY_ERR_IN_USE;
  return resize_array(ptr_to_array, size, type_size);
}

ArrayStatus
setup_array_from_prealloc_mem(void **ptr_to_mem, size_t *ptr_to_mem_capacity,
                              void **ptr_to_array, const size_t size,
                              const size_t type_size, const size_t alignment) {

  const uintptr_t beg_mem = (uintptr_t)*ptr_to_mem;
  const size_t data_mem_needed = type_size * size;
  uintptr_t align_addr = beg_mem;

  if (alignment & (alignment - 1))
    align_addr = (align_addr + (alignment - 1)) & ~(alignment - 1);
  else {
    uintptr_t offset = alignment - (align_addr % alignment);
    align_addr += (offset == alignment) ? 0 : offset;
  }

  const size_t padding = align_addr - beg_mem;
  const size_t total_mem_requiered = data_mem_needed + padding;

  if (total_mem_requiered > *ptr_to_mem_capacity)
    return ARRAY_ERR_NO_CAPACITY;

  (*ptr_to_array) = (void *)align_addr;
  (*ptr_to_mem) = (char *)beg_mem + total_mem_requiered;
  (*ptr_to_mem_capacity) -= total_mem_requiered;

  return ARRAY_OK;
}