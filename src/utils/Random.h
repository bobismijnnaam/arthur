#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

class Random {
public:
    uint64_t x, y, z;

    Random(uint64_t x, uint64_t y, uint64_t z);
    Random();

    uint64_t next();
    uint64_t range(uint64_t max);
    int64_t range(int64_t min, int64_t max);
} ;

#endif // RANDOM_H
