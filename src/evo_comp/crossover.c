#include "../../include/evo_comp/crossover.h"
#include "../../include/evo_comp/genetic_algorithm.h"
#include <stdbool.h>
#include <stddef.h>

int order_crossover_ox1(const individual *ptr_parent1,
                        const individual *ptr_parent2, individual *ptr_child1,
                        individual *ptr_child2, const size_t chromosome_size,
                        ga_workspace *ptr_workspace,
                        xorshiftr128plus_state *ptr_state) {
  if (ptr_parent1 == NULL || ptr_parent2 == NULL)
    return 1;
  if (ptr_parent1->codification == NULL || ptr_parent2->codification == NULL)
    return 2;
  size_t *parent1 = ptr_parent1->codification;
  size_t *parent2 = ptr_parent2->codification;

  if (ptr_child1 == NULL)
    return 3;
  if (ptr_child1->codification == NULL)
    return 4;

  size_t *child1 = ptr_child1->codification, *child2 = NULL;
  bool generate_two_childs = false;

  if (ptr_child2 != NULL && ptr_child2->codification != NULL) {
    child2 = ptr_child2->codification;
    generate_two_childs = true;
  }

  if (ptr_workspace == NULL || ptr_workspace->crossover_workspace == NULL)
    return 5;

  void *workspace = ptr_workspace->crossover_workspace; 
  size_t workspace_capacity = ptr_workspace->crossover_workspace_capacity;

  if (ptr_state == NULL)
    return 6;

  return 0;
}