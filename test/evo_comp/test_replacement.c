#include <assert.h>
#include <omp.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/test/evo_comp/test_replacement.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/evo_comp/replacement.h"
#include "../../include/utils/mytime.h"

void test_replacement() {
  printf("Testing: replacement\n");

  double start = get_wall_time();
  test_full_generational_replacement_1_thread();
  double elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_1_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_2_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_2_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_3_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_3_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_4_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_4_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_5_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_5_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_6_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_6_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_7_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_7_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_8_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_8_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_1_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_1_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_2_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_2_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_3_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_3_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_4_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_4_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_5_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_5_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_6_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_6_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_7_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_7_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_8_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_8_thread: PASSED [%.6f secs]\n",
         elapsed_time);
}

static inline bool same_array(const size_t *arr1, const size_t *arr2,
                              const size_t array_len) {
  for (size_t i = 0; i < array_len; i++)
    if (arr1[i] != arr2[i])
      return false;
  return true;
}

static inline void test_threaded_full_gen_replacement(const size_t n_threads,
                                                      const bool elitism) {

  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);

  ga_execution exec;
  exec.mem = NULL;
  exec.population = NULL;
  exec.offspring = NULL;
  exec.current_best = NULL;
  exec.selected_parents_indexes = NULL;
  exec.codification_size = 200;
  // exec.codification_size = 50;
  exec.codification_entry_size = sizeof(size_t);
  exec.codification_entry_alignment = alignof(size_t);
  exec.population_size = 1024;
  // exec.population_size = 256;
  exec.n_threads = n_threads;
  exec.mutation_rate = 0.0;

  assert(setup_dynamic_mem_for_ga_execution(&exec) == 0);

  ga_workspace *workspace_array = NULL;
  assert(setup_dynamic_mem_for_ga_workspace(&workspace_array, &exec, n_threads,
                                            0, 0, 0, 0, 0) == 0);

  for (size_t i = 0; i < n_threads; i++)
    set_up_seed(&workspace_array[i].state, 0, 0, i);

  fill_and_shuffle_population_of_permutations(
      exec.offspring, exec.population_size, exec.codification_size, &state);

  fill_and_shuffle_population_of_permutations(
      exec.population, exec.population_size, exec.codification_size, &state);

  size_t *current_best_codification = exec.current_best->codification;
  for (size_t i = 0; i < exec.codification_size; i++)
    current_best_codification[i] = i;

  #pragma omp parallel num_threads(n_threads)
  {
    const size_t thread_id = omp_get_thread_num();
    ga_workspace *thread_workspace = &workspace_array[thread_id];

    const size_t beginning =
        thread_workspace->offspring_size_of_previous_threads;
    const size_t end = thread_workspace->thread_offspring_size + beginning;

    for (size_t _ = 0; _ < 50; _++) {
      for (size_t i = beginning; i < end; i++) {
        if (elitism && thread_id == 0 && i == 0) {
          shuffle_array_of_size_t(exec.current_best->codification,
                                  exec.codification_size,
                                  &thread_workspace->state);
          exec.current_best->fitness =
              random_double(&thread_workspace->state) * 100000.0;
        }
        shuffle_array_of_size_t(exec.offspring[i].codification,
                                exec.codification_size,
                                &thread_workspace->state);
        exec.offspring[i].fitness =
            random_double(&thread_workspace->state) * 100000.0;
        assert(!same_array(exec.offspring[i].codification,
                           exec.population[i].codification,
                           exec.codification_size));
      }

      if (elitism)
        full_gen_replacement_elitism(&exec, thread_workspace);
      else
        full_generational_replacement(&exec, thread_workspace);

      for (size_t i = beginning; i < end; i++) {
        if (elitism && thread_id == 0 && i == 0) {
          assert(same_array(exec.current_best->codification,
                            exec.population[i].codification,
                            exec.codification_size));
          assert(exec.current_best->fitness == exec.population[i].fitness);
        } else {
          assert(same_array(exec.offspring[i].codification,
                            exec.population[i].codification,
                            exec.codification_size));
          assert(exec.offspring[i].fitness == exec.population[i].fitness);
        }
      }
    #pragma omp barrier
    }
  }

  for (size_t i = 0; i < n_threads; i++)
    free(workspace_array[i].mem);
  free(workspace_array);
  free(exec.mem);
}

void test_full_generational_replacement_1_thread() {
  test_threaded_full_gen_replacement(1, false);
}

void test_full_generational_replacement_2_thread() {
  test_threaded_full_gen_replacement(2, false);
}

void test_full_generational_replacement_3_thread() {
  test_threaded_full_gen_replacement(3, false);
}

void test_full_generational_replacement_4_thread() {
  test_threaded_full_gen_replacement(4, false);
}

void test_full_generational_replacement_5_thread() {
  test_threaded_full_gen_replacement(5, false);
}

void test_full_generational_replacement_6_thread() {
  test_threaded_full_gen_replacement(6, false);
}

void test_full_generational_replacement_7_thread() {
  test_threaded_full_gen_replacement(7, false);
}

void test_full_generational_replacement_8_thread() {
  test_threaded_full_gen_replacement(8, false);
}

void test_full_gen_replacement_elitism_1_thread() {
  test_threaded_full_gen_replacement(1, true);
}

void test_full_gen_replacement_elitism_2_thread() {
  test_threaded_full_gen_replacement(2, true);
}

void test_full_gen_replacement_elitism_3_thread() {
  test_threaded_full_gen_replacement(3, true);
}

void test_full_gen_replacement_elitism_4_thread() {
  test_threaded_full_gen_replacement(4, true);
}

void test_full_gen_replacement_elitism_5_thread() {
  test_threaded_full_gen_replacement(5, true);
}

void test_full_gen_replacement_elitism_6_thread() {
  test_threaded_full_gen_replacement(6, true);
}

void test_full_gen_replacement_elitism_7_thread() {
  test_threaded_full_gen_replacement(7, true);
}

void test_full_gen_replacement_elitism_8_thread() {
  test_threaded_full_gen_replacement(8, true);
}