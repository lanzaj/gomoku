#include "Gomoku.hpp"

Gomoku::Gomoku() : p_white_(Player('O')), p_black_(Player('X')) {}
Gomoku::~Gomoku() {}

// Getter
const Board &Gomoku::getBoard(void) const
{
    return board_;
}

// operator <<
std::ostream & operator<<(std::ostream & os, Gomoku const & instance)
{
    os << instance.getBoard();
    return os;
}

void    Gomoku::play(void) {
    while (!(playTurn_(p_white_, p_black_)
        || playTurn_(p_black_, p_white_))) {
    }
}


std::tuple<int, int> Gomoku::getCoord_(void) {
    int x = -1, y = -1;
    while (!board_.checkInBound(x) || !board_.checkInBound(y) || board_.getCell(x, y) != '.') {
        std::cout << *this << "Enter x coord: " << std::endl;
        std::cin >> x;
        std::cout << *this << "Enter y coord: " << std::endl;
        std::cin >> y;
    }
    return std::make_tuple(x, y);
}

bool    Gomoku::playTurn_(Player player, Player opponent) {
    int x, y;
    std::tie(x, y) = getCoord_();

    if (y < 0 || x < 0 || y > board_.getSize() || x > board_.getSize()) {
        throw std::out_of_range("Coordinates are out of bounds");
    }
    if (board_.getCell(x, y) != '.') {
        throw std::runtime_error("Cell is already occupied");
    }
    board_.setBoard(player.getChar(), x, y);

    board_.capture(player, opponent, x, y);
    if (board_.checkWin(player, x, y))
        return true;

    return false;
}

