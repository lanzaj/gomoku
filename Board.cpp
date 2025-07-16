#include "Board.hpp"

Board::Board(/* args */)
{
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            board[i][j] = '.';
        }
    }
}

Board::~Board()
{
}

// Board::Board(Board const & src)
// {
// }

// Board & Board::operator=(Board const & rhs)
// {
// }

const char (&Board::getBoard() const)[19][19] {
    return board;
}

std::ostream & operator<<(std::ostream & os, Board const & instance)
{
    const char (&grid)[19][19] = instance.getBoard();

    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            os << grid[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}