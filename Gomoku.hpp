#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
# include "Board.hpp"
# include "Player.hpp"

class Gomoku
{

private:
    Board   board_;
    Player  p_white_;
    Player  p_black_;

public:
    Gomoku();
    ~Gomoku();

    // Getter
    const Board &getBoard(void) const;

    void    play(void);
};

std::ostream & operator<<(std::ostream & os, Gomoku const & instance);

#endif