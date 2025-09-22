#include "tests.hpp"
#include <gtest/gtest.h>

TEST(HeatMap, HeatMap) {
    Board board(19);
    board.setBoard(Cell::Black, {10, 10});
    BoardTestHelper::updateHeatMap_(board, {10, 10});
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[10][10], 3);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[11][11], 2);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[12][12], 1);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[13][13], 0);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[14][14], 0);
    board.setBoard(Cell::Black, {0, 0});
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[18][0], 0);
}