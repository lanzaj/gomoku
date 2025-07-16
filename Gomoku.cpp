#include "Gomoku.hpp"

Gomoku::Gomoku(/* args */)
{
}

Gomoku::~Gomoku()
{
}

// Gomoku::Gomoku(Gomoku const & src)
// {
// }

// Gomoku & Gomoku::operator=(Gomoku const & rhs)
// {
// }


const Board &Gomoku::getBoard(void) const
{
    return board;
}