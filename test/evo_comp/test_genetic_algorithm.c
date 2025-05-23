#include <assert.h>
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include "../../include/test/evo_comp/test_genetic_algorithm.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/tsp/euclidean.h"
#include "../../include/utils/input_output.h"
#include "../../include/utils/myrandom.h"
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
  exec.codification_entry_size = sizeof(size_t);
  exec.codification_entry_alignment = alignof(size_t);
  exec.n_threads = n_threads;
  exec.population = NULL;
  exec.offspring = NULL;
  exec.selected_parents_indexes = NULL;
  exec.population_size = randsize_t_i(5569, 5573, &state);
  exec.codification_size = pr152_instance->number_of_cities - 1;
  exec.mem = NULL;
  assert(setup_dynamic_mem_for_ga_execution(&exec) == 0);
  ga_workspace *workspace_array = NULL;
  assert(setup_dynamic_mem_for_ga_workspace(&workspace_array, &exec, n_threads,
                                            0, 0, 0, 0, 0) == 0);

  for (size_t i = 0; i < n_threads; i++)
    set_up_seed(&workspace_array[i].state, 0, 0, i);

  fill_and_shuffle_population_of_permutations(
      exec.offspring, exec.population_size, exec.codification_size, &state);

  #pragma omp parallel num_threads(n_threads)
  {
    const size_t thread_id = omp_get_thread_num();
    ga_workspace *thread_workspace = &workspace_array[thread_id];

    const size_t beginning =
        thread_workspace->offspring_size_of_previous_threads;
    const size_t end = thread_workspace->thread_offspring_size + beginning;

    for (size_t _ = 0; _ < 50; _++) {

      for (size_t i = beginning; i < end; i++) {
        shuffle_array_of_size_t(exec.offspring[i].codification,
                                exec.codification_size,
                                &thread_workspace->state);
        exec.offspring[i].fitness = random_double(&thread_workspace->state) * 100000.0;
      }

      assert(population_fitness_computing(exec.offspring, thread_workspace,
                                          pr152_instance, tour_distance) == 0);
        
      individual *thread_best = thread_workspace->ptr_to_thread_best;
      size_t same = 0;
      for (size_t i = beginning; i < end; i++) {
        individual *curr_individual = &exec.offspring[i];
        const double curr_individual_fitness = curr_individual->fitness;
        assert(curr_individual_fitness == tour_distance(curr_individual->codification, pr152_instance, NULL));
        assert(thread_best->fitness <= curr_individual_fitness);
        same += (thread_best == curr_individual) ? 1 : 0;
      }
      assert(same == 1);
      #pragma omp barrier
    }
  }

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