#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
# include <tuple>
# include "Board.hpp"
# include "Player.hpp"
# include "Server.hpp"

class Gomoku
{

private:
    Board   board_;
    Server  server_;
    Player  p_black_;
    Player  p_white_;


    void    init_game_();
    std::tuple<int, int>    playHumanTurn_();
    std::tuple<int, int>    playAiTurn_();
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