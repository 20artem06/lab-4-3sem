#ifndef GAMEGRAPH_H
#define GAMEGRAPH_H

#include <vector>
#include <utility>

class GameGraph {
public:
    GameGraph(int size);
    std::vector<std::pair<int, int>> getAdjacents(int x, int y);

private:
    int size;
};

#endif // GAMEGRAPH_H
