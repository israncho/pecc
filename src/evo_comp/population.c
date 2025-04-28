#include <stddef.h>
#include <stdlib.h>
#include <stdalign.h>
#include "../../include/evo_comp/population.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myrandom.h"

int fill_and_shuffle_population_of_permutations(individual *population,
                                                const size_t population_size,
                                                const size_t codification_size,
                                                xorshiftr128plus_state *state) {
  if (population == NULL)
    return 1;
  for (size_t i = 0; i < population_size; i++) {
    for (size_t j = 0; j < codification_size; j++) {
      size_t *codification = population[i].codification;
      codification[j] = j;
    }
    shuffle_array_of_size_t(population[i].codification, codification_size,
                            state);
  }
  return 0;
}

int setup_population_from_prealloc_mem(
    void **ptr_to_mem, size_t *ptr_to_mem_capacity,
    individual **ptr_to_population, const size_t population_size,
    const size_t codification_size, const size_t codification_entry_byte_size,
    const size_t alignment) {

  if (setup_array_from_prealloc_mem(ptr_to_mem, ptr_to_mem_capacity,
                                    (void **)ptr_to_population, population_size,
                                    sizeof(individual),
                                    alignof(individual)) != ARRAY_OK)
    return 1;

  individual *population = *ptr_to_population;
  for (size_t i = 0; i < population_size; i++)
    if (setup_array_from_prealloc_mem(
            ptr_to_mem, ptr_to_mem_capacity, &(population[i].codification),
            codification_size, codification_entry_byte_size,
            alignment) != ARRAY_OK)
      return 2;
  return 0;
}