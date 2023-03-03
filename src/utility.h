#ifndef c3po_utility_h
#define c3po_utility_h

#include <cmath>
#include <random>

float clamp(float val, float low, float high);

float rand_float(float lower, float upper);

int rand_int(int lower, int upper);

bool quadratic(float a, float b, float c, float& t0, float& t1);

#endif
