#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
# include <tuple>
# include "Board.hpp"
# include "Player.hpp"

class Gomoku
{

private:
    Board   board_;
    Player  p_white_;
    Player  p_black_;

    bool    playTurn_(Player player, Player opponent);
    std::tuple<int, int> getCoord_(void);

public:
    Gomoku();
    ~Gomoku();

    // Getter
    const Board &getBoard(void) const;

    // Functions
    void    play(void);
};

// operator <<
std::ostream & operator<<(std::ostream & os, Gomoku const & instance);

#endif