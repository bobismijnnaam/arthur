#include <array>
#include <optional>

#include "TriangleGrid.h"
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

struct Tile {
    Tile() = default;
    Tile(Color side1, Color side2, Color side3);

    std::array<Color, 3> sides;
} ;

Tile::Tile(Color side1, Color side2, Color side3) : sides{{side1, side2, side3}} {}

using Rotation = uint8_t;
using Side = uint8_t;

struct Move {
    Move() = default;
    Move(Vec2i pos, Tile tile, Rotation rotation);

    Color getSide(Side side) const;

    Vec2i pos;
    Tile tile;
    Rotation rotation;
} ;

Move::Move(Vec2i pos, Tile tile, Rotation rotation) : pos{pos}, tile{tile}, rotation{rotation} {}

Color Move::getSide(Side side) const {
    return tile.sides[(rotation + side) % 3];
}

int const SPECTRANGLE_BOARD_SIDE = 11;
int const NUM_MAX_POSSIBLE_MOVES = TriangleGrid<int, SPECTRANGLE_BOARD_SIDE>::NUM_ELEMS * 4 * 3;
int const NUM_TOTAL_TILES = 36;
int const MAX_TILES_PER_PLAYER = 4;
int const MAX_NUM_PLAYERS = 4;

std::array<Tile, NUM_TOTAL_TILES> const tileBagStarterSet = {{
    {Color::RED, Color::BLUE, Color::GREEN},
    {Color::RED, Color::BLUE, Color::GREEN},
    {Color::RED, Color::BLUE, Color::GREEN}
}};

using MoveBuffer = FixVector<Move, NUM_MAX_POSSIBLE_MOVES>;
using PlayerBag = FixVector<Tile, MAX_TILES_PER_PLAYER>;

class Spectrangle {
public:
    Spectrangle();

    bool isMovePossible(Move const & move);
    void applyMove(Move const & move);
    void getMoves(int player, MoveBuffer& moveBuffer);
    Color getNeighbourColorAtSide(Vec2i const pos, Side side);

    int getNumTilesAvailable();
    void removeTileFromBag(Tile const tile);
    Tile takeTileFromBag(int index);

    int getPlayerNumTiles(int player);
    Tile getTileFromPlayer(int index);
    void giveTileToPlayer(Tile const tile);
    Tile takeTileFromPlayer(int index);

    bool isBagEmpty();

private:
    TriangleGrid<std::optional<Tile>, SPECTRANGLE_BOARD_SIDE> grid;
    FixVector<Tile, NUM_TOTAL_TILES> tileBag;
    std::array<PlayerBag, MAX_NUM_PLAYERS> playerBags;

} ;

Spectrangle::Spectrangle() {
    tileBag.data = tileBagStarterSet;
    tileBag.size = NUM_TOTAL_TILES;
}

/**
 * Given a move, checks if the move is possible.
 */
bool Spectrangle::isMovePossible(Move const & move) {

    Color color0 = move.getSide(0);
    Color color1 = move.getSide(1);
    Color color2 = move.getSide(2);

    Color otherColor0 = getNeighbourColorAtSide(move.pos, 0);
    Color otherColor1 = getNeighbourColorAtSide(move.pos, 1);
    Color otherColor2 = getNeighbourColorAtSide(move.pos, 2);

    return
        (otherColor0 == Color::NONE
            || otherColor0 == Color::WHITE
            || color0 == Color::WHITE
            || otherColor0 == color0)
        && (otherColor1 == Color::NONE
            || otherColor1 == Color::WHITE
            || color1 == Color::WHITE
            || otherColor1 == color0)
        && (otherColor2 == Color::NONE 
            || otherColor2 == Color::WHITE
            || color2 == Color::WHITE
            || otherColor2 == color0)
        ;
}

/**
 * Returns the color of the neighbor at the side of pos.
 */
Vec2i neighbourCoordinateAtSide(Vec2i const pos, Side side) {
    bool isUpTriangle = (pos.x % 2) == 0;

    if (isUpTriangle) {
        switch (side) {
            case 0:
                return { pos.x + 1, pos.y };
            case 1:
                return { pos.x + 1, pos.y + 1 };
            case 2:
                return { pos.x - 1, pos.y };
        }
    } else {
        switch (side) {
            case 0:
                return { pos.x + 1, pos.y };
            case 1:
                return { pos.x - 1, pos.y };
            case 2:
                return { pos.x - 1, pos.y - 1 };
        }
    }
}

Color Spectrangle::getNeighbourColorAtSide(Vec2i const pos, Side side) {
    Side neighbourSide = (side + 2) % 3;

    Vec2i neighbour = neighbourCoordinateAtSide(pos, side);

    std::optional<Tile>& tile = grid.get(neighbour);
    if (tile.has_value()) {
        return (*tile).sides[neighbourSide];
    } else {
        return Color::NONE;
    }
}
