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

class Random {
    Random(uint64_t x, uint64_t y, uint64_t z) : x{x}, y{y}, z{z} {}

    uint64_t next();
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

// https://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator
