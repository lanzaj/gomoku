#ifndef MINIMAX_HPP
# define MINIMAX_HPP

# include <iostream>
# include "Board.hpp"

class Minimax
{

private:


    // 1)   no space for 5      0
    //      closed two          5
    //      open two            20          (10 each side)
    //      closed three        100
    //      open three          600         (300 each side)
    //      closed four         5,000
    //      open four           30,000      (15,000 each side)
    //      five in a row       10,000,000

    // 2) x% less points if some capturable in 1 
    //      -> takes potential capture into account

    // 3) x% less points if only space for 5

    // 4) figures:
    //      open three and four (max 1 open three)


    int     right[BOARD_SIZE][BOARD_SIZE];
    int     left[BOARD_SIZE][BOARD_SIZE];
    int     up[BOARD_SIZE][BOARD_SIZE];
    int     down[BOARD_SIZE][BOARD_SIZE];
    int     up_right[BOARD_SIZE][BOARD_SIZE];
    int     down_left[BOARD_SIZE][BOARD_SIZE];
    int     up_left[BOARD_SIZE][BOARD_SIZE];
    int     down_right[BOARD_SIZE][BOARD_SIZE];

    int     capture_threat[BOARD_SIZE][BOARD_SIZE]; // (in 1)

    //int     capturable[BOARD_SIZE][BOARD_SIZE]; // (in 1)

    int     figures[BOARD_SIZE][BOARD_SIZE]; // 1 open threes max and fours

    // 5) Take past player action into account to identify patterns of capture and weigh board accordingly
    //    -> the more capture left to win is high, the higher a capture threat is valued:
    //      0                           0
    //          0 with 1 capture threat     10
    //          0 with 2 capture threat     15
    //          0 with 3 capture threat     30
    //          0 with 4 capture threat     90
    //          0 with 5 capture threat     360
    //      1                           50
    //          1 with 1 capture threat     50
    //          1 with 2 capture threat     75
    //          1 with 3 capture threat     150
    //          1 with 4 capture threat     450
    //      2                           150
    //          2 with 1 capture threat     225
    //          2 with 2 capture threat     450
    //          2 with 3 capture threat     1,300
    //      3                           750
    //          3 with 1 capture threat     1,000
    //          3 with 2 capture threat     2,000
    //      4                           5,000
    //      4 with 1 capture threat     40,000
    //      5                           10,000,000
    int     capture_left_to_win;
    //int     capture_left_to_loose;



    int     threatsToWin;

    // Search strategy
    // Fours and captures > threes > twos
    int     heat_map[BOARD_SIZE][BOARD_SIZE]; // to not evaluate a stone if no stone is near (3 of distance)

    // depth = 10

public:
    Minimax();
    ~Minimax();

    Minimax(Minimax const & src);
    Minimax & operator=(Minimax const & rhs);
};

std::ostream & operator<<(std::ostream & os, Minimax const & instance);

#endif