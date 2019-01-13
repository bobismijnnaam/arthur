#include <iostream>
#include <cmath>

#include "StochasticAI.h"
#include "Spectrangle.h"

StochasticScoreResult stochasticAI(Spectrangle game, int currentPlayer, Random& random, int cycles) {
    GameMoveBuffer gameMoveBuffer;
    StochasticScoreResult scores;

    getAllGameMoves(game, currentPlayer, gameMoveBuffer);
    for (int i = 0; i < gameMoveBuffer.getSize(); ++i) {
        scores.winCount.push(0);
        scores.testCount.push(0);
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
                break;
            case GameMoveType::SKIP:
                break;
            case GameMoveType::MOVE:
                newGame.applyMove(newPlayer, chosenMove.move, random);
                break;
        }

        newPlayer = (newPlayer + 1) % newGame.getNumPlayers();

        std::optional<int> winner = playRandomGame(newGame, newPlayer, random);
        if (winner && *winner == currentPlayer) {
            scores.winCount[chosenIndex]++;
        }

        scores.testCount[chosenIndex]++;
    }

    return scores;
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
        testCount[i] = 0;
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
        testCount[i] = 0;
    }
}

void PausableStochasticAI::think(Random & random) {
    StochasticScoreResult scores = stochasticAI(game, currentPlayer, random, cyclesPerIteration);

    winCount.size = scores.winCount.size;
    testCount.size = scores.testCount.size;

    for (int i = 0; i < scores.winCount.getSize(); i++) {
        winCount[i] += scores.winCount[i];
        testCount[i] += scores.testCount[i];
    }

    cyclesDone += cyclesPerIteration;
}

bool PausableStochasticAI::isFinished() const {
    return cyclesDone >= maxCycles;
}

GameMoveScoreBuffer PausableStochasticAI::getWinCount() const {
    return winCount;
}

GameMoveScoreBuffer PausableStochasticAI::getTestCount() const {
    return testCount;
}

GameMove PausableStochasticAI::getBestMove() const {
    return pickMove(game, currentPlayer, winCount);
}

float PausableStochasticAI::getProgress() const {
    return std::min(cyclesDone, maxCycles) / (float) maxCycles;
}

int PausableStochasticAI::getMaxScore() const {
    int max = 0;
    for (int i = 0; i < winCount.getSize(); ++i) {
        max = std::max(max, winCount[i]);
    }
    return max;
}

float PausableStochasticAI::getWinChance() const {
    int winCounts = 0;
    int testCounts = 0;

    for (int i = 0; i < winCount.getSize(); ++i) {
        winCounts += winCount[i];
        testCounts += testCount[i];
    }

    return winCounts / (float) testCounts;
}
