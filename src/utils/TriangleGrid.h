#ifndef TRIANGLEGRID_H
#define TRIANGLEGRID_H

#include <array>
#include <cassert>

#include "FixVector.h"
#include "Vec2i.h"

template<
    typename T,
    unsigned int SIDE
>
class TriangleGrid {
public:
    // Number of triangles with sides of length SIDE
    static int constexpr NUM_ELEMS = (SIDE + 1) * (SIDE + 1);

    std::array<T, NUM_ELEMS> grid;

    static bool isPosValid(Vec2i const pos) {
        int rowStart = pos.y * pos.y;
        int rowEnd = (pos.y + 1) * (pos.y + 1);

        return pos.x >= 0 && pos.y >= 0
            && (pos.y < SIDE) && (pos.x < (rowEnd - rowStart));
    }

    static int rowLength(int y) {
        return (y + 1) * (y + 1) - y * y;
    }

    T& get(Vec2i const pos) {
        assert(isPosValid(pos));

        int rowStart = pos.y * pos.y;
        return grid[rowStart + pos.x];
    }

    T const & get(Vec2i const pos) const {
        assert(isPosValid(pos));

        int rowStart = pos.y * pos.y;
        return grid[rowStart + pos.x];
    }

    void set(Vec2i const pos, T const & elem) {
        assert(isPosValid(pos));

        int rowStart = pos.y * pos.y;
        grid[rowStart + pos.x] = elem;
    }

    void getNeighbours(Vec2i const pos, FixVector<Vec2i, 3> & vec) {
        // TODO: Move this to a free function or remove it
        vec.clear();

        Vec2i leftNeighbour = pos;
        Vec2i rightNeighbour = pos;
        Vec2i upDownNeighbour = pos;

        leftNeighbour.x -= 1;
        rightNeighbour.x += 1;

        if (pos.x % 2 == 0) {
            // Up triangle
            upDownNeighbour.x += 1;
            upDownNeighbour.y += 1;
        } else {
            // Down triangle
            upDownNeighbour.x -= 1;
            upDownNeighbour.y -= 1;
        }

        if (isPosValid(leftNeighbour)) {
            vec.push(leftNeighbour);
        }
        if (isPosValid(rightNeighbour)) {
            vec.push(rightNeighbour);
        }
        if (isPosValid(upDownNeighbour)) {
            vec.push(upDownNeighbour);
        }
    }
} ;

#endif // TRIANGLEGRID_H
