#include "Random.h"
#include "Spectrangle.h"
#include "Move.h"

GameMove randomAI(Spectrangle game, int player, Random random) {
    MoveBuffer mb;
    getAllTileMoves(game, player, mb);

    GameMove gm;
    if (mb.getSize() > 0) {
        // Pick a valid move
        gm.moveType = GameMoveType::MOVE;
        gm.move = mb[random.range(mb.getSize())];
    } else {
        // Pick either skip or exchange
        gm.moveType = (!game.isBagEmpty() && random.range(2) == 0) ? GameMoveType::EXCHANGE : GameMoveType::SKIP;
    }

    return gm;
}
