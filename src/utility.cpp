#include "utility.h"

double rand_double(double lower, double upper) {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(generator);
}

int rand_int(int lower, int upper) {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}
