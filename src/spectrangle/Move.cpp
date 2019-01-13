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

Move::Move(Vec2i posArg, int tileIndexArg, Rotation rotationArg) : pos{posArg}, tileIndex{tileIndexArg}, rotation{rotationArg} {}

Color Move::getSide(Tile const & tile, Side side) const {
    return tile.sides[(rotation + side) % 3];
}

Tile Move::getTile(Tile const & originalTile) const {
    return {
        originalTile.sides[(0 + rotation) % 3],
        originalTile.sides[(1 + rotation) % 3],
        originalTile.sides[(2 + rotation) % 3],
        originalTile.score
    };
}

GameMove GameMove::Skip(int player) {
    GameMove gm;
    gm.player = player;
    gm.moveType = GameMoveType::SKIP;
    return gm;
}

GameMove GameMove::Exchange(int player) {
    GameMove gm;
    gm.player = player;
    gm.moveType = GameMoveType::EXCHANGE;
    return gm;
}

GameMove GameMove::TileMove(int player, Move const & move) {
    GameMove gm;
    gm.player = player;
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
    return a.pos == b.pos && a.tileIndex == b.tileIndex && a.rotation == b.rotation;
}

bool operator<(Move const & a, Move const & b) {
    return std::tie(a.pos, a.tileIndex, a.rotation) < std::tie(b.pos, b.tileIndex, b.rotation);
} 
