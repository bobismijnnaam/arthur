#ifndef SPECTRANGLE_H
#define SPECTRANGLE_H

#include <optional>

#include "TriangleGrid.h"
#include "Move.h"

int const SPECTRANGLE_BOARD_SIDE = 6;
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
    Spectrangle(int numPlayersArg);

    void setNumPlayers(int numPlayers);

    bool isMovePossible(Move const & move) const;
    void applyMove(int player, Move const & move);
    Color getNeighbourColorAtSide(Vec2i const pos, Side side) const;

    int getNumTilesAvailable() const;
    void removeTileFromBag(Tile const tile);
    Tile takeTileFromBag(int i);
    void putTileInBag(Tile const & tile);

    int getPlayerNumTiles(int player) const;
    Tile getTileFromPlayer(int player, int i) const;
    void giveTileToPlayer(int player, Tile const tile);
    Tile takeTileFromPlayer(int player, int i);

    bool isInitialMoveDone();
    bool isBagEmpty();
    int getNumPlayers();

    PlayersState const & getPlayersState();
    TileBoard const & getBoard();

    int getScore(int player);
    std::optional<int> getWinner();

private:
    TileBoard grid;
    FixVector<Tile, NUM_TOTAL_TILES> tileBag;
    int numPlayers;
    bool isInInitialState;
    PlayersState playerBags;
    std::array<int, MAX_NUM_PLAYERS> scores;
} ;

#include "Random.h"

void getAllTileMoves(Spectrangle const & game, int player, MoveBuffer & buffer);
std::optional<Move> pickRandomTileMove(Spectrangle const & game, int player, Random & random);
int getMultiplier(Vec2i pos);

#endif // SPECTRANGLE_H
