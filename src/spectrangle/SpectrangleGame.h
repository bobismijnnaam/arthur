#include "Spectrangle.h"
#include "Move.h"

class SpectrangleGame {
public:
    SpectrangleGame(int numPlayers, int startPlayer);

    void applyMove(GameMove move, Random & random);

    bool isFinished();
    std::optional<int> getWinner();

    // Variables needed to track the game state
    int numPlayers;
    int currentPlayer;
    int missedTurns;

    Spectrangle board;
} ;
