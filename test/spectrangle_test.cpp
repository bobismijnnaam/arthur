#include <iostream>
#include <gtest/gtest.h>

#include "Vec2i.h"
#include "Spectrangle.h"

TEST(Spectrangle, moves) {
    {
        Spectrangle game;

        EXPECT_FALSE(game.isBagEmpty());
    }

    {
        Spectrangle game;
        Move move({0, 0}, {Color::RED, Color::RED, Color::RED}, 0);

        EXPECT_TRUE(game.isMovePossible(move));
    }

    {
        Spectrangle game;
        //      R
        //      R
        //  G   GB   B

        Move moveR({0, 0}, {Color::RED, Color::RED, Color::RED}, 0);
        Move moveG({0, 1}, {Color::GREEN, Color::GREEN, Color::GREEN}, 0);
        Move moveB({2, 1}, {Color::BLUE, Color::BLUE, Color::BLUE}, 0);

        EXPECT_TRUE(game.isMovePossible(moveR));
        EXPECT_TRUE(game.isMovePossible(moveG));
        EXPECT_TRUE(game.isMovePossible(moveB));

        game.applyMove(moveR);
        game.applyMove(moveG);
        game.applyMove(moveB);

        EXPECT_TRUE(!game.isMovePossible(moveR));
        EXPECT_TRUE(!game.isMovePossible(moveG));
        EXPECT_TRUE(!game.isMovePossible(moveB));

        Move moveCenter({1, 1}, {Color::BLUE, Color::GREEN, Color::RED}, 0);

        EXPECT_TRUE(game.isMovePossible({{1, 1}, {Color::BLUE, Color::GREEN, Color::RED}, 0}));
        EXPECT_TRUE(game.isMovePossible({{1, 1}, {Color::RED, Color::BLUE, Color::GREEN}, 1}));
        EXPECT_TRUE(game.isMovePossible({{1, 1}, {Color::GREEN, Color::RED, Color::BLUE}, 2}));

        EXPECT_FALSE(game.isMovePossible({{1, 1}, {Color::GREEN, Color::GREEN, Color::GREEN}, 0}));
    }

    {
        Spectrangle game;
        //     
        // 0         _
        // 1       _ _ _
        // 2     _ R _ G _
        // 3   _ _ _ B _ _ _
        // 4

        Move moveR({1, 2}, {Color::RED, Color::RED, Color::RED}, 0);
        Move moveG({3, 2}, {Color::GREEN, Color::GREEN, Color::GREEN}, 0);
        Move moveB({3, 3}, {Color::BLUE, Color::BLUE, Color::BLUE}, 0);

        EXPECT_TRUE(game.isMovePossible(moveR));
        EXPECT_TRUE(game.isMovePossible(moveG));
        EXPECT_TRUE(game.isMovePossible(moveB));

        game.applyMove(moveR);
        game.applyMove(moveG);
        game.applyMove(moveB);

        EXPECT_TRUE(!game.isMovePossible(moveR));
        EXPECT_TRUE(!game.isMovePossible(moveG));
        EXPECT_TRUE(!game.isMovePossible(moveB));

        Move moveCenter({1, 1}, {Color::BLUE, Color::GREEN, Color::RED}, 0);

        EXPECT_TRUE(game.isMovePossible({{2, 2}, {Color::GREEN, Color::BLUE, Color::RED}, 0}));
        EXPECT_TRUE(game.isMovePossible({{2, 2}, {Color::RED, Color::GREEN, Color::BLUE}, 1}));
        EXPECT_TRUE(game.isMovePossible({{2, 2}, {Color::BLUE, Color::RED, Color::GREEN}, 2}));

        EXPECT_FALSE(game.isMovePossible({{2, 2}, {Color::GREEN, Color::GREEN, Color::GREEN}, 0}));
    }
}

