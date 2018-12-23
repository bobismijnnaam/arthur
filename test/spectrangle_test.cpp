#include <iostream>
#include <gtest/gtest.h>

#include "Vec2i.h"
#include "Spectrangle.h"

TEST(Spectrangle, moves) {
    Spectrangle game;

    EXPECT_FALSE(game.isBagEmpty());

    {
        Move move({0, 0}, {Color::RED, Color::RED, Color::RED}, 0);

        EXPECT_TRUE(game.isMovePossible(move));

        game.applyMove(move);

        // Introspect game state
    }
}

