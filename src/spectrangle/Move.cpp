#include "Move.h"

Tile::Tile(Color side1, Color side2, Color side3) : sides{{side1, side2, side3}} {}

bool Tile::isSymmetrical() const {
    return sides[0] == sides[1] && sides[1] == sides[2];
}

bool operator==(Tile const & a, Tile const & b) {
    return a.sides == b.sides;
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
    };
}
