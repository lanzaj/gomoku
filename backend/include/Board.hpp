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

constexpr long long INF = LLONG_MAX;
constexpr int WIN = INT_MAX;
constexpr int LOOSE = -INT_MAX;
constexpr int BOARD_SIZE = 19;

struct Coord {
    int x;
    int y;

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
};

struct CoordValue { int x, y; long long value; };
struct Direction { int dx; int dy; };
struct DirMapping {
    int (*state)[BOARD_SIZE];
    Direction dir;
};

struct PlayerState {
    Cell    color;

    // Alignment chains
    // Close[2] adverse = capture threat
    int right[BOARD_SIZE][BOARD_SIZE]{};
    int left[BOARD_SIZE][BOARD_SIZE]{};
    int up[BOARD_SIZE][BOARD_SIZE]{};
    int down[BOARD_SIZE][BOARD_SIZE]{};
    int upRight[BOARD_SIZE][BOARD_SIZE]{};
    int downLeft[BOARD_SIZE][BOARD_SIZE]{};
    int upLeft[BOARD_SIZE][BOARD_SIZE]{};
    int downRight[BOARD_SIZE][BOARD_SIZE]{};

    // Capture evaluation
    int capturable[BOARD_SIZE][BOARD_SIZE]{};   // Stones this player can capture
    int captured{};                                // Stones captured by this player

    // Pattern evaluation
    int figures[BOARD_SIZE][BOARD_SIZE]{};         // e.g., open threes/fours
    int broken[BOARD_SIZE][BOARD_SIZE]{};

    bool align5{};
    Coord align5Coord = {-1, -1};
};

class Board
{

    private:
        Cell    board_[BOARD_SIZE][BOARD_SIZE];
        int     size_;

        PlayerState black_;
        PlayerState white_;

        int         heatMap_[BOARD_SIZE][BOARD_SIZE]{}; // 0=ignore, >0=near activity

        bool        isCapturable(int x, int y, Cell const & color) const;
        std::vector<Coord> getCapturingMoves(const std::vector<Coord>& targets, Cell color) const;
        bool        check5AlignmentDirection_(Player const & player, Coord coord, Direction dir) const;
        bool        checkCaptureWinDirection_(Player const & player, Coord coord, Coord capture, Direction dir) const;
        bool        checkWinDirection_(Player const & player, Coord coord, Direction dir) const;
        void        captureDirection_(Player const & player, Player const & opponent, Coord coord, Direction dir);
        void        closeAlignmentDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir);
        void        updateBrokenDirection_(PlayerState & state, Coord coord);
        void        updateBroken_(Coord coord);
        void        updateFiguresDirection_(PlayerState & state, Coord coord);
        void        updateAlignmentDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir);
        void        updateFigures_(Coord coord);
        void        updateAlignment_(Coord coord);
        void        updateCapturableDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir);
        void        updateCapturable_(Coord coord);
        void        updateHeatMap_(Coord coord);
        long long   evaluateAlignments_(PlayerState const & state, PlayerState const & opp_state);

        // Getter
        int         getCapture_(Player const & player) const;
        
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
        // WARNING open_score[2] // 2 should be different than closed_score[2]
        static constexpr int   open_score[6] = {0, 5, 50, 1000, 50000,   3024000};
        static constexpr int closed_score[6] = {0, 1, 10,  200,  15000, 1024000};

        static constexpr int capture_score[6] = {0, 200, 500, 2000, 5000, 5024000};
        static constexpr int capture_threat[6] = {500, 1000, 2000, 10000, 50000,  5024000};

        static constexpr int beam_search[10] = {30, 10, 5, 5, 5, 4, 3, 3, 3, 2};

        static constexpr int DEFENSE_MODIFIER = 2;

        Board(int size); 
        ~Board() = default;

        Board(Board const & src) = default;
        Board & operator=(Board const & rhs) = default;

        // Getter
        const Cell (&getBoard() const)[BOARD_SIZE][BOARD_SIZE];
        Cell        getCell(Coord coord) const;
        int         getSize() const;
        Cell        reverse(Cell const & c) const;


        PlayerState         &getPlayerState_(Cell const & color);
        PlayerState         &getPlayerState_(Player const & player);
        const PlayerState   &getPlayerState_(Player const & player) const;

        // Setter
        void    setBoardWithCapture(Cell color, Coord coord, Player const & player, Player const & opponent);
        void    setBoard(Cell color, Coord coord);
        void    setSize(int size);

        // Functions
        bool    isForbiddenDoubleThree(Coord coord, Cell const & color) const;
        bool    checkInBound(int n) const;
        bool    checkInBound(int a, int b) const;
        bool    checkCaptureWin(Player const & player, Coord coord, Coord capture) const;
        bool    checkWin(Player const & player, Coord coord) const;
        void    capture(Player const & player, Player const & opponent, Coord coord);
        
        long long           evaluate(Player const & player, Player const & opponent, Coord LastMove);
        bool                isGameOver(Player const & player, Player const & opponent, Coord last);
        std::vector<Coord>  generateMoves(Player const & player, Player const & opponent);
        std::vector<Coord>  getCapturingMovesToWin(Player const & player);

        // Exception
        class AiException : public std::runtime_error {
            public:
                explicit AiException(const std::string& msg)
                    : std::runtime_error("Ai error: " + msg) {}
        };
    };

std::ostream & operator<<(std::ostream & os, Board const & instance);
std::ostream & operator<<(std::ostream & os, PlayerState const & instance);
std::ostream & operator<<(std::ostream & os, int board[BOARD_SIZE][BOARD_SIZE]);

#endif