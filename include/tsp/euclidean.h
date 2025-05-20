#pragma once

#ifndef EUCLIDEAN_H
#define EUCLIDEAN_H

#include <stddef.h>
#include "../../include/evo_comp/genetic_algorithm.h"
#include "../../include/utils/input_output.h"

typedef struct {
  double **cities; // array/matrix of cities.
  size_t number_of_cities;
  size_t city_size;
  double **distances; // array/matrix of distances between cities.
  char *name;
  char *type;
  char *comment;
  char *edge_weight_type;
} tsp_euc_instance;

double euclidean_distance(const double *array1, const double *array2,
                          const size_t size);

int fill_distance_matrix(const double *const *cities,
                         const size_t number_of_cities,
                         const size_t city_size,
                         double **distance_matrix);

double tour_distance(void *solution, void *instance_details, ga_workspace *_);

double lsearch_2opt_tour_distance(void *solution, void *instance_details, ga_workspace *thread_workspace);

int init_tsp_euc_instance(const file_line *array_of_lines,
                          const size_t number_of_lines,
                          tsp_euc_instance **ptr_to_tsp_instance);

void free_tsp_euc_instance_content(tsp_euc_instance *tsp_euc_instance_to_free);

#endif