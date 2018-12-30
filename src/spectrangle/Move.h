#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <array>

#include "Vec2i.h"

enum class Color : uint8_t {
    NONE = 0,
    RED = 1,
    BLUE = 2,
    GREEN = 4,
    YELOW = 8,
    PURPLE = 16,
    WHITE = 32
} ;

using Rotation = uint8_t;
using Side = uint8_t;

struct Tile {
    Tile() = default;
    Tile(Color side1, Color side2, Color side3);

    bool isSymmetrical() const;

    std::array<Color, 3> sides;
} ;

struct Move {
    Move() = default;
    Move(Vec2i pos, Tile tile, Rotation rotation);

    Color getSide(Side side) const;
    Tile getTile() const;

    Vec2i pos;
    Tile tile;
    Rotation rotation;
} ;

bool operator==(Tile const & a, Tile const & b);
bool operator<(Tile const & a, Tile const & b);
bool operator==(Move const & a, Move const & b);
bool operator<(Move const & a, Move const & b);

#endif // MOVE_H
