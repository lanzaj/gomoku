#include "tests.hpp"
#include <gtest/gtest.h>

TEST(CheckAlignment2s, OpenTwo) {
    Board board;
    board.setBoard(Cell::Black, {10, 0});
    board.setBoard(Cell::Black, {11, 0});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[0][9], board.open_score[2] / 2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[0][12], board.open_score[2] / 2);
}

TEST(CheckAlignment2s, ClosedRightWrongColorBefore) {
    Board board;
    board.setBoard(Cell::White, {9, 1});
    board.setBoard(Cell::Black, {10, 1});
    board.setBoard(Cell::Black, {11, 1});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][9], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][12], board.closed_score[2]);
}

TEST(CheckAlignment2s, ClosedRightWrongColorAfter) {
    Board board;
    board.setBoard(Cell::Black, {10, 2});
    board.setBoard(Cell::Black, {11, 2});
    board.setBoard(Cell::White, {9, 2});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][9], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][12], board.closed_score[2]);
}

TEST(CheckAlignment2s, ClosedLeftWrongColorBefore) {
    Board board;
    board.setBoard(Cell::White, {12, 1});
    board.setBoard(Cell::Black, {10, 1});
    board.setBoard(Cell::Black, {11, 1});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][9], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][12], 0);
}

TEST(CheckAlignment2s, ClosedLeftWrongColorAfter) {
    Board board;
    board.setBoard(Cell::Black, {10, 2});
    board.setBoard(Cell::Black, {11, 2});
    board.setBoard(Cell::White, {12, 2});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][9], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][12], 0);
}

TEST(CheckAlignment2s, ClosedLeftBorder) {
    Board board;
    board.setBoard(Cell::Black, {0, 2});
    board.setBoard(Cell::Black, {1, 2});
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][2], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][0], 0);
}

TEST(CheckAlignment2s, ClosedRigthBorder) {
    Board board;
    board.setBoard(Cell::Black, {18, 2});
    board.setBoard(Cell::Black, {17, 2});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][16], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][18], 0);
}

TEST(CheckAlignment2s, Two2Filled) {
    Board board;
    board.setBoard(Cell::White, {12, 1});
    board.setBoard(Cell::Black, {10, 1});
    board.setBoard(Cell::Black, {11, 1});
    board.setBoard(Cell::Black, {13, 1});
    board.setBoard(Cell::Black, {14, 1});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][9], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][15], board.closed_score[2]);
    board.setBoard(Cell::Black, {10, 2});
    board.setBoard(Cell::Black, {11, 2});
    board.setBoard(Cell::White, {12, 2});
    board.setBoard(Cell::Black, {13, 2});
    board.setBoard(Cell::Black, {14, 2});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][9], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][15], board.closed_score[2]);
    board.setBoard(Cell::Black, {10, 12});
    board.setBoard(Cell::Black, {11, 12});
    board.setBoard(Cell::Black, {13, 12});
    board.setBoard(Cell::Black, {14, 12});
    board.setBoard(Cell::White, {12, 12});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[12][9], board.closed_score[2]);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[12][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[12][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[12][15], board.closed_score[2]);
}

TEST(CheckAlignment2s, Two2Empty) {
    Board board;
    board.setBoard(Cell::Black, {10, 1});
    board.setBoard(Cell::Black, {11, 1});
    board.setBoard(Cell::Black, {13, 1});
    board.setBoard(Cell::Black, {14, 1});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][9], board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][12],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][12],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][15], board.open_score[2]/2);
    board.setBoard(Cell::Black, {14, 2});
    board.setBoard(Cell::Black, {13, 2});
    board.setBoard(Cell::Black, {11, 2});
    board.setBoard(Cell::Black, {10, 2});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][9], board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[2][12],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][12],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[2][15],  board.open_score[2]/2);
    board.setBoard(Cell::Black, {10, 12});
    board.setBoard(Cell::Black, {11, 12});
    board.setBoard(Cell::Black, {14, 12});
    board.setBoard(Cell::Black, {13, 12});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[12][9],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[12][12],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[12][12],  board.open_score[2]/2);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[12][15],  board.open_score[2]/2);
}

TEST(CheckAlignment2s, NotEnoughSpaceEasy) {
    Board board;
    board.setBoard(Cell::White, {8, 1});
    board.setBoard(Cell::White, {13, 1});
    board.setBoard(Cell::Black, {10, 1});
    board.setBoard(Cell::Black, {11, 1});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][9], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][10], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][11],  0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][9], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][10], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][11],  0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][12], 0);
}

TEST(CheckAlignment2s, NotEnoughSpaceHardAfter) {
    Board board;
    board.setBoard(Cell::Black, {10, 1});
    board.setBoard(Cell::Black, {11, 1});
    board.setBoard(Cell::White, {13, 1});
    board.setBoard(Cell::White, {8, 1});
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][9], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][10], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][11],  0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).right[1][12], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][9], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][10], 0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][11],  0);
    EXPECT_EQ(BoardTestHelper::getBlack(board).left[1][12], 0);
}