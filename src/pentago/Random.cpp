#include <cstdint>

/* Original code from: https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c */
/* static unsigned long x=123456789, y=362436069, z=521288629;
 * 
 * unsigned long xorshf96(void) {          //period 2^96-1
 * unsigned long t;
 *     x ^= x << 16;
 *     x ^= x >> 5;
 *     x ^= x << 1;
 * 
 *    t = x;
 *    x = y;
 *    y = z;
 *    z = t ^ x ^ y;
 * 
 *   return z;
 * }
 */

#include <random>
#include <cstdint>

Random::Random(uint64_t x, uint64_t y, uint64_t z) : x{x}, y{y}, z{z} {}

Random::Random() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint64_t> dist();

    x = dist(mt);
    y = dist(mt);
    z = dist(mt);
}

uint64_t Random::next() {
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    uint64_t t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

uint64_t Random::range(uint64_t max) {
    uint64_t x;
    do {
        x = next();
    } while (x >= (std::numeric_limits<uint64_t>::max() - std::numeric_limits<uint64_t>::max() % max));

    return x % max;
}

int64_t Random::range(int64_t min, int64_t max) {
    return min + range(max - min);
}

// https://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator
