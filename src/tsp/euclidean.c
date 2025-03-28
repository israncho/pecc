#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "../../include/tsp/euclidean.h"

double euclidean_distance(const double *array1, const double *array2,
                          const size_t size) {
  double sum = 0.0;
  for (size_t i = 0; i < size; i++)
    sum += pow(array1[i] - array2[i], 2);
  return sqrt(sum);
}

int fill_distance_matrix(const double **cities, const size_t number_of_cities,
                         const size_t city_size, double **distance_matrix) {
  if (cities == NULL)
    return 1;
  if (distance_matrix == NULL)
    return 1;
  for (size_t i = 0; i < number_of_cities; i++) {
    distance_matrix[i][i] = 0.0;
    for (size_t j = i + 1; j < number_of_cities; j++) {
      double euc_distance = euclidean_distance(cities[i], cities[j], city_size);
      distance_matrix[i][j] = euc_distance;
      distance_matrix[j][i] = euc_distance;
    }
  }
  return 0;
}

double tour_distance(const void *solution, const void *instance_details) {
  const tsp_euc_instance *instance = (tsp_euc_instance *)instance_details;
  const size_t *permutation = (size_t *)solution;
  const size_t n = instance->number_of_cities;
  const double *const *distances = (const double *const *)instance->distances;

  double total_distance = 0.0;

  // last city never included in the permutation
  // so it's index is n - 1
  const size_t last_city_i = n - 1;
  const size_t permutation_size = last_city_i;
  size_t prev_city_i = permutation[0];

  // add distance from last to fst city
  total_distance += distances[last_city_i][prev_city_i];

  for (size_t i = 1; i < permutation_size; i++) {
    size_t curr_city_i = permutation[i];
    total_distance += distances[prev_city_i][curr_city_i];
    prev_city_i = curr_city_i;
  }

  // at the end prev_city_i = permutation[n - 2]
  // distance from snd to last city to last city
  total_distance += distances[prev_city_i][last_city_i];

  return total_distance;
}

int init_tsp_euc_instance(const file_line *array_of_lines,
                          const size_t number_of_lines,
                          tsp_euc_instance **ptr_to_tsp_instance) {
  if (array_of_lines == NULL)
    return 1;
  *ptr_to_tsp_instance = malloc(sizeof(tsp_euc_instance));

  for (size_t i = 0; i < number_of_lines; i++) {
    // to do
  }
  return 0;
}