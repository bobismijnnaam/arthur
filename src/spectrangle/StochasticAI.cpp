#include <iostream>

#include "Spectrangle.h"

GameMove stochasticAI(Spectrangle game, int currentPlayer, Random& random, int cycles) {
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
                newGame.applyMove(newPlayer, chosenMove.move);
                newGame.removeTileFromPlayer(newPlayer, chosenMove.move.tile);
                newGame.givePlayerRandomTile(newPlayer, random);
                newPlayer = (newPlayer + 1) % newGame.getNumPlayers();
                break;
        }

        std::optional<int> winner = playRandomGame(newGame, newPlayer, random);
        if (winner && *winner == currentPlayer) {
            winCount[chosenIndex]++;
        }

        std::cout << "Finished a game!\n";
    }

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
