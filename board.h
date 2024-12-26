#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
public:
    Board(int size);
    void draw();
    void update(int x, int y, char player);
    bool isCellEmpty(int x, int y);

private:
    std::vector<std::vector<char>> board;
};

#endif // BOARD_H
