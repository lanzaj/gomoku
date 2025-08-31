#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <iomanip>
# include "Player.hpp"
# include "Cell.hpp"

constexpr int BOARD_SIZE = 19;

struct Coord { int x; int y; };

class Board
{

    private:
        Cell    board_[BOARD_SIZE][BOARD_SIZE];
        int     size_;

        bool    checkWinDirections_(Player const & player, int x0, int y0, int dx, int dy) const;
        void    captureDirection_(Player & player, Player & opponent, int x0, int y0, int dx, int dy);

    public:

        Board(); 
        ~Board();

        Board(Board const & src);
        Board & operator=(Board const & rhs);

        // Getter
        const Cell (&getBoard() const)[BOARD_SIZE][BOARD_SIZE];
        Cell  getCell(int x, int y) const;
        int   getSize() const;

        // Setter
        void setBoard(Cell c, int x, int y);

        // Functions
        bool    checkInBound(int n) const;
        bool    checkWin(Player const & player, int x, int y) const;
        void    capture(Player & player, Player &opponent, int x, int y);
};

std::ostream & operator<<(std::ostream & os, Board const & instance);

#endif