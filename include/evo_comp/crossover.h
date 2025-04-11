#pragma once

#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "../../include/utils/myrandom.h"
#include "genetic_algorithm.h"

int order_crossover_ox1(const individual *ptr_parent1,
                        const individual *ptr_parent2, individual *ptr_child1,
                        individual *ptr_child2, const size_t chromosome_size,
                        ga_workspace *ptr_workspace,
                        xorshiftr128plus_state *ptr_state);

#endif