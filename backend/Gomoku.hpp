#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
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
        std::string mode_;


        void    init_game_();
        bool    playTurn_(Player &player, Player &opponent);
        Coord   playHumanTurn_();
        Coord   playAiTurn_();

    public:
        Gomoku();
        ~Gomoku();

        // Getter
        const Board     &getBoard(void) const;

        // Functions
        void            play(void);
};

// operator <<
std::ostream & operator<<(std::ostream & os, Gomoku const & instance);

#endif