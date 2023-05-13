#include "random_gen.h"

#include <random>

double get_random_number() {
    static std::mt19937 rnd(std::random_device{}());
    static std::uniform_real_distribution dist;
    return dist(rnd);
}
