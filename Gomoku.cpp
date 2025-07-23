#include "Gomoku.hpp"

Gomoku::Gomoku() : p_white_(Player('O')), p_black_(Player('X')) {}

Gomoku::~Gomoku() {}

void    Gomoku::play(void) {
    while (true) {
        board_.playTurn(p_white_);
        board_.playTurn(p_black_);
    }
}

const Board &Gomoku::getBoard(void) const
{
    return board_;
}

std::ostream & operator<<(std::ostream & os, Gomoku const & instance)
{
    os << instance.getBoard();
    return os;
}