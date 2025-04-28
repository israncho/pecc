#include <stddef.h>
#include <stdalign.h>
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/utils/array.h"

int setup_from_prealloc_mem_arrays_for_ga_execution(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity, ga_execution *exec,
    const size_t codification_entry_size,
    const size_t codification_entry_alignment) {
  const size_t population_size = exec->population_size;
  const size_t codification_size = exec->codification_size;

  int status_code = setup_population_from_prealloc_mem(
      ptr_to_mem, ptr_to_mem_capacity, &exec->population, population_size,
      codification_size, codification_entry_size, codification_entry_alignment);
  if (status_code != 0)
    return 1;

  status_code = setup_population_from_prealloc_mem(
      ptr_to_mem, ptr_to_mem_capacity, &exec->offspring, population_size,
      codification_size, codification_entry_size, codification_entry_alignment);
  if (status_code != 0)
    return 2;

  status_code = setup_array_from_prealloc_mem(
      ptr_to_mem, ptr_to_mem_capacity, (void **)&exec->selected_parents_indexes,
      population_size + (population_size % 2), sizeof(size_t), alignof(size_t));
  if (status_code != 0)
    return 3;

  return 0;
}
