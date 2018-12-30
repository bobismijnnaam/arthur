#include <array>
#include <iostream>
#include <gtest/gtest.h>
#include <set>

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
int const NUM_RUNS = 10'000;
// int const NUM_RUNS = 5;

TEST(Spectrangle, randomMove) {
    Spectrangle game;
    Random random;
    game.giveTileToPlayer(0, {Color::RED, Color::GREEN, Color::BLUE});

    int maxY = SPECTRANGLE_BOARD_SIDE - 1;
    int maxX = TriangleGrid<int, SPECTRANGLE_BOARD_SIDE>::rowLength(maxY) - 1;

    game.applyMove({{0, 0}, {Color::RED, Color::RED, Color::RED}, 0});
    game.applyMove({{0, maxY}, {Color::GREEN, Color::GREEN, Color::GREEN}, 0});
    game.applyMove({{maxX, maxY}, {Color::BLUE, Color::BLUE, Color::BLUE}, 0});

    std::set<Move> generatedMoves;

    for (int i = 0; i < NUM_RUNS; i++) {
        std::optional<Move> move = pickRandomMove(game, 0, random);
        EXPECT_TRUE(move.has_value());
        if (move.has_value()) {
            generatedMoves.insert(*move);
        }
    }

    EXPECT_EQ(generatedMoves.size(), 3);
}

