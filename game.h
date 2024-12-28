#ifndef GAME_H
#define GAME_H

#include <vector>
#include <unordered_set>
#include "player.h"
#include "board.h"


class Game {
public:
    Game(int size);
    ~Game();
    bool makeMove(int x, int y, char symbol);
    bool checkWin(char symbol);
    void reset();
    std::pair<int, int> getBestAIMove(int depth, char playerSymbol);

private:
    struct Node {
        Board board;
        int depth;
        char player;
        int wins;
        int opponentWins;
        std::vector<Node*> neighbors;
        bool visited;
        Node(const Board& board, int depth, char player, int wins, int opponentWins);
    };
    Node createNode(const Board& board, int depth, char player, int wins, int opponentWins);
    void generateDecisionGraph(Node& startNode, int maxDepth, std::vector<Node>& graph);
    std::pair<int, int> evaluateGraph(const std::vector<Node>& graph, char aiPlayer, int maxDepth);
    bool checkVictory(int x, int y, char symbol);
    bool checkWin(int x, int y, char symbol);
    bool checkWin(char symbol, const Board& gameBoard);
    int size;
    Board board;
    int WIN_CONDITION;
    int lastMoveX;
    int lastMoveY;
};

#endif // GAME_H
