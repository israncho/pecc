#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/test/evo_comp/test_population.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/utils/array.h"
#include "../../include/utils/myrandom.h"
#include "../../include/utils/mytime.h"

void test_population() {
  printf("Testing: population\n");

  double start = get_wall_time();
  test_setup_population_from_prealloc_mem();
  double elapsed_time = get_wall_time() - start;
  printf("\t- setup_population_from_prealloc_mem: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_fill_and_shuffle_population_of_permutations();
  elapsed_time = get_wall_time() - start;
  printf(
      "\t- fill_and_shuffle_population_of_permutations: PASSED [%.6f secs]\n",
      elapsed_time);
}

static inline bool all_elements_present(bool *boolset, const size_t set_size,
                                        const size_t *test_set) {
  for (size_t i = 0; i < set_size; i++)
    boolset[i] = false;
  for (size_t i = 0; i < set_size; i++)
    boolset[test_set[i]] = true;
  for (size_t i = 0; i < set_size; i++)
    if (!boolset[i])
      return false;
  return true;
}

static inline bool same_arrays(const size_t arrays_len, const size_t *arr1,
                               const size_t *arr2) {
  for (size_t i = 0; i < arrays_len; i++)
    if (arr1[i] != arr2[i])
      return false;
  return true;
}

void test_fill_and_shuffle_population_of_permutations() {
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0);

  for (size_t _ = 0; _ < 1000; _++) {
    const size_t population_size = randsize_t_i(50, 150, &state);
    const size_t codification_size = randsize_t_i(50, 150, &state);
    const size_t total_memory_needed =
        population_size *
            (sizeof(individual) + codification_size * sizeof(size_t)) +
        alignof(individual) + alignof(size_t) +
        sizeof(bool) * codification_size + alignof(bool);
    size_t memory_capacity = total_memory_needed;
    void *mem = malloc(total_memory_needed);
    void *mem_ = mem;

    individual *population = NULL;

    setup_population_from_prealloc_mem(&mem_, &memory_capacity, &population,
                                       population_size, codification_size,
                                       sizeof(size_t), alignof(size_t));
    bool *boolset = NULL;
    assert(setup_array_from_prealloc_mem(
               &mem_, &memory_capacity, (void **)&boolset, codification_size,
               sizeof(bool), alignof(bool)) == ARRAY_OK);

    assert(fill_and_shuffle_population_of_permutations(
               population, population_size, codification_size, &state) == 0);
    for (size_t i = 0; i < population_size; i++)
      assert(all_elements_present(boolset, codification_size,
                                  population[i].codification));
                        
    for (size_t i = 0; i < population_size; i++)
      for (size_t j = i + 1; j < population_size; j++) 
         assert(!same_arrays(codification_size, population[i].codification, population[j].codification));

    free(mem);
  }
}

void test_setup_population_from_prealloc_mem() {
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0);

  for (size_t _ = 0; _ < 1000; _++) {
    const size_t population_size = randsize_t_i(150, 250, &state);
    const size_t codification_size = randsize_t_i(150, 250, &state);
    const size_t total_memory_needed =
        population_size *
            (sizeof(individual) + codification_size * sizeof(size_t)) +
        alignof(individual) + alignof(size_t);
    size_t memory_capacity = total_memory_needed;
    void *mem = malloc(total_memory_needed);
    void *mem_ = mem;

    individual *population = NULL;

    assert(setup_population_from_prealloc_mem(
               &mem_, &memory_capacity, &population, population_size,
               codification_size, sizeof(size_t), alignof(size_t)) == 0);
    for (size_t i = 0; i < population_size; i++) {
      size_t *codification = population[i].codification;
      for (size_t j = 0; j < codification_size; j++)
        codification[j] = j;
    }

    for (size_t i = 0; i < population_size; i++) {
      size_t test = 0;
      for (size_t j = 0; j < codification_size; j++) {
        size_t *codification = population[i].codification;
        assert(codification[j] == test && j == test++);
      }
    }
    free(mem);
  }
}