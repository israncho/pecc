#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/test/evo_comp/test_genetic_algorithm.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/tsp/euclidean.h"
#include "../../include/utils/input_output.h"
#include "../../include/utils/mytime.h"

void test_genetic_algorithm() {
  printf("Testing: genetic_algorithm\n");

  double start = get_wall_time();
  test_population_fitness_computing_1_thread();
  double elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_1_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_2_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_2_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_3_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_3_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_4_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_4_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_5_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_5_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_6_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_6_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_7_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_7_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_population_fitness_computing_8_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- population_fitness_computing_8_thread: PASSED [%.6f secs]\n",
         elapsed_time);
}

static inline void
test_threaded_population_fitness_computing(const size_t n_threads) {
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  assert(read_file("instances/euc_TSP/pr152.tsp", &array_of_lines,
                   &num_of_lines) == FILE_READ_SUCCESS);
  tsp_euc_instance *pr152_instance = NULL;
  assert(init_tsp_euc_instance(array_of_lines, num_of_lines, &pr152_instance) ==
         0);

  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);

  ga_execution exec;
  exec.n_threads = n_threads;
  exec.population = NULL;
  exec.offspring = NULL;
  exec.selected_parents_indexes = NULL;
  exec.population_size = randsize_t_i(1223, 1227, &state);
  exec.codification_size = pr152_instance->number_of_cities - 1;
  exec.generations = 400;
  exec.mem = NULL;
  assert(setup_dynamic_mem_for_ga_execution(&exec, sizeof(size_t),
                                            alignof(size_t)) == 0);
  ga_workspace *workspace_array = NULL;
  assert(setup_dynamic_mem_for_ga_workspace(&workspace_array, &exec, n_threads,
                                            0, 0, 0, 0, 0, sizeof(size_t),
                                            alignof(size_t)) == 0);

  for (size_t i = 0; i < n_threads; i++)
    set_up_seed(&workspace_array[i].state, 0, 0, i);

  fill_and_shuffle_population_of_permutations(
      exec.offspring, exec.population_size, exec.codification_size, &state);

  // To do

  for (size_t i = 0; i < n_threads; i++)
    free(workspace_array[i].mem);

  free(workspace_array);
  free(exec.mem);

  free_tsp_euc_instance_content(pr152_instance);
  free(pr152_instance);

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
}

void test_population_fitness_computing_1_thread() {
  test_threaded_population_fitness_computing(1);
}

void test_population_fitness_computing_2_thread() {
  test_threaded_population_fitness_computing(2);
}

void test_population_fitness_computing_3_thread() {
  test_threaded_population_fitness_computing(3);
}

void test_population_fitness_computing_4_thread() {
  test_threaded_population_fitness_computing(4);
}

void test_population_fitness_computing_5_thread() {
  test_threaded_population_fitness_computing(5);
}

void test_population_fitness_computing_6_thread() {
  test_threaded_population_fitness_computing(6);
}

void test_population_fitness_computing_7_thread() {
  test_threaded_population_fitness_computing(7);
}

void test_population_fitness_computing_8_thread() {
  test_threaded_population_fitness_computing(8);
}