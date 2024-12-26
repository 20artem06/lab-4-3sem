#include "game.h"

Game::Game(int size) : size(size) {
    board.resize(size, std::vector<char>(size, ' '));
    graph = new GameGraph(size); // Создание графа


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
    delete graph;
}

bool Game::makeMove(int x, int y, char symbol) {
    if (board[x][y] == ' ') {
        board[x][y] = symbol;
        return true;
    }
    return false;
}

bool Game::checkWin(char symbol) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == symbol) {

                if (checkVictory(i, j, symbol)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Game::checkVictory(int x, int y, char symbol) {
    static const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    for (int d = 0; d < 4; ++d) {
        int count = 1; // Начинаем с 1, так как мы нашли символ в (x, y)

        // положительное направление
        for (int step = 1; step < WIN_CONDITION; ++step) {
            int nx = x + step * directions[d][0];
            int ny = y + step * directions[d][1];
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && board[nx][ny] == symbol) {
                count++;
            } else {
                break;
            }
        }

        // отрицательное направление
        for (int step = 1; step < WIN_CONDITION; ++step) {
            int nx = x - step * directions[d][0];
            int ny = y - step * directions[d][1];
            if (nx >= 0 && nx < size && ny >= 0 && ny < size && board[nx][ny] == symbol) {
                count++;
            } else {
                break;
            }
        }

        if (count >= WIN_CONDITION) {
            return true; // ура победа
        }
    }

    return false;
}

void Game::reset() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = ' '; // сброс поля
        }
    }
}
