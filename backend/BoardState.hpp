#ifndef BOARDSTATE_HPP
# define BOARDSTATE_HPP

# include <iostream>
# include "Board.hpp"

class BoardState
{
private:
    Cell board_[BOARD_SIZE][BOARD_SIZE]{};

    // Alignment chains
    int right_[BOARD_SIZE][BOARD_SIZE]{};     
    int left_[BOARD_SIZE][BOARD_SIZE]{};      
    int up_[BOARD_SIZE][BOARD_SIZE]{};
    int down_[BOARD_SIZE][BOARD_SIZE]{};
    int upRight_[BOARD_SIZE][BOARD_SIZE]{};
    int downLeft_[BOARD_SIZE][BOARD_SIZE]{};
    int upLeft_[BOARD_SIZE][BOARD_SIZE]{};
    int downRight_[BOARD_SIZE][BOARD_SIZE]{};

    // Capture evaluation
    int captureThreat_[BOARD_SIZE][BOARD_SIZE]{}; // Stones player can capture
    int capturable_[BOARD_SIZE][BOARD_SIZE]{};    // Stones opponent can capture
    int captured_[2]{};                           // Stones captured per player

    // Pattern evaluation
    int figures_[BOARD_SIZE][BOARD_SIZE]{}; // e.g., open threes/fours
    int heatMap_[BOARD_SIZE][BOARD_SIZE]{}; // 0=ignore, >0=near activity

    int depth_{};

public:
    // Use compiler defaults (rule of 0)
    BoardState() = default;
    BoardState(const BoardState&) = default;
    BoardState& operator=(const BoardState&) = default;
    ~BoardState() = default;

    // Getters
    const Cell (&getBoard() const)[BOARD_SIZE][BOARD_SIZE] { return board_; }
    int getDepth() const { return depth_; }
};

std::ostream & operator<<(std::ostream & os, BoardState const & instance);

#endif