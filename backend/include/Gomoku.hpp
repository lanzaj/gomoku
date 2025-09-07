#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
# include <stack>
# include "Board.hpp"
# include "Player.hpp"
# include "Server.hpp"

struct MoveEval { long long score; Coord bestMove;};

class Gomoku
{
    private:
        std::stack<Board> board_;
        Server  server_;
        Player  p_black_;
        Player  p_white_;
        std::string mode_;


        void    init_game_();
        bool    playTurn_(Player &player, Player &opponent);
        Coord   playHumanTurn_();
        Coord   playAiTurn_(Player const & player, Player const & opponent);
        
        void    play_(Coord coord, Player const & player,  Player const & opponent);
        void    undo_();
    public:
        Gomoku();
        ~Gomoku();

        // Getter
        const Board     &getBoard(void) const;

        // Functions
        void            play(void);
        MoveEval        minimax(int depth, long long alpha, long long beta, bool maximizing, Player const & player, Player const & opponent, Coord lastMove);

        // Exception
        class AiException : public std::runtime_error {
            public:
                explicit AiException(const std::string& msg)
                    : std::runtime_error("Ai error: " + msg) {}
        };
};

// operator <<
std::ostream & operator<<(std::ostream & os, Gomoku const & instance);

#endif