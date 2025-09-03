#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <iomanip>
# include "Player.hpp"
# include "Cell.hpp"

constexpr int BOARD_SIZE = 19;

struct Coord { int x; int y; };

struct PlayerState {
    // Alignment chains
    int right[BOARD_SIZE][BOARD_SIZE]{};
    int left[BOARD_SIZE][BOARD_SIZE]{};
    int up[BOARD_SIZE][BOARD_SIZE]{};
    int down[BOARD_SIZE][BOARD_SIZE]{};
    int upRight[BOARD_SIZE][BOARD_SIZE]{};
    int downLeft[BOARD_SIZE][BOARD_SIZE]{};
    int upLeft[BOARD_SIZE][BOARD_SIZE]{};
    int downRight[BOARD_SIZE][BOARD_SIZE]{};

    // Capture evaluation
    int captureThreat[BOARD_SIZE][BOARD_SIZE]{};   // Stones this player can capture
    int captured{};                                // Stones captured by this player

    // Pattern evaluation
    int figures[BOARD_SIZE][BOARD_SIZE]{};         // e.g., open threes/fours
};

class Board
{

    private:
        Cell    board_[BOARD_SIZE][BOARD_SIZE];
        int     size_;

        PlayerState black_;
        PlayerState white_;
        int         heatMap_[BOARD_SIZE][BOARD_SIZE]{}; // 0=ignore, >0=near activity

        bool        checkWinDirections_(Player const & player, int x0, int y0, int dx, int dy) const;
        void        captureDirection_(Player & player, Player & opponent, int x0, int y0, int dx, int dy);
        
        // Getter
        int         getCapture_(Player const & player) const;

        PlayerState &getPlayerState_(Player const & player);
        const PlayerState &getPlayerState_(Player const & player) const; // const overload
        
        // Setter
        void        incrementCapture_(Player const & player);

    public:

        Board(); 
        ~Board() = default;

        Board(Board const & src) = default;
        Board & operator=(Board const & rhs) = default;

        // Getter
        const Cell (&getBoard() const)[BOARD_SIZE][BOARD_SIZE];
        Cell        getCell(int x, int y) const;
        int         getSize() const;

        // Setter
        void    setBoard(Cell c, int x, int y);

        // Functions
        bool    checkInBound(int n) const;
        bool    checkWin(Player const & player, int x, int y) const;
        void    capture(Player & player, Player &opponent, int x, int y);
};

std::ostream & operator<<(std::ostream & os, Board const & instance);

#endif