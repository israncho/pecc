#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/test/tsp/test_euclidean.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/evo_comp/population.h"
#include "../../include/tsp/euclidean.h"
#include "../../include/utils/array.h"
#include "../../include/utils/input_output.h"
#include "../../include/utils/matrix.h"
#include "../../include/utils/myrandom.h"
#include "../../include/utils/mytime.h"

void test_euclidean() {
  printf("Testing: tsp_euclidean\n");

  double start = get_wall_time();
  test_init_tsp_euc_instance();
  double elapsed_time = get_wall_time() - start;
  printf("\t- init_tsp_euc_instance: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_euclidean_distance();
  elapsed_time = get_wall_time() - start;
  printf("\t- euclidean_distance: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_tour_distance();
  elapsed_time = get_wall_time() - start;
  printf("\t- tour_distance: PASSED [%.6f secs]\n", elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_1_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_1_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_2_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_2_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_3_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_3_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_4_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_4_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_5_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_5_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_6_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_6_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_7_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_7_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_lsearch_2opt_tour_distance_8_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- lsearch_2opt_tour_distance_8_thread: PASSED [%.6f secs]\n",
         elapsed_time);
}

void test_init_tsp_euc_instance() {
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  assert(read_file("instances/euc_TSP/eil51.tsp", &array_of_lines,
                   &num_of_lines) == FILE_READ_SUCCESS);

  tsp_euc_instance *computed_eil51 = NULL;
  int exit_code =
      init_tsp_euc_instance(array_of_lines, num_of_lines, &computed_eil51);
  assert(exit_code == 0);

  double cities_data[102] = {
      37.0, 52.0, 49.0, 49.0, 52.0, 64.0, 20.0, 26.0, 40.0, 30.0, 21.0, 47.0,
      17.0, 63.0, 31.0, 62.0, 52.0, 33.0, 51.0, 21.0, 42.0, 41.0, 31.0, 32.0,
      5.0,  25.0, 12.0, 42.0, 36.0, 16.0, 52.0, 41.0, 27.0, 23.0, 17.0, 33.0,
      13.0, 13.0, 57.0, 58.0, 62.0, 42.0, 42.0, 57.0, 16.0, 57.0, 8.0,  52.0,
      7.0,  38.0, 27.0, 68.0, 30.0, 48.0, 43.0, 67.0, 58.0, 48.0, 58.0, 27.0,
      37.0, 69.0, 38.0, 46.0, 46.0, 10.0, 61.0, 33.0, 62.0, 63.0, 63.0, 69.0,
      32.0, 22.0, 45.0, 35.0, 59.0, 15.0, 5.0,  6.0,  10.0, 17.0, 21.0, 10.0,
      5.0,  64.0, 30.0, 15.0, 39.0, 10.0, 32.0, 39.0, 25.0, 32.0, 25.0, 55.0,
      48.0, 28.0, 56.0, 37.0, 30.0, 40.0};
  double *cities[51];
  for (size_t i = 0; i < 51; i++) {
    cities[i] = &(cities_data[i * 2]);
  }

  tsp_euc_instance target = {cities,
                             51,
                             2,
                             {},
                             "eil51",
                             "TSP",
                             "51-city problem (Christofides/Eilon)",
                             "EUC_2D"};

  assert(target.number_of_cities == computed_eil51->number_of_cities);
  assert(strcmp(target.name, computed_eil51->name) == 0);
  assert(strcmp(target.type, computed_eil51->type) == 0);
  assert(strcmp(target.comment, computed_eil51->comment) == 0);
  assert(strcmp(target.edge_weight_type, computed_eil51->edge_weight_type) ==
         0);
  for (size_t i = 0; i < 51; i++) {
    assert(target.cities[i][0] == computed_eil51->cities[i][0]);
    assert(target.cities[i][1] == computed_eil51->cities[i][1]);
  }

  for (size_t i = 0; i < 51; i++) {
    double city_i_x = target.cities[i][0];
    double city_i_y = target.cities[i][1];
    for (size_t j = i; j < 51; j++) {
      double city_j_x = target.cities[j][0];
      double city_j_y = target.cities[j][1];
      double euc_distance =
          sqrt(pow(city_i_x - city_j_x, 2) + pow(city_i_y - city_j_y, 2));
      assert(euc_distance == computed_eil51->distances[i][j]);
      assert(euc_distance == computed_eil51->distances[j][i]);
    }
  }

  free_tsp_euc_instance_content(computed_eil51);
  free(computed_eil51);
  computed_eil51 = NULL;
  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  array_of_lines = NULL;
}

void test_euclidean_distance() {
  double p1[2] = {0.0, 0.0};
  double p2[2] = {0.0, 0.0};
  for (size_t i = 0; i < 1000; i++) {
    double distance = i * 1.0;
    p2[1] = distance;
    assert(euclidean_distance(p1, p2, 2) == distance);
  }
  for (size_t i = 0; i < 1000; i++) {
    double distance = sqrt(pow(i, 2) * 2);
    p2[0] = i;
    p2[1] = i;
    assert(euclidean_distance(p1, p2, 2) == distance);
  }
}

void test_tour_distance() {
  const size_t max_n_of_cities = 1500;
  const size_t max_codification_size = max_n_of_cities - 1;
  const size_t min_n_of_cities = 1450;

  tsp_euc_instance instance;
  instance.cities = NULL;
  instance.distances = NULL;
  xorshiftr128plus_state state;
  set_up_seed(&state, 0, 0, 0);
  assert(init_matrix((void ***)&instance.cities, max_n_of_cities, 2,
                     sizeof(double), alignof(double)) == 0);
  assert(init_matrix((void ***)&instance.distances, max_n_of_cities,
                     max_n_of_cities, sizeof(double), alignof(double)) == 0);

  size_t *solution = NULL;
  assert(init_array((void **)&solution, max_codification_size,
                    sizeof(size_t)) == ARRAY_OK);

  for (size_t i = 0; i < max_codification_size; i++)
    solution[i] = i;

  for (size_t _ = 0; _ < 250; _++) {
    const size_t rnd_n = randsize_t_i(min_n_of_cities, max_n_of_cities, &state);
    const size_t curr_n_of_cities = rnd_n + (rnd_n & 1);
    instance.number_of_cities = curr_n_of_cities;

    const size_t half = curr_n_of_cities / 2;
    double curr = 0.0;

    for (size_t i = 0; i < half; i++) {
      instance.cities[i][0] = curr;
      instance.cities[i][1] = 0.0;
      curr += randsize_t_i(5, 50, &state) * 1.0;
    }

    const double base_size = instance.cities[half - 1][0];

    const double height = randsize_t_i(25, 50, &state) * 1.0;

    size_t aux = half - 1;
    for (size_t i = half; i < curr_n_of_cities; i++) {
      instance.cities[i][0] = instance.cities[aux--][0];
      instance.cities[i][1] = height;
    }
    fill_distance_matrix((const double *const *)instance.cities,
                         curr_n_of_cities, 2, instance.distances);
    const double simple_tour_distance = base_size * 2 + height * 2;
    const double computed_tour_distance = tour_distance(solution, &instance, NULL);
    assert(simple_tour_distance == computed_tour_distance);
  }
  free(instance.cities);
  free(instance.distances);
  free(solution);
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
  exec.population_size = 1103;
  // exec.population_size = 32;
  exec.codification_size = pr152_instance->number_of_cities - 1;
  exec.mem = NULL;
  assert(setup_dynamic_mem_for_ga_execution(&exec) == 0);
  ga_workspace *workspace_array = NULL;
  assert(setup_dynamic_mem_for_ga_workspace(&workspace_array, &exec, n_threads,
                                            0, 0, 0, 0, 0) == 0);

  for (size_t i = 0; i < n_threads; i++) {
    set_up_seed(&workspace_array[i].state, 0, 0, i);
    workspace_array[i].local_search_iterations = 1;
  }

  fill_and_shuffle_population_of_permutations(
      exec.offspring, exec.population_size, exec.codification_size, &state);

  double *prev_fitness = NULL;
  assert(init_array((void **)&prev_fitness, exec.population_size,
                    sizeof(double)) == ARRAY_OK);

  #pragma omp parallel num_threads(n_threads)
  {
    const size_t thread_id = omp_get_thread_num();
    ga_workspace *thread_workspace = &workspace_array[thread_id];

    const size_t beginning =
        thread_workspace->offspring_size_of_previous_threads;
    const size_t end = thread_workspace->thread_offspring_size + beginning;

    for (size_t i = beginning; i < end; i++) {
      exec.offspring[i].fitness =
          tour_distance(exec.offspring[i].codification, pr152_instance, NULL);
      prev_fitness[i] = exec.offspring[i].fitness;
    }

    for (size_t _ = 0; _ < 50; _++) {
    //for (size_t _ = 0; _ < 10; _++) {
      assert(population_fitness_computing(&exec, thread_workspace,
                                          pr152_instance,
                                          lsearch_2opt_tour_distance) == 0);

      individual *thread_best = thread_workspace->ptr_to_thread_best;
      size_t same = 0;
      for (size_t i = beginning; i < end; i++) {
        individual *curr_individual = &exec.offspring[i];
        const double curr_individual_prev_fitness = prev_fitness[i];
        const double curr_individual_fitness = curr_individual->fitness;
        const double computed =
            tour_distance(curr_individual->codification, pr152_instance, NULL);
        assert(fabs(curr_individual_fitness - computed) < 1e-8);
        assert(curr_individual_fitness - curr_individual_prev_fitness <= 1e-8);
        assert(thread_best->fitness <= curr_individual_fitness);
        prev_fitness[i] = curr_individual_fitness;
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
  free(prev_fitness);
}

void test_lsearch_2opt_tour_distance_1_thread() {
  test_threaded_population_fitness_computing(1);
}

void test_lsearch_2opt_tour_distance_2_thread() {
  test_threaded_population_fitness_computing(2);
}

void test_lsearch_2opt_tour_distance_3_thread() {
  test_threaded_population_fitness_computing(3);
}

void test_lsearch_2opt_tour_distance_4_thread() {
  test_threaded_population_fitness_computing(4);
}

void test_lsearch_2opt_tour_distance_5_thread() {
  test_threaded_population_fitness_computing(5);
}

void test_lsearch_2opt_tour_distance_6_thread() {
  test_threaded_population_fitness_computing(6);
}

void test_lsearch_2opt_tour_distance_7_thread() {
  test_threaded_population_fitness_computing(7);
}

void test_lsearch_2opt_tour_distance_8_thread() {
  test_threaded_population_fitness_computing(8);
}