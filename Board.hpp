#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <iomanip>
# include "Player.hpp"
# define BOARD_SIZE 19

class Board
{

private:
    char    board_[BOARD_SIZE][BOARD_SIZE];
    int     size_;

public:
    Board();
    ~Board();

    Board(Board const & src);
    Board & operator=(Board const & rhs);

    // Getter
    const char (&getBoard() const)[BOARD_SIZE][BOARD_SIZE];

    void    playTurn(Player player);
    bool    checkOutOfBound(int n);
    void    checkWinDir(Player player, int x0, int y0, int dx, int dy);
    void    checkWin(Player player, int x, int y);
};

std::ostream & operator<<(std::ostream & os, Board const & instance);

#endif