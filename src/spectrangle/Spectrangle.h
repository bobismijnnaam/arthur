#ifndef SPECTRANGLE_H
#define SPECTRANGLE_H

#include "TriangleGrid.h"
#include "Move.h"

int const SPECTRANGLE_BOARD_SIDE = 11;
int const NUM_MAX_POSSIBLE_MOVES = TriangleGrid<int, SPECTRANGLE_BOARD_SIDE>::NUM_ELEMS * 4 * 3;
int const NUM_TOTAL_TILES = 36;
int const MAX_TILES_PER_PLAYER = 4;
int const MAX_NUM_PLAYERS = 4;

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

#endif // SPECTRANGLE_H
