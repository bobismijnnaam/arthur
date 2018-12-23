#include <array>
#include <iostream>
#include <gtest/gtest.h>

#include "Spectrangle.h"

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

TEST(Spectrangle, randomMove) {
    for (int i = 0; i < NUM_RUNS; i++) {
        // TODO: Hier verder
        adiwajdoiw  aojdow
    }
}

