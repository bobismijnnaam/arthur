#include <iostream>

#include "Random.h"
#include "Spectrangle.h"
#include "Move.h"

GameMove randomAI(Spectrangle game, int player, Random& random) {
    GameMoveBuffer buffer;
    getAllGameMoves(game, player, buffer);

    return buffer[random.range(buffer.getSize())];
}
