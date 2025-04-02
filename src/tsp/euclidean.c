#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/tsp/euclidean.h"
#include "../../include/utils/array.h"
#include "../../include/utils/input_output.h"
#include "../../include/utils/matrix.h"
#include "../../include/utils/mystring.h"

double euclidean_distance(const double *array1, const double *array2,
                          const size_t size) {
  double sum = 0.0;
  for (size_t i = 0; i < size; i++)
    sum += pow(array1[i] - array2[i], 2);
  return sqrt(sum);
}

int fill_distance_matrix(const double *const *cities,
                         const size_t number_of_cities,
                         const size_t city_size,
                         double **distance_matrix) {
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

static int set_members(tsp_euc_instance *instance, const char *member,
                       const char *value, const size_t value_size) {
  int status_code = 0;
  if (strcmp("DIMENSION:", member) == 0) {
    status_code = str_to_size_t(value, &(instance->number_of_cities));
    if (status_code != 0)
      return 1;
    return 0;
  }

  char *str = NULL;
  status_code = init_array((void **)&str, value_size + 1, sizeof(char));
  if (status_code != 0)
    return 2;
  strcpy(str, value);

  if (strcmp("NAME:", member) == 0) {
    instance->name = str;
    return 0;
  }
  if (strcmp("TYPE:", member) == 0) {
    instance->type = str;
    return 0;
  }
  if (strcmp("COMMENT:", member) == 0) {
    instance->comment = str;
    return 0;
  }
  if (strcmp("EDGE_WEIGHT_TYPE:", member) == 0) {
    instance->edge_weight_type = str;
    return 0;
  }
  free(str);
  return 3;
}

int init_tsp_euc_instance(const file_line *array_of_lines,
                          const size_t number_of_lines,
                          tsp_euc_instance **ptr_ptr_to_tsp_instance) {
  if (array_of_lines == NULL)
    return 1;
  if (number_of_lines <= 7)
    return 2;
  if (ptr_ptr_to_tsp_instance == NULL)
    return 3;
  if (*ptr_ptr_to_tsp_instance != NULL)
    return 4;

  bool reading_cities = false;
  bool setup_cities = false;

  size_t number_of_cities = 0;
  size_t city_size = 0;
  size_t city_i = 0;
  size_t char_size = sizeof(char);
  size_t double_size = sizeof(double);

  double **cities = NULL;

  tsp_euc_instance *new_instance = malloc(sizeof(tsp_euc_instance));

  for (size_t i = 0; i < number_of_lines; i++) {
    char *str = NULL;
    size_t str_size = array_of_lines[i].length;
    if (!init_array((void **)&str, str_size + 1, char_size))
      return 2;
    strcpy(str, array_of_lines[i].content);

    char **array_of_splited_str = NULL;
    size_t array_of_splited_size = 0;
    if (!strip(&str, &str_size))
      return 2;

    if (!split(str, str_size, ' ', &array_of_splited_str,
               &array_of_splited_size))
      return 3;

    // to do

    free(str);
    free_matrix((void ***)&array_of_splited_str);
  }

  if (init_matrix((void ***)&(new_instance->distances), number_of_cities,
                  number_of_cities, double_size))
    return 5;

  fill_distance_matrix((const double *const *)new_instance->cities,
                       number_of_cities, city_size, new_instance->distances);

  *ptr_ptr_to_tsp_instance = new_instance;
  return 0;
}