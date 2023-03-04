#ifndef C3PO_UTILITY_H
#define C3PO_UTILITY_H

namespace c3po {

float clamp(float val, float low, float high);

float rand_float(float lower, float upper);

int rand_int(int lower, int upper);

bool quadratic(float a, float b, float c, float& t0, float& t1);

} // namespace c3po

#endif
