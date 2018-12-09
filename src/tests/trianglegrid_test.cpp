#include <iostream>
#include <cassert>

#include "Vec2i.h"
#include "TriangleGrid.h"

int main() {
    TriangleGrid<int, 4> triangle;

    std::array<Vec2i, 4> validPositions = {{
        {0, 0},
        {0, 1},
        {6, 3},
        {2, 2}
    }};

    for (auto pos : validPositions) {
        std::cout << "Position " << pos << " is valid: " << triangle.isPosValid(pos) << "\n";
        assert(triangle.isPosValid(pos));
    }

    std::array<Vec2i, 4> invalidPositions = {{
        {-1, 0},
        {0, -1},
        {7, 3},
        {3, 1}
    }};

    for (auto pos : invalidPositions) {
        std::cout << "Position " << pos << " is invalid: " << triangle.isPosValid(pos) << "\n";
        assert(!triangle.isPosValid(pos));
    }

    FixVector<Vec2i, 3> neighbours;

    triangle.getNeighbours({1, 1}, neighbours);
    assert(neighbours.getSize() == 3);
    assert(neighbours.contains({0,0}));
    assert(neighbours.contains({0,1}));
    assert(neighbours.contains({2,1}));

    triangle.getNeighbours({0, 0}, neighbours);
    assert(neighbours.getSize() == 1);
    assert(neighbours.contains({1,1}));

    triangle.getNeighbours({0, 3}, neighbours);
    assert(neighbours.getSize() == 1);
    assert(neighbours.contains({1,3}));

    triangle.getNeighbours({6, 3}, neighbours);
    assert(neighbours.getSize() == 1);
    assert(neighbours.contains({5,3}));

    triangle.getNeighbours({1, 2}, neighbours);
    assert(neighbours.getSize() == 3);
    assert(neighbours.contains({0,2}));
    assert(neighbours.contains({2,2}));
    assert(neighbours.contains({0,1}));
}

