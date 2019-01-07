#include <tuple>

#include "Move.h"

std::ostream& operator<<(std::ostream& os, Color const clr) {
    switch (clr) {
        case Color::NONE:
        case Color::RED: os << "RED"; break;
        case Color::BLUE: os << "BLUE"; break;
        case Color::GREEN: os << "GREEN"; break;
        case Color::YELLOW: os << "YELLOW"; break;
        case Color::PURPLE: os << "PURPLE"; break;
        case Color::WHITE: os << "WHITE"; break;
        default: os << "UNDEFINED"; break;
    }
    return os;
}

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

GameMove GameMove::Skip() {
    GameMove gm;
    gm.moveType = GameMoveType::SKIP;
    return gm;
}

GameMove GameMove::Exchange() {
    GameMove gm;
    gm.moveType = GameMoveType::EXCHANGE;
    return gm;
}

GameMove GameMove::TileMove(Move const & move) {
    GameMove gm;
    gm.moveType = GameMoveType::MOVE;
    gm.move = move;
    return gm;
}

bool operator==(Tile const & a, Tile const & b) {
    return a.sides == b.sides;
}

bool operator<(Tile const & a, Tile const & b) {
    return std::tie(a.sides[0], a.sides[1], a.sides[2]) < std::tie(b.sides[0], b.sides[1], b.sides[2]);
}

std::ostream& operator<<(std::ostream& os, Tile const & tile) {
    os << "{" << tile.sides[0] << ", " << tile.sides[1] << ", " << tile.sides[2] << ", " << (int) tile.score << "}";
    return os;
}

bool operator==(Move const & a, Move const & b) {
    return a.pos == b.pos && a.tile == b.tile && a.rotation == b.rotation;
}

bool operator<(Move const & a, Move const & b) {
    return std::tie(a.pos, a.tile, a.rotation) < std::tie(b.pos, b.tile, b.rotation);
} 
