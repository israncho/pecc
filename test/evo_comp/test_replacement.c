#include <stdio.h>
#include "../../include/test/evo_comp/test_replacement.h"
#include "../../include/evo_comp/replacement.h"
#include "../../include/utils/mytime.h"

void test_replacement() {
  printf("Testing: replacement\n");

  double start = get_wall_time();
  test_full_generational_replacement();
  double elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism();
  elapsed_time = get_wall_time() - start;
  printf(
      "\t- full_gen_replacement_elitism: PASSED [%.6f secs]\n",
      elapsed_time);
}

void test_full_generational_replacement() {

}

void test_full_gen_replacement_elitism() {

}