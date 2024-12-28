
#include "game.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>

Game::Game(int size) : size(size), lastMoveX(-1), lastMoveY(-1), board(size) {
    if (size == 3) {
        WIN_CONDITION = 3;
    } else if (size == 5) {
        WIN_CONDITION = 4;
    } else if (size == 10) {
        WIN_CONDITION = 5;
    } else {
        WIN_CONDITION = 4;
    }
}

Game::~Game() {
}
Game::Node::Node(const Board& board, int depth, char player, int wins, int opponentWins) :
    board(board), depth(depth), player(player), wins(wins), opponentWins(opponentWins), visited(false) {}


bool Game::makeMove(int x, int y, char symbol) {
    if (board.isCellEmpty(x,y)) {
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
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && !board.isCellEmpty(nx, ny) && board.isCellEmpty(nx,ny) != (board.update(nx,ny,' ') ,  true) && board.isCellEmpty(nx,ny) != (board.update(nx,ny,symbol), true) ) {
                count++;
            } else {
                break;
            }
        }
        for (int step = 1; step < WIN_CONDITION; ++step) {
            int nx = x - step * directions[d][0];
            int ny = y + step * directions[d][1];
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && !board.isCellEmpty(nx, ny) && board.isCellEmpty(nx,ny) != (board.update(nx,ny,' ') ,  true) && board.isCellEmpty(nx,ny) != (board.update(nx,ny,symbol), true) ) {
                count++;
            } else {
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
            if(!board.isCellEmpty(i,j))
            {
                board.update(i,j,' ');
            }
        }
    }
    lastMoveX = -1;
    lastMoveY = -1;
}

Game::Node Game::createNode(const Board& board, int depth, char player, int wins, int opponentWins) {
    Node node(board, depth, player, wins, opponentWins);
    return node;
}
bool Game::checkWin(char symbol, const Board& gameBoard) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!gameBoard.isCellEmpty(i,j) && gameBoard.isCellEmpty(i,j) != (gameBoard.update(i,j,' '), true) && gameBoard.isCellEmpty(i,j) != (gameBoard.update(i,j,symbol),true)) {
                if (checkVictory(i, j, symbol)) {
                    return true;
                }
            }
        }
    }
    return false;
}
void Game::generateDecisionGraph(Node& startNode, int maxDepth, std::vector<Node>& graph) {
    std::queue<Node*> queue;
    queue.push(&startNode);
    startNode.visited = true;

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();

        if (current->depth >= maxDepth) {
            graph.push_back(*current);
            continue;
        }

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (current->board.isCellEmpty(i,j)) {
                    Board newBoard = current->board;
                    newBoard.update(i,j, current->player);

                    Node child(newBoard, current->depth + 1, current->player == 'X' ? 'O' : 'X', current->wins, current->opponentWins);
                    if (checkWin(current->player, child.board)) {
                        child.wins += 1;
                    } else if (checkWin(child.player, child.board)) {
                        child.opponentWins += 1;
                    }
                    current->neighbors.push_back(new Node(child));
                    queue.push(current->neighbors.back());
                    graph.push_back(child);

                }
            }
        }
        if(current->neighbors.empty())
        {
            graph.push_back(*current);
        }
    }
}

std::pair<int, int> Game::evaluateGraph(const std::vector<Node>& graph, char aiPlayer, int maxDepth) {
    int bestWins = -1;
    int bestOpponentWins = std::numeric_limits<int>::max();
    std::pair<int, int> bestMove = {-1, -1};
    for(const auto& node : graph)
    {
        if(node.depth != maxDepth) continue;
        if(node.wins > bestWins)
        {
            bestWins = node.wins;
            bestOpponentWins = node.opponentWins;
            for(int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    Board tempBoard = board;
                    if(!tempBoard.isCellEmpty(i,j) && tempBoard.isCellEmpty(i,j) != (tempBoard.update(i,j, ' '), true) && tempBoard.isCellEmpty(i,j) != (tempBoard.update(i,j, symbol), true) && node.board.isCellEmpty(i,j) != (node.board.update(i,j,' '), true) && node.board.isCellEmpty(i,j) != (node.board.update(i,j,symbol), true))
                    {
                        bestMove = {i, j};
                        break;
                    }
                }
                if(bestMove.first != -1)
                    break;
            }
        }
        else if(node.wins == bestWins && node.opponentWins < bestOpponentWins)
        {
            bestOpponentWins = node.opponentWins;
            for(int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    Board tempBoard = board;
                    if(!tempBoard.isCellEmpty(i,j) && tempBoard.isCellEmpty(i,j) != (tempBoard.update(i,j, ' '), true) && tempBoard.isCellEmpty(i,j) != (tempBoard.update(i,j, symbol), true) && node.board.isCellEmpty(i,j) != (node.board.update(i,j,' '), true) && node.board.isCellEmpty(i,j) != (node.board.update(i,j,symbol), true))
                    {
                        bestMove = {i, j};
                        break;
                    }
                }
                if(bestMove.first != -1)
                    break;
            }
        }
    }

    return bestMove;
}

std::pair<int, int> Game::getBestAIMove(int depth, char playerSymbol) {
    Node root = createNode(board, 0, playerSymbol, 0, 0);
    std::vector<Node> graph;
    generateDecisionGraph(root, depth, graph);
    return evaluateGraph(graph, playerSymbol, depth);
}
