#include "Spectrangle.h"

using GameMoveScoreBuffer = FixVector<int, NUM_MAX_POSSIBLE_GAMEMOVES>;

struct StochasticScoreResult {
    GameMoveScoreBuffer winCount;
    GameMoveScoreBuffer testCount;
} ;

StochasticScoreResult stochasticAI(Spectrangle game, int currentPlayer, Random& random, int cycles);
GameMove pickMove(Spectrangle const & game, int currentPlayer, GameMoveScoreBuffer const & winCount);

class PausableStochasticAI {
public:
    PausableStochasticAI(int cyclesPerIterationArg, int maxCycles);
    
    void setCyclesPerIteration(int cyclesPerIterationArg);
    void setMaxCycles(int maxCyclesArg);

    void setGameAndResetState(Spectrangle gameArg, int currentPlayer);
    void think(Random & random);

    bool isFinished() const;
    GameMoveScoreBuffer getWinCount() const;
    GameMoveScoreBuffer getTestCount() const;
    GameMove getBestMove() const;
    float getProgress() const;
    int getMaxScore() const;
    float getWinChance() const;

private:
    Spectrangle game;
    int currentPlayer;

    int cyclesDone;
    int cyclesPerIteration;
    int maxCycles;
    GameMoveScoreBuffer winCount;
    GameMoveScoreBuffer testCount;

} ;
