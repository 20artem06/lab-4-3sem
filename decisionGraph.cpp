#include<string>
#include "DecisionGraph.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>
#include <sstream>
#include <string>
#include "game.h"

DecisionGraph::DecisionGraph(int size, Game* game) : size(size), gamePtr(game) {}

DecisionGraph::~DecisionGraph() {
    for (Node& node : nodes) {
        for (Node* neighbor : node.neighbors) {
            if (neighbor)
                delete neighbor;
        }
    }
}

DecisionGraph::Node::Node(const Board& board, int depth, char player, int wins, int opponentWins) :
    board(board), depth(depth), player(player), wins(wins), opponentWins(opponentWins), visited(false) {}

bool DecisionGraph::Node::operator==(const Node& other) const {
    if (depth != other.depth || player != other.player || wins != other.wins || opponentWins != other.opponentWins)
        return false;

    for (int i = 0; i < board.getSize(); i++)
        for (int j = 0; j < board.getSize(); j++)
            if (board.isCellEmpty(i, j) != other.board.isCellEmpty(i, j))
                return false;
    return true;
}

DecisionGraph::Node* DecisionGraph::createNode(const Board& board, int depth, char player, int wins, int opponentWins) {
    Node* node = new Node(board, depth, player, wins, opponentWins);
    nodes.push_back(*node);
    return node;
}

DecisionGraph::Node* DecisionGraph::findNode(const Board& board) {
    std::ostringstream oss;
    for (int i = 0; i < board.getSize(); i++)
        for (int j = 0; j < board.getSize(); j++)
            oss << board.board[i][j];
    std::string boardKey = oss.str();

    if (nodeMap.find(boardKey) != nodeMap.end()) {
        return nodeMap[boardKey];
    }
    return nullptr;
}

void DecisionGraph::clearGraph() {
    for (Node& node : nodes) {
        for (Node* neighbor : node.neighbors) {
            if (neighbor)
                delete neighbor;
        }
    }
    nodes.clear();
    nodeMap.clear();
}

void DecisionGraph::generateGraph(Node& startNode, int maxDepth, std::vector<Node>& graph) {
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
                if (current->board.isCellEmpty(i, j)) {
                    Board newBoard = current->board;
                    newBoard.update(i, j, current->player);

                    Node* existingNode = findNode(newBoard);
                    if (existingNode) {
                        current->neighbors.push_back(existingNode);
                        continue;
                    }

                    Node* child = createNode(newBoard, current->depth + 1, current->player == 'X' ? 'O' : 'X', current->wins, current->opponentWins);
                    if (gamePtr->checkWin(current->player, child->board)) {  // Вызов метода через gamePtr
                        child->wins += 1;
                    } else if (gamePtr->checkWin(child->player, child->board)) { // Вызов метода через gamePtr
                        child->opponentWins += 1;
                    }

                    current->neighbors.push_back(child);
                    queue.push(child);
                    graph.push_back(*child);
                    std::ostringstream oss;
                    for (int k = 0; k < size; k++)
                        for (int l = 0; l < size; l++)
                            oss << newBoard.board[k][l];
                    std::string boardKey = oss.str();
                    nodeMap[boardKey] = child;

                }
            }
        }
        if (current->neighbors.empty()) {
            graph.push_back(*current);
        }
    }
}

bool DecisionGraph::checkWin(char symbol, const Board& gameBoard) {
    Board tempBoard = gameBoard;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!tempBoard.isCellEmpty(i, j))
            {
                Board tempBoard2 = tempBoard;
                if (tempBoard2.isCellEmpty(i, j) != (tempBoard2.update(i, j, ' '), true) && tempBoard2.isCellEmpty(i, j) != (tempBoard2.update(i, j, symbol), true)) {
                    if (gamePtr->checkVictory(i, j, symbol, tempBoard2)) { // Вызов метода через gamePtr
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool DecisionGraph::checkVictory(int x, int y, char symbol, const Board& board) {
    static const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    int winCondition = 0;
    if (size == 3) {
        winCondition = 3;
    }
    else if (size == 5) {
        winCondition = 4;
    }
    else if (size == 10) {
        winCondition = 5;
    }
    else {
        winCondition = 4;
    }

    for (int d = 0; d < 4; ++d) {
        int count = 1;

        for (int step = 1; step < winCondition; ++step) {
            Board tempBoard = board;
            int nx = x + step * directions[d][0];
            int ny = y + step * directions[d][1];
            if (nx >= 0 && nx < tempBoard.getSize() && ny >= 0 && ny < tempBoard.getSize() && !tempBoard.isCellEmpty(nx, ny)) {
                Board tempBoard2 = tempBoard;
                if(tempBoard2.isCellEmpty(nx,ny) != (tempBoard2.update(nx,ny,' '), true) && tempBoard2.isCellEmpty(nx,ny) != (tempBoard2.update(nx,ny,symbol), true))
                    count++;
            }
            else {
                break;
            }
        }

        for (int step = 1; step < winCondition; ++step) {
            Board tempBoard = board;
            int nx = x - step * directions[d][0];
            int ny = y + step * directions[d][1];
            if (nx >= 0 && nx < tempBoard.getSize() && ny >= 0 && ny < tempBoard.getSize() && !tempBoard.isCellEmpty(nx, ny)) {
                Board tempBoard2 = tempBoard;
                if(tempBoard2.isCellEmpty(nx,ny) != (tempBoard2.update(nx,ny,' '), true) && tempBoard2.isCellEmpty(nx,ny) != (tempBoard2.update(nx,ny,symbol), true))
                    count++;
            }
            else {
                break;
            }
        }

        if (count >= winCondition) {
            return true;
        }
    }

    return false;
}

std::pair<int, int> DecisionGraph::evaluateGraph(const std::vector<Node>& graph, char aiPlayer, int maxDepth) {
    int bestWins = -1;
    int bestOpponentWins = std::numeric_limits<int>::max();
    std::pair<int, int> bestMove = { -1, -1 };
    for (const auto& node : graph) {
        if (node.depth != maxDepth) continue;
        if (node.wins > bestWins) {
            bestWins = node.wins;
            bestOpponentWins = node.opponentWins;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    Board tempBoard = nodes[0].board;
                    if (!tempBoard.isCellEmpty(i, j) && tempBoard.isCellEmpty(i, j) != (tempBoard.update(i, j, ' '), true) && tempBoard.isCellEmpty(i, j) != (tempBoard.update(i, j, aiPlayer), true) && node.board.isCellEmpty(i, j) != (node.board.update(i, j, ' '), true) && node.board.isCellEmpty(i, j) != (node.board.update(i, j, aiPlayer), true))
                    {
                        bestMove = { i, j };
                        break;
                    }
                }
                if (bestMove.first != -1)
                    break;
            }
        }
        else if (node.wins == bestWins && node.opponentWins < bestOpponentWins) {
            bestOpponentWins = node.opponentWins;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    Board tempBoard = nodes[0].board;
                    if (!tempBoard.isCellEmpty(i, j) && tempBoard.isCellEmpty(i, j) != (tempBoard.update(i, j, ' '), true) && tempBoard.isCellEmpty(i, j) != (tempBoard.update(i, j, aiPlayer), true) && node.board.isCellEmpty(i, j) != (node.board.update(i, j, ' '), true) && node.board.isCellEmpty(i, j) != (node.board.update(i, j, aiPlayer), true))
                    {
                        bestMove = { i, j };
                        break;
                    }
                }
                if (bestMove.first != -1)
                    break;
            }
        }
    }

    return bestMove;
}
