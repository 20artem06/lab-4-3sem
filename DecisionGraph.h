#ifndef DECISIONGRAPH_H
#define DECISIONGRAPH_H

#include <vector>
#include <unordered_map>
#include "board.h"
#include <string>
class Game;

class DecisionGraph {
public:
    struct Node {
        Board board;
        int depth;
        char player;
        int wins;
        int opponentWins;
        std::vector<Node*> neighbors;
        bool visited;
        Node(const Board& board, int depth, char player, int wins, int opponentWins);

        bool operator==(const Node& other) const;
    };
    DecisionGraph(int size, Game* game);
    ~DecisionGraph();

    Node* createNode(const Board& board, int depth, char player, int wins, int opponentWins);
    void generateGraph(Node& startNode, int maxDepth, std::vector<Node>& graph);
    std::pair<int, int> evaluateGraph(const std::vector<Node>& graph, char aiPlayer, int maxDepth);
    Node* findNode(const Board& board);
    void clearGraph();
    int size;
    std::vector<Node> nodes;
    std::unordered_map<std::string, Node*> nodeMap;
private:
    Game* gamePtr;
};

#endif // DECISIONGRAPH_H
