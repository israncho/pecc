#pragma once

#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "genetic_algorithm.h"

int full_generational_replacement(ga_execution *exec, ga_workspace *thread_workspace);

int full_gen_replacement_elitism(ga_execution *exec, ga_workspace *thread_workspace);

#endif