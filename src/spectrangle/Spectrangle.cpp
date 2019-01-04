#include <iostream>
#include <array>
#include <optional>

#include "TriangleGrid.h"
#include "Vec2i.h"
#include "Move.h"
#include "Spectrangle.h"

std::array<Tile, NUM_TOTAL_TILES> const tileBagStarterSet = {{
    {Color::RED, Color::BLUE, Color::GREEN},
    {Color::RED, Color::BLUE, Color::GREEN},
    {Color::RED, Color::BLUE, Color::GREEN}
}};

Spectrangle::Spectrangle(int numPlayers) {
    tileBag.data = tileBagStarterSet;
    tileBag.size = NUM_TOTAL_TILES;
    playerBags.size = numPlayers;
}

void Spectrangle::setNumPlayers(int numPlayers) {
    playerBags.size = numPlayers;
}

/**
 * Given a move, checks if the move is possible.
 */
bool Spectrangle::isMovePossible(Move const & move) const {
    if (!grid.isPosValid(move.pos)) {
        return false;
    }

    if (grid.get(move.pos).has_value()) {
        return false;
    }

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

bool Spectrangle::isBagEmpty() {
    return tileBag.getSize() == 0;
}

void Spectrangle::applyMove(Move const & move) {
    grid.set(move.pos, move.getTile());
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

#include "Random.h"

std::optional<Move> pickRandomMove(Spectrangle const & game, int player, Random & random) {

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
                    Move candidateMove({x, y}, tile, rot);
                    if (game.isMovePossible(candidateMove)) {
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
