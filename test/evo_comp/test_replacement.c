#include <stdio.h>
#include "../../include/test/evo_comp/test_replacement.h"
#include "../../include/evo_comp/replacement.h"
#include "../../include/utils/mytime.h"

void test_replacement() {
  printf("Testing: replacement\n");

  double start = get_wall_time();
  test_full_generational_replacement_1_thread();
  double elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_1_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_2_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_2_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_3_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_3_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_4_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_4_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_5_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_5_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_6_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_6_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_7_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_7_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_generational_replacement_8_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_generational_replacement_8_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_1_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_1_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_2_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_2_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_3_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_3_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_4_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_4_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_5_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_5_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_6_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_6_thread: PASSED [%.6f secs]\n",
         elapsed_time);


  start = get_wall_time();
  test_full_gen_replacement_elitism_7_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_7_thread: PASSED [%.6f secs]\n",
         elapsed_time);

  start = get_wall_time();
  test_full_gen_replacement_elitism_8_thread();
  elapsed_time = get_wall_time() - start;
  printf("\t- full_gen_replacement_elitism_8_thread: PASSED [%.6f secs]\n",
         elapsed_time);
}

void test_full_generational_replacement_1_thread() {

}

void test_full_generational_replacement_2_thread() {

}

void test_full_generational_replacement_3_thread() {

}

void test_full_generational_replacement_4_thread() {

}

void test_full_generational_replacement_5_thread() {

}

void test_full_generational_replacement_6_thread() {

}

void test_full_generational_replacement_7_thread() {

}

void test_full_generational_replacement_8_thread() {

}

void test_full_gen_replacement_elitism_1_thread() {

}

void test_full_gen_replacement_elitism_2_thread() {

}

void test_full_gen_replacement_elitism_3_thread() {

}

void test_full_gen_replacement_elitism_4_thread() {

}

void test_full_gen_replacement_elitism_5_thread() {

}

void test_full_gen_replacement_elitism_6_thread() {

}

void test_full_gen_replacement_elitism_7_thread() {

}

void test_full_gen_replacement_elitism_8_thread() {

}