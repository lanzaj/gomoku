#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# include <iostream>
# include <stack>
# include <chrono>
# include "Board.hpp"
# include "Player.hpp"
# include "Server.hpp"

struct MoveEval { long long score; Coord bestMove;};
struct AiMoveResult {
    Coord move;
    long long timeMs; // execution time in ms
};

class Gomoku
{
    private:
        std::stack<Board> board_;
        Server  server_;
        Player  p_black_;
        Player  p_white_;
        std::string mode_ = "";
        std::string start_option_ = "";
        bool    first_move_centered_{};
        bool    pro_flag_5x5_{};
        int     size_{};
        int     n_turn{};


        void    init_rule_();
        void    init_game_();
        bool    playTurn_(Player &player, Player &opponent);
        Coord   playHumanTurn_(Player const & player, Player const & opponent, Board & board);
        AiMoveResult   playAiTurn_(Player const & player, Player const & opponent);
        
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
};

// operator <<
std::ostream & operator<<(std::ostream & os, Gomoku const & instance);

#endif