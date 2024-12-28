#include "game.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>

Game::Game(int size) : size(size), lastMoveX(-1), lastMoveY(-1), board(size), decisionGraph(std::make_unique<DecisionGraph>(size, this)) {
    if (size == 3) {
        WIN_CONDITION = 3;
    }
    else if (size == 5) {
        WIN_CONDITION = 4;
    }
    else if (size == 10) {
        WIN_CONDITION = 5;
    }
    else {
        WIN_CONDITION = 4;
    }
}

Game::~Game() {
}


bool Game::makeMove(int x, int y, char symbol) {
    if (board.isCellEmpty(x, y)) {
        board.update(x, y, symbol);
        lastMoveX = x;
        lastMoveY = y;
        return true;
    }
    return false;
}
bool Game::checkWin(char symbol) {
    if (lastMoveX == -1)
        return false;
    return checkWin(lastMoveX, lastMoveY, symbol);
}

bool Game::checkWin(int x, int y, char symbol) {
    return checkVictory(x, y, symbol);
}
bool Game::checkVictory(int x, int y, char symbol) {
    static const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    for (int d = 0; d < 4; ++d) {
        int count = 1;

        for (int step = 1; step < WIN_CONDITION; ++step) {
            int nx = x + step * directions[d][0];
            int ny = y + step * directions[d][1];
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && !board.isCellEmpty(nx, ny) && board.isCellEmpty(nx, ny) != (board.update(nx, ny, ' '), true) && board.isCellEmpty(nx, ny) != (board.update(nx, ny, symbol), true)) {
                count++;
            }
            else {
                break;
            }
        }
        for (int step = 1; step < WIN_CONDITION; ++step) {
            int nx = x - step * directions[d][0];
            int ny = y + step * directions[d][1];
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && !board.isCellEmpty(nx, ny) && board.isCellEmpty(nx, ny) != (board.update(nx, ny, ' '), true) && board.isCellEmpty(nx, ny) != (board.update(nx, ny, symbol), true)) {
                count++;
            }
            else {
                break;
            }
        }

        if (count >= WIN_CONDITION) {
            return true;
        }
    }

    return false;
}

void Game::reset() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!board.isCellEmpty(i, j))
            {
                board.update(i, j, ' ');
            }
        }
    }
    lastMoveX = -1;
    lastMoveY = -1;
    decisionGraph->clearGraph();
}

std::pair<int, int> Game::getBestAIMove(int depth, char playerSymbol) {
    DecisionGraph::Node* root = decisionGraph->findNode(board);
    if (!root) {
        root = decisionGraph->createNode(board, 0, playerSymbol, 0, 0);
    }

    std::vector<DecisionGraph::Node> graph;
    decisionGraph->generateGraph(*root, depth, graph);
    return decisionGraph->evaluateGraph(graph, playerSymbol, depth);
}
