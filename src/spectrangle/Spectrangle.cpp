#include <iostream>
#include <array>
#include <optional>

#include "TriangleGrid.h"
#include "Vec2i.h"
#include "Move.h"
#include "Spectrangle.h"

#define ALL(color) {Color::color, Color::color, Color::color, 6}
#define T215(color2, color1) {Color::color2, Color::color2, Color::color1, 5}
#define T214(color2, color1) {Color::color2, Color::color2, Color::color1, 4}
#define T3(c1, c2, c3) {Color::c1, Color::c2, Color::c3, 3}
#define T2(c1, c2, c3) {Color::c1, Color::c2, Color::c3, 2}
#define T1(c1, c2, c3) {Color::c1, Color::c2, Color::c3, 1}

std::array<Tile, NUM_TOTAL_TILES> const tileBagStarterSet = {{
    ALL(RED),
    ALL(BLUE),
    ALL(GREEN),
    ALL(YELLOW),
    ALL(PURPLE),

    T215(RED, YELLOW),
    T215(RED, PURPLE),
    T215(BLUE, RED),
    T215(BLUE, PURPLE),
    T215(GREEN, RED),
    T215(GREEN, BLUE),
    T215(YELLOW, GREEN),
    T215(YELLOW, BLUE),
    T215(PURPLE, YELLOW),
    T215(PURPLE, GREEN),

    T214(RED, BLUE),
    T214(RED, GREEN),
    T214(BLUE, GREEN),
    T214(BLUE, YELLOW),
    T214(GREEN, YELLOW),
    T214(GREEN, PURPLE),
    T214(YELLOW, RED),
    T214(YELLOW, PURPLE),
    T214(PURPLE, RED),
    T214(PURPLE, BLUE),

    T3(YELLOW, BLUE, PURPLE),
    T3(RED, GREEN, YELLOW),
    T3(BLUE, GREEN, PURPLE),
    T3(GREEN, RED, BLUE),

    T2(BLUE, RED, PURPLE),
    T2(YELLOW, PURPLE, RED),
    T2(YELLOW, PURPLE, GREEN),

    T1(GREEN, RED, PURPLE),
    T1(BLUE, YELLOW, GREEN),
    T1(RED, YELLOW, BLUE),

    T1(WHITE, WHITE, WHITE)
}};

Spectrangle::Spectrangle(int numPlayersArg) :
        // Set all scores to zero
        scores{} 
        {
    numPlayers = numPlayersArg;

    tileBag.data = tileBagStarterSet;
    tileBag.size = NUM_TOTAL_TILES;

    // Fixvector always initializes size to zero. So we only have to set
    // the size to numPlayers here to have a few empty playerbags available
    playerBags.size = numPlayers;

    isInInitialState = true;
}

void Spectrangle::setNumPlayers(int numPlayersArg) {
    numPlayers = numPlayersArg;
    playerBags.size = numPlayersArg;
}

void Spectrangle::initializePlayerBags(Random& random) {
    for (int player = 0; player < numPlayers; ++player) {
        for (int i = 0; i < 4; ++i) {
            int tileIndex = random.range(getNumTilesAvailable());
            Tile tile = takeTileFromBag(tileIndex);
            giveTileToPlayer(player, tile);
        }
    }
}

bool Spectrangle::isMovePossible(int player, Move const & move) const {
    // The tile should be available
    if (move.tileIndex >= getPlayerNumTiles(player)) {
        return false;
    }

    Tile const & tile = getTileFromPlayer(player, move.tileIndex);

    return isMovePossible(tile, move);
}

/**
 * Given a move, checks if the move is possible.
 */
bool Spectrangle::isMovePossible(Tile const & tile, Move const & move) const {
    if (!grid.isPosValid(move.pos)) {
        return false;
    }

    if (grid.get(move.pos).has_value()) {
        return false;
    }

    Color color0 = move.getSide(tile, 0);
    Color color1 = move.getSide(tile, 1);
    Color color2 = move.getSide(tile, 2);

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
            || otherColor1 == color1)
        && (otherColor2 == Color::NONE 
            || otherColor2 == Color::WHITE
            || color2 == Color::WHITE
            || otherColor2 == color2)
        && ( otherColor0 != Color::NONE
             || otherColor1 != Color::NONE
             || otherColor2 != Color::NONE
           )
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
            default:
                std::cout << "Side was passed that is not a side: " << side << "\n";
                std::exit(1);
        }
    } else {
        // isDownTriangle
        switch (side) {
            case 0:
                return { pos.x + 1, pos.y };
            case 1:
                return { pos.x - 1, pos.y };
            case 2:
                return { pos.x - 1, pos.y - 1 };
            default:
                std::cout << "Side was passed that is not a side: " << side << "\n";
                std::exit(1);
        }
    }
}

Side neighbouringSide(Vec2i const & pos, Side side) {
    switch (side) {
        case 0:
            if (pos.x % 2 == 0) {
                return 1;
            } else {
                return 2;
            }
        case 1:
            if (pos.x % 2 == 0) {
                return 2;
            } else {
                return 0;
            }
        case 2:
            if (pos.x % 2 == 0) {
                return 0;
            } else {
                return 1;
            }
        default:
            std::cout << "Side was passed that is not a side: " << side << "\n";
            std::exit(1);
    }
}

Color Spectrangle::getNeighbourColorAtSide(Vec2i const pos, Side side) const {
    Side neighbourSide = neighbouringSide(pos, side);

    Vec2i neighbour = neighbourCoordinateAtSide(pos, side);

    if (grid.isPosValid(neighbour)) {
        std::optional<Tile> const & tile = grid.get(neighbour);
        if (tile.has_value()) {
            return (*tile).sides[neighbourSide];
        } else {
            return Color::NONE;
        }
    } else {
        return Color::NONE;
    }   
}

bool Spectrangle::isBagEmpty() const {
    return tileBag.getSize() == 0;
}

void Spectrangle::applyMove(int player, Move const & move, Random & random) {
    isInInitialState = false;
    grid.set(move.pos, move.getTile(getTileFromPlayer(player, move.tileIndex)));
    int numNeighbours = getNumNeighbours(move.pos);
    scores[player] += getMultiplier(move.pos) * playerBags[player][move.tileIndex].score * (numNeighbours == 0 ? 1 : numNeighbours);

    takeTileFromPlayer(player, move.tileIndex);
    if (getNumTilesAvailable() > 0) {
        givePlayerRandomTile(player, random);
    }
}

int Spectrangle::getNumNeighbours(Vec2i pos) const {
    FixVector<Vec2i, 3> neighbours;
    TileBoard::getNeighbours(pos, neighbours);

    int occupiedNeighbours = 0;
    for (int i = 0; i < neighbours.getSize(); i++) {
        Vec2i const & pos = neighbours[i];
        if (grid.get(pos).has_value()) {
            ++occupiedNeighbours;
        }
    }

    return occupiedNeighbours;
}

int Spectrangle::getNumTilesAvailable() const {
    return tileBag.getSize();
}

void Spectrangle::removeTileFromBag(Tile const tile) {
    tileBag.removeElem(tile);
}

Tile Spectrangle::takeTileFromBag(int i) {
    return tileBag.removeIndex(i);
}

Tile Spectrangle::getTileFromBag(int i) const {
    return tileBag[i];
}

void Spectrangle::putTileInBag(Tile const & tile) {
    tileBag.push(tile);
}

int Spectrangle::getPlayerNumTiles(int player) const {
    return playerBags[player].getSize();
}

Tile Spectrangle::getTileFromPlayer(int player, int i) const {
    return playerBags[player].get(i);
}

void Spectrangle::giveTileToPlayer(int player, Tile const tile) {
    playerBags[player].push(tile);
}

Tile Spectrangle::takeTileFromPlayer(int player, int i) {
    return playerBags[player].removeIndex(i);
}

void Spectrangle::removeTileFromPlayer(int player, Tile const & tile) {
    playerBags[player].removeElem(tile);
}

void Spectrangle::givePlayerRandomTile(int player, Random & random) {
    int bagTileIndex = random.range(getNumTilesAvailable());
    Tile bagTile = takeTileFromBag(bagTileIndex);
    giveTileToPlayer(player, bagTile);
}

void Spectrangle::exchangePlayerTile(int player, Random & random) {
    int bagTileIndex = random.range(getNumTilesAvailable());
    Tile bagTile = takeTileFromBag(bagTileIndex);

    int playerTileIndex = random.range(getPlayerNumTiles(player));
    Tile playerTile = takeTileFromPlayer(player, playerTileIndex);

    putTileInBag(playerTile);
    giveTileToPlayer(player, bagTile);
}

PlayersState const & Spectrangle::getPlayersState() const {
    return playerBags;
}

TileBoard const & Spectrangle::getBoard() const {
    return grid;
}

int Spectrangle::getScore(int player) const {
    return scores[player];
}

bool Spectrangle::isInitialMoveDone() const {
    return !isInInitialState;
}

std::optional<int> Spectrangle::getWinner() const {
    int winner = 0;
    int maxScore = scores[0];
    bool sawDouble = false;

    for (int player = 1; player < numPlayers; player++) {
        if (scores[player] == maxScore) {
            sawDouble = true;
        } else if(scores[player] > maxScore) {
            winner = player;
            maxScore = scores[player];
            sawDouble = false;
        }
    }

    if (!sawDouble) {
        return winner;
    } else {
        return {};
    }
}

int Spectrangle::getNumPlayers() const {
    return numPlayers;
}

#include "Random.h"

void getAllTileMoves(Spectrangle const & game, int player, MoveBuffer & buffer) {
    buffer.clear();
    int numTiles = game.getPlayerNumTiles(player);

    for (int tileIndex = 0; tileIndex < numTiles; tileIndex++) {
        Tile tile = game.getTileFromPlayer(player, tileIndex);
        int maxNumRotations = tile.isSymmetrical() ? 1 : 3;
        for (Rotation rot = 0; rot < maxNumRotations; rot++) {
            for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; y++) {
                int rowLength = TriangleGrid<bool, SPECTRANGLE_BOARD_SIDE>::rowLength(y);
                for (int x = 0; x < rowLength; x++) {
                    Move candidateMove({x, y}, tileIndex, rot);
                    if (game.isMovePossible(player, candidateMove)) {
                        buffer.push(candidateMove);
                    }
                }
            }
        }   
    }
}

void getAllGameMoves(Spectrangle const & game, int player, GameMoveBuffer & buffer) {
    if (!game.isInitialMoveDone()) {
        buffer.clear();

        for (int tileIndex = 0; tileIndex < game.getPlayerNumTiles(player); ++tileIndex) {
            Tile const & tile = game.getTileFromPlayer(player, tileIndex);
            Rotation const numRotations = tile.isSymmetrical() ? 1 : 3;
            for (Rotation rotation = 0; rotation < numRotations; ++rotation) {
                for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; ++y) {
                    for (int x = 0; x < TileBoard::rowLength(y); ++x) {
                        // Only add positions if they have no multiplier
                        if (getMultiplier({x, y}) == 1) {
                            buffer.push(GameMove::TileMove(player, {{x, y}, tileIndex, rotation}));
                        }
                    }
                }
            }
        }
    } else {
        MoveBuffer moveBuffer;
        getAllTileMoves(game, player, moveBuffer);

        if (moveBuffer.getSize() == 0) {
            // Add the skips
            if (!game.isBagEmpty() && game.getPlayerNumTiles(player) > 0) {
                buffer.push(GameMove::Exchange(player));
            } 

            buffer.push(GameMove::Skip(player));
        } else {
            // Add all the moves as GameMoves
            for (int i = 0; i < moveBuffer.getSize(); ++i) {
                buffer.push(GameMove::TileMove(player, moveBuffer[i]));
            }
        }
    }
}

std::optional<Move> pickRandomTileMove(Spectrangle const & game, int player, Random & random) {

    std::optional<Move> move;
    int chanceCounter = 1;
    int numTiles = game.getPlayerNumTiles(player);

    for (int tileIndex = 0; tileIndex < numTiles; tileIndex++) {
        Tile tile = game.getTileFromPlayer(player, tileIndex);
        int maxNumRotations = tile.isSymmetrical() ? 1 : 3;
        for (Rotation rot = 0; rot < maxNumRotations; rot++) {
            for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; y++) {
                int rowLength = TriangleGrid<bool, SPECTRANGLE_BOARD_SIDE>::rowLength(y);
                for (int x = 0; x < rowLength; x++) {
                    Move candidateMove({x, y}, tileIndex, rot);
                    if (game.isMovePossible(player, candidateMove)) {
                        if (random.range(chanceCounter) == 0) {
                            move = candidateMove;
                        }
                        chanceCounter++;
                    }
                }
            }
        }   
    }

    return move;
}

using MovesPermutationsBuffer = std::array<MoveBuffer, MAX_TILES_PER_PLAYER>;

MovesPermutationsBuffer getAllPossibleMoves() {
    MovesPermutationsBuffer res;

    for (int numTilesLeft = 1; numTilesLeft <= MAX_TILES_PER_PLAYER; ++numTilesLeft) {
        MoveBuffer & moveBuffer = res[numTilesLeft - 1];
        for (int tileIndex = 0; tileIndex < numTilesLeft; ++tileIndex) {
            for (Rotation rotation = 0; rotation < 3; rotation++) {
                for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; ++y) {
                    for (int x = 0; x < TileBoard::rowLength(y); ++x) {
                        Move candidateMove({x, y}, tileIndex, rotation);
                        moveBuffer.push(candidateMove);
                    }
                }
            }
        }
    }

    return res;
}

MovesPermutationsBuffer allPossibleMoves = getAllPossibleMoves();

std::optional<Move> pickRandomTileMoveFisherYates(Spectrangle const & game, int player, Random & random) {
    MoveBuffer & moveBuffer = allPossibleMoves[game.getPlayerNumTiles(player) - 1];

    for (int i = 0; i < moveBuffer.getSize(); ++i) {
        int moveIndex = random.range(moveBuffer.getSize() - i);

        // Swap picked move and move last in the array
        Move candidateMove = moveBuffer[moveIndex];
        Move moveToBeSaved = moveBuffer[NUM_MAX_POSSIBLE_MOVES - i - 1];

        moveBuffer[NUM_MAX_POSSIBLE_MOVES - i - 1] = candidateMove;
        moveBuffer[moveIndex] = moveToBeSaved;
        
        if (game.isMovePossible(player, candidateMove)) {
            return candidateMove;
        }
    }

    return {};
}

bool possibleMoveExists(Spectrangle const & game) {
    for (int player = 0; player < game.getNumPlayers(); ++player) {
        for (int tileIndex = 0; tileIndex < game.getPlayerNumTiles(player); ++tileIndex) {
            Tile tile = game.getTileFromPlayer(player, tileIndex);
            int maxNumRotations = tile.isSymmetrical() ? 1 : 3;
            for (Rotation rotation = 0; rotation < maxNumRotations; ++rotation) {
                for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; ++y) {
                    for (int x = 0; x < TileBoard::rowLength(y); ++x) {
                        Move candidateMove({x, y}, tileIndex, rotation);
                        if (game.isMovePossible(player, candidateMove)) {
                            return true;
                        }
                    }
                }
            }
        }
    }

    // Check if any tiles from the bag work
    int numTiles = game.getNumTilesAvailable();
    for (int tileIndex = 0; tileIndex < numTiles; tileIndex++) {
        Tile tile = game.getTileFromBag(tileIndex);
        Rotation maxNumRotations = tile.isSymmetrical() ? 1 : 3;
        for (Rotation rot = 0; rot < maxNumRotations; rot++) {
            for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; y++) {
                int rowLength = TriangleGrid<bool, SPECTRANGLE_BOARD_SIDE>::rowLength(y);
                for (int x = 0; x < rowLength; x++) {
                    Move candidateMove({x, y}, tileIndex, rot);
                    if (game.isMovePossible(tile, candidateMove)) {
                        return true;
                    }
                }
            }
        }   
    }

    return false;
}

std::optional<int> playRandomGame(Spectrangle game, int currentPlayer, Random & random) {
    bool done = false;
    std::optional<int> winner;
    int missedTurns = 0;

    if (!game.isInitialMoveDone()) {
        std::cout << "Cannot play a random game where the initial move is not yet done!\n";
        std::exit(1);
    }

    while (!done) {
        if (missedTurns >= game.getNumPlayers() && !possibleMoveExists(game)) {
            done = true;
            winner = game.getWinner();
        } else {
            std::optional<Move> candidateMove = pickRandomTileMoveFisherYates(game, currentPlayer, random);

            if (candidateMove) {
                // If a move is possible it must be done!
                game.applyMove(currentPlayer, *candidateMove, random);
                
                // Reset the missed turns counter to zero because someone made a move
                missedTurns = 0;
            } else {
                // Miss a turn!
                missedTurns++;

                // Either skip a turn or (if the bag is not empty) exchange a tile
                if (!game.isBagEmpty() && game.getPlayerNumTiles(currentPlayer) > 0 && random.range(2) == 0) {
                    game.exchangePlayerTile(currentPlayer, random);
                } 
            }

            // Move to next player
            currentPlayer = (currentPlayer + 1) % game.getNumPlayers();
        }
    }

    return winner;
}

int getMultiplier(Vec2i pos) {
    switch (pos.y) {
        case 1:
            switch (pos.x) {
                case 1: return 3;
                default: return 1;
            }
        case 3:
            switch (pos.x) {
                case 1:
                case 5: return 2;
                case 2:
                case 4: return 4;
                default: return 1;
            }
        case 4:
            switch (pos.x) {
                case 4: return 4;
                default: return 1;
            }
        case 5:
            switch (pos.x) {
                case 1:
                case 9: return 3;
                case 5: return 2;
                default: return 1;
            }
        default:
            return 1;
    }
}
