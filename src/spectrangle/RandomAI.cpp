#include <iostream>

#include "Random.h"
#include "Spectrangle.h"
#include "Move.h"

GameMove randomAI(Spectrangle game, int player, Random& random) {
    GameMove gm;

    if (!game.isInitialMoveDone()) {
        // Do initial move
        gm.moveType = GameMoveType::MOVE;

        // Take tile from player and set as move
        int playerTileIndex = random.range(game.getPlayerNumTiles(player));
        Tile playerTile = game.takeTileFromPlayer(player, playerTileIndex);
        gm.move.tile = playerTile;

        // Take tile from bag and give to player
        int bagTileIndex = random.range(game.getNumTilesAvailable());
        Tile bagTile = game.takeTileFromBag(bagTileIndex);
        game.giveTileToPlayer(player, bagTile);

        // Pick a random rotation if the piece is not symmetrical
        gm.move.rotation = playerTile.isSymmetrical() ? 0 : random.range(3);

        // Pick a random position and return it
        int i = 0;
        int targetI = random.range(TileBoard::NUM_ELEMS);
        for (int y = 0; y < SPECTRANGLE_BOARD_SIDE; ++y) {
            for (int x = 0; x < TileBoard::rowLength(y); ++x) {
                if (i == targetI) {
                    gm.move.pos = Vec2i(x, y);
                    return gm;
                }
                ++i;
            }
        }
        
        std::cout << "SHOULD NOT BE TRIGGERED! FALLING BACK TO ORIGIN\n";
        gm.move.pos = Vec2i(0, 0);
    } else {
        if (std::optional<Move> move = pickRandomTileMove(game, player, random)) {
            // Pick a valid move
            gm.moveType = GameMoveType::MOVE;
            // gm.move = mb[random.range(mb.getSize())];
            gm.move = *move;
        } else {
            // Pick either skip or exchange
            gm.moveType = (!game.isBagEmpty() && random.range(2) == 0) ? GameMoveType::EXCHANGE : GameMoveType::SKIP;
        }
    }

    return gm;
}
