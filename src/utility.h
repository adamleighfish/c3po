#ifndef c3po_utility_h
#define c3po_utility_h

#include <cmath>
#include <random>

inline int RoundAndClamp(float x) {
    int min = 0;
    int max = 255;
    int raw = int(std::floor(255 * x + 0.5));

    if (raw < min) {
        return min;
    } else if (raw > max) {
        return max;
    }
    return raw;
}

float rand_float(float lower, float upper);
int rand_int(int lower, int upper);

#endif
