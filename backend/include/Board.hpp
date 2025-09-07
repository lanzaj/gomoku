#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <iomanip>
# include "Player.hpp"
# include "Cell.hpp"
# include <climits>
# include <cstring>
# include <vector>
# include <algorithm>

constexpr int INF = INT_MAX;
constexpr int WIN = INT_MAX - 1;
constexpr int LOOSE = -INT_MAX + 1;
constexpr int BOARD_SIZE = 19;

struct Coord { int x; int y; };
struct CoordValue { int x, y; long long value; };
struct Direction { int dx; int dy; };
struct DirMapping {
    int (*state)[BOARD_SIZE];
    Direction dir;
};

struct PlayerState {
    Cell    color;

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
    bool closed5 = false;
};

class Board
{

    private:
        Cell    board_[BOARD_SIZE][BOARD_SIZE];
        int     size_;

        PlayerState black_;
        PlayerState white_;

        int         heatMap_[BOARD_SIZE][BOARD_SIZE]{}; // 0=ignore, >0=near activity

        bool        isForbiddenDoubleThree(Coord coord, Player const & player) const;
        bool        isCapturable(int x, int y, Cell const & color) const;
        bool        checkWinDirection_(Player const & player, Coord coord, Direction dir) const;
        void        captureDirection_(Player const & player, Player const & opponent, Coord coord, Direction dir);
        void        closeAlignmentDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir);
        void        updateAlignmentDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir);
        void        updateAlignment_(Coord coord);
        void        updateCapturableDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir);
        void        updateCapturable_(Coord coord);
        void        updateHeatMap_(Coord coord);
        long long   evaluateAlignments_(PlayerState const & state);

        // Getter
        int                 getCapture_(Player const & player) const;
        
        // Setter
        void        incrementCapture_(Player const & player);

        friend class BoardTestHelper;

    public:
        // Directions
        static constexpr Direction RIGHT      {1, 0};
        static constexpr Direction LEFT       {-1, 0};
        static constexpr Direction UP         {0, -1};
        static constexpr Direction DOWN       {0, 1};
        static constexpr Direction UP_RIGHT   {1, -1};
        static constexpr Direction DOWN_LEFT  {-1, 1};
        static constexpr Direction UP_LEFT    {-1, -1};
        static constexpr Direction DOWN_RIGHT {1, 1};

        static constexpr Direction DIRECTIONS[8] = {
            RIGHT, LEFT, UP, DOWN, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT
        };

        // Scores depending of the length of the alinment
        static constexpr int   open_score[6] = {0, 2, 20, 600, 30000, 3024000};
        static constexpr int closed_score[6] = {0, 1, 10,  200,  10000, 1024000};

        static constexpr int capture_score[6] = {0, 200, 1000, 5000, 40000, 5024000};

        static constexpr int beam_search[10] = {50, 20, 10, 10, 10, 3, 3, 3, 3, 3};

        static constexpr int DEFENSE_MODIFIER = 2;

        Board(); 
        ~Board() = default;

        Board(Board const & src) = default;
        Board & operator=(Board const & rhs) = default;

        // Getter
        const Cell (&getBoard() const)[BOARD_SIZE][BOARD_SIZE];
        Cell        getCell(Coord coord) const;
        int         getSize() const;
        Cell        reverse(Cell const & c) const;

        PlayerState         &getPlayerState_(Cell const & corlor);
        PlayerState         &getPlayerState_(Player const & player);
        const PlayerState   &getPlayerState_(Player const & player) const;

        // Setter
        void    setBoard(Cell color, Coord coord);

        // Functions
        bool    checkInBound(int n) const;
        bool    checkInBound(int a, int b) const;
        bool    checkWin(Player const & player, Coord coord) const;
        void    capture(Player const & player, Player const & opponent, Coord coord);
        
        long long           evaluate(Player const & player, Player const & opponent, Coord LastMove);
        bool                isGameOver(Player const & player, Player const & opponent, Coord last);
        std::vector<Coord>  generateMoves(int depth, Player const & player);
};

std::ostream & operator<<(std::ostream & os, Board const & instance);
std::ostream & operator<<(std::ostream & os, PlayerState const & instance);
std::ostream & operator<<(std::ostream & os, int board[BOARD_SIZE][BOARD_SIZE]);

#endif