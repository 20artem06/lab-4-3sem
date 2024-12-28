#include "board.h"
#include <iostream>

Board::Board(int size) : board(size, std::vector<char>(size, ' ')), size(size) {}

void Board::draw() {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

bool Board::update(int x, int y, char player)  {
    if (x >= 0 && x < size && y >= 0 && y < size) {
        board[x][y] = player;
        return true;
    }
    return false;
}

bool Board::isCellEmpty(int x, int y) const {
    if (x >= 0 && x < size && y >= 0 && y < size) {
        return board[x][y] == ' ';
    }
    return false;
}

