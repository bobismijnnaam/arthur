#ifndef SPECTRANGLE_H
#define SPECTRANGLE_H

#include <optional>

#include "TriangleGrid.h"
#include "Move.h"
#include "Random.h"

int const SPECTRANGLE_BOARD_SIDE = 6;
int const NUM_MAX_POSSIBLE_MOVES = TriangleGrid<int, SPECTRANGLE_BOARD_SIDE>::NUM_ELEMS * 4 * 3;
int const NUM_MAX_POSSIBLE_GAMEMOVES = NUM_MAX_POSSIBLE_MOVES + 2;
int const NUM_TOTAL_TILES = 36;
int const MAX_TILES_PER_PLAYER = 4;
int const MAX_NUM_PLAYERS = 4;

using GameMoveBuffer = FixVector<GameMove, NUM_MAX_POSSIBLE_GAMEMOVES>;
using MoveBuffer = FixVector<Move, NUM_MAX_POSSIBLE_MOVES>;
using PlayerBag = FixVector<Tile, MAX_TILES_PER_PLAYER>;
using PlayersState = FixVector<PlayerBag, MAX_NUM_PLAYERS>;
using TileBoard = TriangleGrid<std::optional<Tile>, SPECTRANGLE_BOARD_SIDE>;

class Spectrangle {
public:
    Spectrangle(int numPlayersArg);

    void setNumPlayers(int numPlayers);
    void initializePlayerBags(Random & random);

    bool isMovePossible(int player, Move const & move) const;
    bool isMovePossible(Tile const & tile, Move const & move) const;
    void applyMove(int player, Move const & move, Random & random);
    int getNumNeighbours(Vec2i pos) const;
    Color getNeighbourColorAtSide(Vec2i const pos, Side side) const;

    int getNumTilesAvailable() const;
    void removeTileFromBag(Tile const tile);
    Tile takeTileFromBag(int i);
    Tile getTileFromBag(int i) const;
    void putTileInBag(Tile const & tile);

    int getPlayerNumTiles(int player) const;
    Tile getTileFromPlayer(int player, int i) const;
    void giveTileToPlayer(int player, Tile const tile);
    Tile takeTileFromPlayer(int player, int i);
    void removeTileFromPlayer(int player, Tile const & tile);
    void givePlayerRandomTile(int player, Random & random);
    void exchangePlayerTile(int player, Random & random);

    bool isInitialMoveDone() const;
    bool isBagEmpty() const;
    int getNumPlayers() const;

    PlayersState const & getPlayersState() const ;
    TileBoard const & getBoard() const;

    int getScore(int player) const;
    std::optional<int> getWinner() const;

private:
    TileBoard grid;
    FixVector<Tile, NUM_TOTAL_TILES> tileBag;
    int numPlayers;
    bool isInInitialState;
    PlayersState playerBags;
    std::array<int, MAX_NUM_PLAYERS> scores;
} ;

void getAllTileMoves(Spectrangle const & game, int player, MoveBuffer & buffer);
void getAllGameMoves(Spectrangle const & game, int player, GameMoveBuffer & buffer);
std::optional<Move> pickRandomTileMove(Spectrangle const & game, int player, Random & random);
std::optional<Move> pickRandomTileMoveFisherYates(Spectrangle const & game, int player, Random & random);
std::optional<int> playRandomGame(Spectrangle game, int currentPlayer, Random & random);
bool possibleMoveExists(Spectrangle const & game);
int getMultiplier(Vec2i pos);

#endif // SPECTRANGLE_H
