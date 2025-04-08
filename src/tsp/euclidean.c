#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
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
                         const size_t number_of_cities, const size_t city_size,
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

static int set_tsp_euc_members(tsp_euc_instance *instance, char *member,
                               char *value, size_t value_size) {
  if (strip_in_place(&value, &value_size) != 0)
    return 1;

  if (strcmp("DIMENSION", member) == 0) {
    if (str_to_size_t(value, &(instance->number_of_cities)) != 0)
      return 2;
    return 0;
  }

  char *str = NULL;
  if (init_array((void **)&str, value_size + 1, sizeof(char)) != 0)
    return 3;
  strcpy(str, value);

  if (strcmp("NAME", member) == 0) {
    instance->name = str;
    return 0;
  }
  if (strcmp("TYPE", member) == 0) {
    instance->type = str;
    return 0;
  }
  if (strcmp("COMMENT", member) == 0) {
    instance->comment = str;
    return 0;
  }
  if (strcmp("EDGE_WEIGHT_TYPE", member) == 0) {
    instance->edge_weight_type = str;
    return 0;
  }
  free(str);
  return 4;
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

  // array of cities, encoded as a bidimensional matrix
  double **cities = NULL;
  // matrix of distances between cities
  double **distances = NULL;

  tsp_euc_instance *new_instance = malloc(sizeof(tsp_euc_instance));

  size_t buffer_capacity = 50;
  size_t buffer_usage = 0;
  char *buffer = NULL;
  if (init_array((void **)&buffer, buffer_capacity, char_size) != ARRAY_OK)
    return 5;
  size_t tokens_array_capacity = 5;
  size_t number_of_tokens = 0;
  char **tokens_array = NULL;
  if (init_array((void **)&tokens_array, tokens_array_capacity,
                 sizeof(char *)) != ARRAY_OK)
    return 5;

  for (size_t i = 0; i < number_of_lines; i++) {
    if (strip_to_buffer(array_of_lines[i].content, array_of_lines[i].length,
                        &buffer, &buffer_usage, &buffer_capacity) != 0)
      return 6;

    if (!reading_cities) {
      if (split_in_place(&buffer, buffer_usage, ':', &tokens_array,
                         &tokens_array_capacity, &number_of_tokens) != 0)
        return 7;
    } else {
      if (split_in_place(&buffer, buffer_usage, ' ', &tokens_array,
                         &tokens_array_capacity, &number_of_tokens) != 0)
        return 7;
    }

    if (!reading_cities) {
      if (strcmp(tokens_array[0], "NODE_COORD_SECTION") == 0) {
        number_of_cities = new_instance->number_of_cities;
        reading_cities = true;
        setup_cities = true;
        continue;
      }
      if (set_tsp_euc_members(new_instance, tokens_array[0], tokens_array[1],
                              strlen(tokens_array[1])) != 0)
        return 8;
      continue;
    }

    if (strcmp(tokens_array[0], "EOF") == 0)
      break;

    if (setup_cities) {
      setup_cities = false;
      city_size = number_of_tokens - 1;
      if (init_matrix((void ***)&cities, number_of_cities, city_size,
                      double_size) != 0)
        return 5;
    }

    for (size_t j = 0; j < city_size; j++)
      if (str_to_double(tokens_array[j + 1], &(cities[city_i][j])) != 0)
        return 9;
    city_i++;
  }

  if (init_matrix((void ***)&distances, number_of_cities, number_of_cities,
                  double_size))
    return 5;

  if (fill_distance_matrix((const double *const *)cities, number_of_cities,
                           city_size, distances) != 0)
    return 10;

  new_instance->cities = cities;
  new_instance->distances = distances;
  *ptr_ptr_to_tsp_instance = new_instance;
  free(buffer);
  free(tokens_array);
  return 0;
}

void free_tsp_euc_instance_content(tsp_euc_instance *tsp_euc_instance_to_free) {
  free_matrix((void ***)&tsp_euc_instance_to_free->cities);
  free_matrix((void ***)&tsp_euc_instance_to_free->distances);
  free(tsp_euc_instance_to_free->name);
  free(tsp_euc_instance_to_free->type);
  free(tsp_euc_instance_to_free->comment);
  free(tsp_euc_instance_to_free->edge_weight_type);
  tsp_euc_instance_to_free->city_size = 0;
  tsp_euc_instance_to_free->number_of_cities = 0;
}