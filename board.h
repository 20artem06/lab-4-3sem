#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
public:
    Board(int size);
    void draw();
    bool update(int x, int y, char player);
    bool isCellEmpty(int x, int y) const;
    int getSize() const;
private:
    std::vector<std::vector<char>> board;
    int size;
};

#endif // BOARD_H
