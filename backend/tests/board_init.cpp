#include "tests.hpp"
#include <gtest/gtest.h>

TEST(BoardInit, Initialisation) {
    Board board(19);
    EXPECT_EQ(board.getSize(), 19);
    EXPECT_EQ(BoardTestHelper::getBlack(board).color, Cell::Black);
    EXPECT_EQ(BoardTestHelper::getWhite(board).color, Cell::White);
}