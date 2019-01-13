#include <iostream>

#include "Random.h"
#include "SpectrangleGame.h"

SpectrangleGame::SpectrangleGame(int numPlayers, int startPlayer, Random & random)
        : numPlayers{numPlayers}
        , currentPlayer{startPlayer}
        , missedTurns{0}
        , board(numPlayers){
    board.initializePlayerBags(random);
}

void SpectrangleGame::applyMove(GameMove move, Random & random) {
    switch (move.moveType) {
        case GameMoveType::EXCHANGE:
            board.exchangePlayerTile(move.player, random);
            missedTurns++;
            break;

        case GameMoveType::SKIP:
            missedTurns++;
            break;

        case GameMoveType::MOVE:
            board.applyMove(currentPlayer, move.move, random);
            missedTurns = 0;
            break;

        default:
            std::cout << "moveType encountered that is not moveType: " << (int) move.moveType << "\n";
            break;
    }

    currentPlayer = (currentPlayer + 1) % numPlayers;
}

bool SpectrangleGame::isFinished() {
    if (missedTurns >= numPlayers) {
        return !possibleMoveExists(board);
    } else {
        return false;
    }
}

std::optional<int> SpectrangleGame::getWinner() {
    return board.getWinner();
}
