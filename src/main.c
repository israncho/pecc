#include <assert.h>
#include <stdalign.h>
#include <stdio.h>
#include "../include/evo_comp/crossover.h"
#include "../include/evo_comp/genetic_algorithm.h"
#include "../include/evo_comp/mutation.h"
#include "../include/evo_comp/population.h"
#include "../include/evo_comp/replacement.h"
#include "../include/evo_comp/selection.h"
#include "../include/tsp/euclidean.h"
#include "../include/utils/mytime.h"
#include "../include/utils/input_output.h"
#include "../include/utils/myrandom.h"

int main() {
  const size_t generations = 400;
  const size_t population_size = 300;
  const size_t n_threads = 8;
  const char instance_file_path[] = "instances/euc_TSP/saturn.tsp";
  printf(instance_file_path);
  printf("\nthreads: %zu\n", n_threads);
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  assert(read_file(instance_file_path, &array_of_lines, &num_of_lines) ==
         FILE_READ_SUCCESS);

  tsp_euc_instance *tsp_instance = NULL;
  assert(init_tsp_euc_instance(array_of_lines, num_of_lines, &tsp_instance) ==
         0);

  ga_execution exec;
  exec.mem = NULL;
  exec.n_threads = n_threads;
  exec.generations = generations;
  exec.population_size = population_size;
  exec.codification_size = tsp_instance->number_of_cities - 1;
  exec.codification_entry_size = sizeof(size_t);
  exec.codification_entry_alignment = sizeof(size_t);
  exec.mutation_rate = 0.1;

  assert(setup_dynamic_mem_for_ga_execution(&exec) == 0);
  ga_workspace *workspace_array = NULL;
  assert(setup_dynamic_mem_for_ga_workspace(
             &workspace_array, &exec, n_threads,
             ox1_workspace_size(exec.codification_size), 0, 0, 0, 0) == 0);

  for (size_t i = 0; i < n_threads; i++) {
    set_up_seed(&workspace_array[i].state, 0, 0, i);
    workspace_array[i].local_search_iterations = 3;
    workspace_array[i].thread_id = i;
  }

  fill_and_shuffle_population_of_permutations(
      exec.population, exec.population_size, exec.codification_size,
      &workspace_array[0].state);

  
  double start = get_wall_time();
  parallel_genetic_algorithm(&exec, workspace_array, n_threads, tsp_instance,
                             random_selection, order_crossover_ox1,
                             swap_mutation_size_t, lsearch_2opt_tour_distance,
                             full_gen_replacement_elitism);
  double elapsed_time = get_wall_time() - start;
  
  
  printf("best fitness found: %f\n", exec.current_best->fitness);
  printf("elapsed time: %.6f secs\n", elapsed_time);

  size_t *permutation_of_file_lines = NULL;
  init_and_setup_permutation_for_file_lines(exec.current_best->codification, tsp_instance, &permutation_of_file_lines);

  const char output_file[] = "output.tsp";
  write_to_file_in_specific_order(output_file, array_of_lines, num_of_lines, permutation_of_file_lines, "w");

  free(permutation_of_file_lines);

  for (size_t i = 0; i < n_threads; i++)
    free(workspace_array[i].mem);
  free(workspace_array);

  free(exec.mem);

  free_tsp_euc_instance_content(tsp_instance);
  free(tsp_instance);

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  return 0;
}