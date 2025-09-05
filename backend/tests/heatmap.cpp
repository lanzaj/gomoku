#include "tests.hpp"
#include <gtest/gtest.h>

TEST(HeatMap, HeatMap) {
    Board board;
    board.setBoard(Cell::Black, {10, 10});
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[10][10], 4);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[11][11], 3);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[12][12], 2);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[13][13], 1);
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[14][14], 0);
    board.setBoard(Cell::Black, {0, 0});
    EXPECT_EQ(BoardTestHelper::getHeatMap(board)[18][0], 0);
}