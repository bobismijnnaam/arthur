#include <tuple>

#include "Move.h"

Tile::Tile(Color side1, Color side2, Color side3, Score scoreArg) : sides{{side1, side2, side3}}, score{scoreArg} {}

bool Tile::isSymmetrical() const {
    return sides[0] == sides[1] && sides[1] == sides[2];
}

Move::Move(Vec2i pos, Tile tile, Rotation rotation) : pos{pos}, tile{tile}, rotation{rotation} {}

Color Move::getSide(Side side) const {
    return tile.sides[(rotation + side) % 3];
}

Tile Move::getTile() const {
    return {
        tile.sides[(0 + rotation) % 3],
        tile.sides[(1 + rotation) % 3],
        tile.sides[(2 + rotation) % 3],
        tile.score
    };
}

bool operator==(Tile const & a, Tile const & b) {
    return a.sides == b.sides;
}

bool operator<(Tile const & a, Tile const & b) {
    return std::tie(a.sides[0], a.sides[1], a.sides[2]) < std::tie(b.sides[0], b.sides[1], b.sides[2]);
}

bool operator==(Move const & a, Move const & b) {
    return a.pos == b.pos && a.tile == b.tile && a.rotation == b.rotation;
}

bool operator<(Move const & a, Move const & b) {
    return std::tie(a.pos, a.tile, a.rotation) < std::tie(b.pos, b.tile, b.rotation);
} 
