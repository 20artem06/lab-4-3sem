#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <utility>
#include <memory>

class DecisionGraph;
class Game {
public:
    Game(int size);
    ~Game();
    bool makeMove(int x, int y, char symbol);
    bool checkWin(char symbol);
    bool checkWin(int x, int y, char symbol);
    bool checkVictory(int x, int y, char symbol);
    void reset();
    std::pair<int, int> getBestAIMove(int depth, char playerSymbol);
    int size;
private:
    Board board;
    int lastMoveX;
    int lastMoveY;
    int WIN_CONDITION;
    std::unique_ptr<DecisionGraph> decisionGraph;
};

#endif
