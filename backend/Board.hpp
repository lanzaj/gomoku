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

        bool    checkWinDirections_(Player player, int x0, int y0, int dx, int dy);
        void    captureDirection_(Player player, Player opponent, int x0, int y0, int dx, int dy);

    public:
        Board();
        ~Board();

        Board(Board const & src);
        Board & operator=(Board const & rhs);

        // Getter
        const char (&getBoard() const)[BOARD_SIZE][BOARD_SIZE];
        char  getCell(int x, int y);
        int   getSize() const;

        // Setter
        void setBoard(char c, int x, int y);

        // Functions
        bool    checkInBound(int n);
        bool    checkWin(Player player, int x, int y);
        void    capture(Player player, Player opponent, int x, int y);
};

std::ostream & operator<<(std::ostream & os, Board const & instance);

#endif