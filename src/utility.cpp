#include "utility.h"

float clamp(float val, float low, float high) {
    if (val < low) {
        return low;
    } else if (val > high) {
        return high;
    }
    return val;
}

float rand_float(float lower, float upper) {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(lower, upper);
    return distribution(generator);
}

int rand_int(int lower, int upper) {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}

bool quadratic(float a, float b, float c, float& t0, float& t1) {
    double const discim = double(b) * double(b) - 4 * double(a) * double(c);
    if (discim < 0.) {
        return false;
    }
    double const root_discim = std::sqrt(discim);

    double q;
    if (b < 0) {
        q = -.5 * (double(b) - root_discim);
    } else {
        q = -.5 * (double(b) + root_discim);
    }
   
    t0 = q / a;
    t1 = c / q;

    if (t0 > t1) {
        std::swap(t0, t1);
    }

    return true;
}
