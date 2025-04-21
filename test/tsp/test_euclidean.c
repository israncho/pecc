#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../include/utils/mytime.h"
#include "../../include/test/tsp/test_euclidean.h"
#include "../../include/tsp/euclidean.h"
#include "../../include/utils/input_output.h"

void test_euclidean() {
  printf("Testing: tsp_euclidean\n");

  clock_t start = clock();
  test_init_tsp_euc_instance();
  printf("\t- init_tsp_euc_instance: PASSED [%.4f secs]\n", MEASURE_TIME(start));

  start = clock();
  test_euclidean_distance();
  printf("\t- euclidean_distance: PASSED [%.4f secs]\n", MEASURE_TIME(start));
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