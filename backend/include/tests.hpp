#ifndef TESTS_HPP
# define TESTS_HPP

# include "Board.hpp"

// TEST black_ and white_
class BoardTestHelper {
public:
    static PlayerState& getBlack(Board& b) { return b.black_; }
    static PlayerState& getWhite(Board& b) { return b.white_; }
    static const int (&getHeatMap(Board& b))[BOARD_SIZE][BOARD_SIZE] { return b.heatMap_; }
    static void updateHeatMap_(Board& b, Coord coord) { b.updateHeatMap_(coord); }

};

#endif