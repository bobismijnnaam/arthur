#ifndef SPECTRANGLE_H
#define SPECTRANGLE_H

#include <optional>

#include "TriangleGrid.h"
#include "Move.h"

int const SPECTRANGLE_BOARD_SIDE = 11;
int const NUM_MAX_POSSIBLE_MOVES = TriangleGrid<int, SPECTRANGLE_BOARD_SIDE>::NUM_ELEMS * 4 * 3;
int const NUM_TOTAL_TILES = 36;
int const MAX_TILES_PER_PLAYER = 4;
int const MAX_NUM_PLAYERS = 4;

using MoveBuffer = FixVector<Move, NUM_MAX_POSSIBLE_MOVES>;
using PlayerBag = FixVector<Tile, MAX_TILES_PER_PLAYER>;
using PlayersState = FixVector<PlayerBag, MAX_NUM_PLAYERS>;
using TileBoard = TriangleGrid<std::optional<Tile>, SPECTRANGLE_BOARD_SIDE>;

class Spectrangle {
public:
    Spectrangle(int numPlayers);

    void setNumPlayers(int numPlayers);

    bool isMovePossible(Move const & move) const;
    void applyMove(Move const & move);
    Color getNeighbourColorAtSide(Vec2i const pos, Side side) const;

    int getNumTilesAvailable() const;
    void removeTileFromBag(Tile const tile);
    Tile takeTileFromBag(int i);

    int getPlayerNumTiles(int player) const;
    Tile getTileFromPlayer(int player, int i) const;
    void giveTileToPlayer(int player, Tile const tile);
    Tile takeTileFromPlayer(int player, int i);

    bool isBagEmpty();

private:
    TileBoard grid;
    FixVector<Tile, NUM_TOTAL_TILES> tileBag;
    PlayersState playerBags;

} ;

#include "Random.h"

std::optional<Move> pickRandomMove(Spectrangle const & game, int player, Random & random);

#endif // SPECTRANGLE_H
