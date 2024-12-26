#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "gameGraph.h"

class Game {
public:
    Game(int size);
    ~Game();
    bool makeMove(int x, int y, char symbol);
    bool checkWin(char symbol);
    void reset();

private:
    bool checkVictory(int x, int y, char symbol);
    int size;
    std::vector<std::vector<char>> board;
    GameGraph* graph;
    int WIN_CONDITION;
};

#endif // GAME_H
