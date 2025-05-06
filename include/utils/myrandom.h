#pragma once

#ifndef MYRANDOM_H
#define MYRANDOM_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint64_t s[2];
} xorshiftr128plus_state;

void set_up_seed(xorshiftr128plus_state *state, const uint64_t seed1,
                const uint64_t seed2, const uint64_t thread_id);

static inline uint64_t xorshiftr128plus(xorshiftr128plus_state *state) {
  uint64_t x = state->s[0];
  uint64_t const y = state->s[1];
  state->s[0] = y;
  x ^= x << 23; // shift & xor
  x ^= x >> 17; // shift & xor
  x ^= y;       // xor
  state->s[1] = x + y;
  return x;
}

static inline size_t randsize_t(size_t max, xorshiftr128plus_state *state) {
  uint64_t x = xorshiftr128plus(state);
  return ((__uint128_t)x * (__uint128_t)max) >> 64;
}

static inline size_t randsize_t_i(size_t min, size_t max, xorshiftr128plus_state *state) {
  size_t interval_size = max - min + 1;
  uint64_t x = xorshiftr128plus(state);
  return (((__uint128_t)x * (__uint128_t)interval_size) >> 64) + min;
}

int shuffle_array_of_size_t(size_t *array, const size_t array_size, xorshiftr128plus_state *state);

#endif