#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
# include "Board.hpp"

class Gomoku
{

private:
    Board   board;

public:
    Gomoku(/* args */);
    ~Gomoku();

    // Gomoku(Gomoku const & src);
    // Gomoku & operator=(Gomoku const & rhs);

    // Getter
    const Board &getBoard(void) const;
};

std::ostream & operator<<(std::ostream & os, Gomoku const & instance);

#endif