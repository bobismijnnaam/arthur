#include <iostream>
#include <gtest/gtest.h>

#include "Vec2i.h"
#include "TriangleGrid.h"

TEST(TriangleGrid, basicOperations) {
    TriangleGrid<int, 4> triangle;

    std::array<Vec2i, 4> validPositions = {{
        {0, 0},
        {0, 1},
        {6, 3},
        {2, 2}
    }};

    for (auto pos : validPositions) {
        // std::cout << "Position " << pos << " is valid: " << triangle.isPosValid(pos) << "\n";
        EXPECT_TRUE(triangle.isPosValid(pos));
    }

    std::array<Vec2i, 4> invalidPositions = {{
        {-1, 0},
        {0, -1},
        {7, 3},
        {3, 1}
    }};

    for (auto pos : invalidPositions) {
        // std::cout << "Position " << pos << " is invalid: " << triangle.isPosValid(pos) << "\n";
        EXPECT_TRUE(!triangle.isPosValid(pos));
    }

    FixVector<Vec2i, 3> neighbours;

    triangle.getNeighbours({1, 1}, neighbours);
    EXPECT_TRUE(neighbours.getSize() == 3);
    EXPECT_TRUE(neighbours.contains({0,0}));
    EXPECT_TRUE(neighbours.contains({0,1}));
    EXPECT_TRUE(neighbours.contains({2,1}));

    triangle.getNeighbours({0, 0}, neighbours);
    EXPECT_TRUE(neighbours.getSize() == 1);
    EXPECT_TRUE(neighbours.contains({1,1}));

    triangle.getNeighbours({0, 3}, neighbours);
    EXPECT_TRUE(neighbours.getSize() == 1);
    EXPECT_TRUE(neighbours.contains({1,3}));

    triangle.getNeighbours({6, 3}, neighbours);
    EXPECT_TRUE(neighbours.getSize() == 1);
    EXPECT_TRUE(neighbours.contains({5,3}));

    triangle.getNeighbours({1, 2}, neighbours);
    EXPECT_TRUE(neighbours.getSize() == 3);
    EXPECT_TRUE(neighbours.contains({0,2}));
    EXPECT_TRUE(neighbours.contains({2,2}));
    EXPECT_TRUE(neighbours.contains({0,1}));
}

