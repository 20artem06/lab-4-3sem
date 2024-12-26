#include "gameGraph.h"

GameGraph::GameGraph(int size) : size(size) {}

std::vector<std::pair<int, int>> GameGraph::getAdjacents(int x, int y) {
    std::vector<std::pair<int, int>> adjacents;
    // Проверка всех возможных направлений (8 направлений для клетки)
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Пропустить саму клетку
            if (x + dx >= 0 && x + dx < size && y + dy >= 0 && y + dy < size) {
                adjacents.emplace_back(x + dx, y + dy);
            }
        }
    }
    return adjacents;
}
