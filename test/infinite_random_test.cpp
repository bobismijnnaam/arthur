#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "Random.h"

namespace testing
{
 namespace internal
 {
  enum GTestColor {
      COLOR_DEFAULT,
      COLOR_RED,
      COLOR_GREEN,
      COLOR_YELLOW
  };

  extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
 }
}
#define PRINTF(...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } while(0)

int const NUMS = 20;
int const NUM_RUNS = 10'000'00;

TEST(RandomGeneration, distribution) {
    std::array<int, NUMS> amountOfTimesPicked {};

    Random rand;

    // std::cout << "Starting...\n";

    for (int i = 0; i < NUM_RUNS; ++i) {
        int pickedIndex = 0;

        for (int candidateIndex = 0; candidateIndex < NUMS; ++candidateIndex) {
            if (rand.range(candidateIndex + 1) == 0) {
                pickedIndex = candidateIndex;
            }
        }

        ++amountOfTimesPicked[pickedIndex];
    }

    // std::cout << "Finished.\n";

    double avg = 0;
    for (auto x : amountOfTimesPicked) {
        // std::cout << x << "\n";
        avg += x / NUMS;
    }

    std::cout << "Avg: " << avg << "\n";

    ASSERT_TRUE(((NUM_RUNS / NUMS) - avg) < 20);
}
