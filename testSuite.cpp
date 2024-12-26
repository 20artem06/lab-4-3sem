#include "testSuite.h"
#include "game.h"
#include "board.h"
#include <iostream>

void TestSuite::runTests() {
    testGameLogic();
    testBoardUpdates();
}

void TestSuite::testGameLogic() {
    Game game(3);
    if (game.makeMove(0, 0, 'X')) {
        std::cout << "Test Passed: Move made" << std::endl;
    } else {
        std::cout << "Test Failed: Move not made" << std::endl;
    }

    game.makeMove(0, 1, 'X');
    game.makeMove(0, 2, 'X');
    if (game.checkWin('X')) {
        std::cout << "Test Passed: X wins" << std::endl;
    } else {
        std::cout << "Test Failed: X should win" << std::endl;
    }

    game.reset();
    if (!game.checkWin('X')) {
        std::cout << "Test Passed: Game reset correctly" << std::endl;
    } else {
        std::cout << "Test Failed: Game not reset correctly" << std::endl;
    }
}

void TestSuite::testBoardUpdates() {
    Board board(3);
    board.update(1, 1, 'X');
    if (!board.isCellEmpty(1, 1)) {
        std::cout << "Test Passed: Cell updated correctly" << std::endl;
    } else {
        std::cout << "Test Failed: Cell not updated correctly" << std::endl;
    }
}
