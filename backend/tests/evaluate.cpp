#include "tests.hpp"
#include "Player.hpp"
#include <gtest/gtest.h>

TEST(Capturable, BugPattern) {
    Board board(19);
    Player pl(Cell::Black);
    Player opp(Cell::White);

    board.setBoard(Cell::Black, {10, 10});
    board.setBoard(Cell::White, {11, 10});

    board.setBoard(Cell::White, {10, 11});
    board.setBoard(Cell::Black, {11, 11});


    board.setBoard(Cell::White, {10, 12});
    board.setBoard(Cell::Black, {11, 12});
    board.setBoard(Cell::White, {12, 12});

    board.setBoard(Cell::White, {10, 13});
    board.setBoard(Cell::Black, {11, 13});

    board.setBoard(Cell::Black, {11, 14});
    board.setBoard(Cell::Black, {11, 14});

    board.setBoard(Cell::White, {11, 15});

    EXPECT_TRUE(board.evaluate(pl, opp, {11, 14}) < 40000);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[0][9], board.open_score[2] / 2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[0][12], board.open_score[2] / 2);
}