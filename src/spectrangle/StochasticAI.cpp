#include <iostream>
#include <cmath>

#include "StochasticAI.h"
#include "Spectrangle.h"

GameMoveScoreBuffer stochasticAI(Spectrangle game, int currentPlayer, Random& random, int cycles) {
    GameMoveBuffer gameMoveBuffer;
    FixVector<int, NUM_MAX_POSSIBLE_GAMEMOVES> winCount;

    getAllGameMoves(game, currentPlayer, gameMoveBuffer);
    for (int i = 0; i < gameMoveBuffer.getSize(); ++i) {
        winCount.push(0);
    }

    // Play many games
    for (int i = 0; i < cycles; ++i) {
        Spectrangle newGame = game;
        int newPlayer = currentPlayer;
        int chosenIndex = random.range(gameMoveBuffer.getSize());
        GameMove const & chosenMove = gameMoveBuffer[chosenIndex];

        switch (chosenMove.moveType) {
            case GameMoveType::EXCHANGE:
                newGame.exchangePlayerTile(newPlayer, random);
            case GameMoveType::SKIP:
                newPlayer = (newPlayer + 1) % newGame.getNumPlayers();
                break;
            case GameMoveType::MOVE:
                newGame.applyMove(newPlayer, chosenMove.move, random);
                newPlayer = (newPlayer + 1) % newGame.getNumPlayers();
                break;
        }

        std::optional<int> winner = playRandomGame(newGame, newPlayer, random);
        if (winner && *winner == currentPlayer) {
            winCount[chosenIndex]++;
        }
    }

    return winCount;
}

GameMove pickMove(Spectrangle const & game, int currentPlayer, GameMoveScoreBuffer const & winCount) {
    GameMoveBuffer gameMoveBuffer;
    getAllGameMoves(game, currentPlayer, gameMoveBuffer);

    // Find the best move
    GameMove bestGameMove = gameMoveBuffer[0];
    int bestGameMoveScore = winCount[0];
    for (int i = 1; i < gameMoveBuffer.getSize(); ++i) {
        if (winCount[i] > bestGameMoveScore) {
            bestGameMoveScore = winCount[i];
            bestGameMove = gameMoveBuffer[i];
        }
    }

    return bestGameMove;
}

//////////////////////////
// PausableStochasticAI //
//////////////////////////

PausableStochasticAI::PausableStochasticAI(int cyclesPerIterationArg, int maxCyclesArg) :
    game(2),
    cyclesPerIteration{cyclesPerIterationArg},
    maxCycles{maxCyclesArg} {

    for (int i = 0; i < winCount.getCapacity(); ++i) {
        winCount[i] = 0;
    }
}

void PausableStochasticAI::setCyclesPerIteration(int cyclesPerIterationArg) {
    cyclesPerIteration = cyclesPerIterationArg;
}

void PausableStochasticAI::setMaxCycles(int maxCyclesArg) {
    maxCycles = maxCyclesArg;
}

void PausableStochasticAI::setGameAndResetState(Spectrangle gameArg, int currentPlayerArg) {
    game = gameArg;
    currentPlayer = currentPlayerArg;
    cyclesDone = 0;

    for (int i = 0; i < winCount.getCapacity(); ++i) {
        winCount[i] = 0;
    }
}

void PausableStochasticAI::think(Random & random) {
    GameMoveScoreBuffer scoreBuffer = stochasticAI(game, currentPlayer, random, cyclesPerIteration);

    winCount.size = scoreBuffer.size;

    for (int i = 0; i < scoreBuffer.getSize(); i++) {
        winCount[i] += scoreBuffer[i];
    }

    cyclesDone += cyclesPerIteration;
}

bool PausableStochasticAI::isFinished() const {
    return cyclesDone >= maxCycles;
}

GameMoveScoreBuffer PausableStochasticAI::getWinCount() const {
    return winCount;
}

GameMove PausableStochasticAI::getBestMove() const {
    return pickMove(game, currentPlayer, winCount);
}

float PausableStochasticAI::getProgress() const {
    return std::min(cyclesDone, maxCycles) / (float) maxCycles;
}
