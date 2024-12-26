#include "board.h"
#include <iostream>

Board::Board(int size) : board(size, std::vector<char>(size, ' ')) {}

void Board::draw() {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void Board::update(int x, int y, char player) {
    board[x][y] = player;
}

bool Board::isCellEmpty(int x, int y) {
    return board[x][y] == ' ';
}
