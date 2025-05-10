#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/test/evo_comp/test_selection.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/selection.h"
#include "../../include/utils/array.h"
#include "../../include/utils/mytime.h"

void test_selection() {
  printf("Testing: selection\n");

  double start = get_wall_time();
  test_random_selection();
  double elapsed_time = get_wall_time() - start;
  printf("\t- random_selection: PASSED [%.6f secs]\n", elapsed_time);
}

void test_random_selection() {
  ga_workspace workspace;
  set_up_seed(&workspace.state, 0, 0, 0);

  ga_execution exec;
  exec.selected_parents_indexes = NULL;

  const size_t max_array_len = 1000;
  const size_t total_mem_needed =
      sizeof(size_t) * max_array_len + alignof(size_t);

  void *mem = NULL;
  assert(init_array(&mem, total_mem_needed, 1) == ARRAY_OK);

  size_t min_count = 0;
  size_t max_count = 0;
  for (size_t _ = 0; _ < 1000; _++) {
    void *mem_ = mem;
    size_t mem_capacity = total_mem_needed;

    exec.population_size = randsize_t_i(990, max_array_len, &workspace.state);
    const size_t selected_parents_len =
        exec.population_size + (exec.population_size & 1);
    assert(setup_array_from_prealloc_mem(
               &mem_, &mem_capacity, (void **)&exec.selected_parents_indexes,
               selected_parents_len, sizeof(size_t),
               alignof(size_t)) == ARRAY_OK);

    for (size_t i = 0; i < selected_parents_len; i++)
      exec.selected_parents_indexes[i] = 0;

    size_t zero_count = 0;

    assert(random_selection(&exec, &workspace) == 0);
    for (size_t i = 0; i < selected_parents_len; i++) {
      size_t value = exec.selected_parents_indexes[i];
      assert(0 <= value && value < exec.population_size);
      if (value == 0) {
        zero_count++;
        min_count++;
      }
      if (value == exec.population_size - 1)
        max_count++;
    }
    assert(zero_count < 10);
  }
  assert(min_count > 10);
  assert(max_count > 10);
  free(mem);
}