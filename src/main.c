#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/evo_comp/crossover.h"
#include "../include/evo_comp/genetic_algorithm.h"
#include "../include/evo_comp/mutation.h"
#include "../include/evo_comp/population.h"
#include "../include/evo_comp/replacement.h"
#include "../include/evo_comp/selection.h"
#include "../include/tsp/euclidean.h"
#include "../include/utils/input_output.h"
#include "../include/utils/mystring.h"
#include "../include/utils/mytime.h"

void get_arguments_for_exec(size_t *generations, size_t *population_size,
                            double *mutation_rate, size_t *n_threads,
                            uint64_t *seed1, uint64_t *seed2,
                            size_t *local_search_iters,
                            char **ptr_to_instance_file_path,
                            char **ptr_to_output_file_path) {
  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  assert(read_file("input.txt", &array_of_lines, &num_of_lines) ==
         FILE_READ_SUCCESS);
  for (size_t i = 0; i < num_of_lines; i += 2) {
    strip_in_place(&array_of_lines[i].content, &array_of_lines[i].length);
    char *label = array_of_lines[i].content;
    strip_in_place(&array_of_lines[i + 1].content,
                   &array_of_lines[i + 1].length);
    char *content = array_of_lines[i + 1].content;
    const size_t content_len = array_of_lines[i + 1].length;

    if (strcmp(label, "INSTANCE_FILE_PATH:") == 0) {
      assert(init_array((void **)ptr_to_instance_file_path, content_len + 1,
                        sizeof(char)) == ARRAY_OK);
      strcpy(*ptr_to_instance_file_path, content);
      printf("INSTANCE_FILE_PATH: %s\n", *ptr_to_instance_file_path);
      continue;
    }

    if (strcmp(label, "OUTPUT_FILE_PATH:") == 0) {
      assert(init_array((void **)ptr_to_output_file_path, content_len + 1,
                        sizeof(char)) == ARRAY_OK);
      strcpy(*ptr_to_output_file_path, content);
      printf("OUTPUT_FILE_PATH: %s\n", *ptr_to_output_file_path);
      continue;
    }

    if (strcmp(label, "SEED1:") == 0) {
      str_to_size_t(content, seed1);
      printf("SEED1: %lu\n", *seed1);
      continue;
    }

    if (strcmp(label, "SEED2:") == 0) {
      str_to_size_t(content, seed2);
      printf("SEED2: %lu\n", *seed2);
      continue;
    }

    if (strcmp(label, "N_GENERATIONS:") == 0) {
      str_to_size_t(content, generations);
      printf("N_GENERATIONS: %zu\n", *generations);
      continue;
    }

    if (strcmp(label, "POPULATION_SIZE:") == 0) {
      str_to_size_t(content, population_size);
      printf("POPULATION_SIZE: %zu\n", *population_size);
      continue;
    }

    if (strcmp(label, "MUTATION_RATE:") == 0) {
      str_to_double(content, mutation_rate);
      printf("MUTATION_RATE: %f\n", *mutation_rate);
      continue;
    }

    if (strcmp(label, "L_SEARCH_ITERATIONS:") == 0) {
      str_to_size_t(content, local_search_iters);
      printf("L_SEARCH_ITERATIONS: %zu\n", *local_search_iters);
      continue;
    }

    if (strcmp(label, "THREADS:") == 0) {
      str_to_size_t(content, n_threads);
      printf("THREADS: %zu\n", *n_threads);
      continue;
    }
  }

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
}

int main() {
  size_t generations;
  size_t population_size;
  double mutation_rate;
  size_t n_threads;
  uint64_t seed1;
  uint64_t seed2;
  size_t local_search_iters;
  char *instance_file_path = NULL;
  char *output_file = NULL;
  get_arguments_for_exec(&generations, &population_size, &mutation_rate,
                         &n_threads, &seed1, &seed2, &local_search_iters,
                         &instance_file_path, &output_file);

  const size_t codification_entry_size = sizeof(size_t);
  const size_t codification_entry_alignment = alignof(size_t);

  file_line *array_of_lines = NULL;
  size_t num_of_lines = 0;
  assert(read_file(instance_file_path, &array_of_lines, &num_of_lines) ==
         FILE_READ_SUCCESS);

  tsp_euc_instance *tsp_instance = NULL;
  assert(init_tsp_euc_instance(array_of_lines, num_of_lines, &tsp_instance) ==
         0);

  ga_execution exec;
  assert(init_ga_execution(
             &exec, n_threads, generations, population_size,
             tsp_instance->number_of_cities - 1, codification_entry_size,
             codification_entry_alignment, mutation_rate, seed1, seed2,
             fill_and_shuffle_population_of_permutations) == 0);

  ga_workspace *workspace_array = NULL;
  assert(init_ga_workspace(&workspace_array, &exec,
                           ox1_workspace_size(exec.codification_size), 0, 0, 0,
                           0, seed1, seed2, local_search_iters) == 0);

  double start = get_wall_time();
  parallel_genetic_algorithm(&exec, workspace_array, n_threads, tsp_instance,
                             random_selection, order_crossover_ox1,
                             swap_mutation_size_t, lsearch_2opt_tour_distance,
                             full_gen_replacement_elitism);
  double elapsed_time = get_wall_time() - start;

  printf("best fitness found: %f\n", exec.current_best->fitness);
  printf("elapsed time: %.6f secs\n", elapsed_time);

  size_t *permutation_of_file_lines = NULL;
  init_and_setup_permutation_for_file_lines(exec.current_best->codification,
                                            tsp_instance,
                                            &permutation_of_file_lines);

  write_to_file_in_specific_order(output_file, array_of_lines, num_of_lines,
                                  permutation_of_file_lines, "w");

  free(permutation_of_file_lines);

  for (size_t i = 0; i < n_threads; i++)
    free(workspace_array[i].mem);
  free(workspace_array);

  free(exec.mem);

  free_tsp_euc_instance_content(tsp_instance);
  free(tsp_instance);

  free_lines_array_content(array_of_lines, num_of_lines);
  free(array_of_lines);
  free(instance_file_path);
  free(output_file);
  return 0;
}