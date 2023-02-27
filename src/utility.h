#ifndef utility_h
#define utility_h

#include <cmath>

inline int RoundAndClamp(double x) {
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

#endif
