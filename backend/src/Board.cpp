#include "Board.hpp"

Board::Board(int size) : size_(size) 
{
    for (int i = 0; i < size_; ++i)
        for (int j = 0; j < size_; ++j)
            board_[i][j] = Cell::Empty;
    black_.color = Cell::Black;
    white_.color = Cell::White;
}

// Getter
const Cell (&Board::getBoard() const)[BOARD_SIZE][BOARD_SIZE] {
    return board_;
}

Cell  Board::getCell(Coord coord) const {
    if (!checkInBound(coord.x, coord.y)) {
        return Cell::Empty;
    }
    return board_[coord.y][coord.x];
}

int   Board::getSize() const {
    return size_;
}


Cell  Board::reverse(Cell const & c) const {
    if (c == Cell::Black)
        return Cell::White;
    if (c == Cell::White)
        return Cell::Black;
    return Cell::Empty;
}

// Setter
void Board::setBoardWithCapture(Cell color, Coord coord, Player const & player, Player const & opponent) {
    board_[coord.y][coord.x] = color;
    capture(player, opponent, coord);
    updateCapturable_(coord);
    updateAlignment_(coord);
}

void Board::setBoard(Cell color, Coord coord) {
    board_[coord.y][coord.x] = color;
    updateCapturable_(coord);
    updateAlignment_(coord);
    if (color == Cell::Empty) {
        for (auto dir : DIRECTIONS) {
            int x = coord.x + dir.dx;
            int y = coord.y + dir.dy;
            if (checkInBound(x, y))
                updateAlignment_({x, y});
        }
    }
}

void Board::setSize(int size) {
    size_ = size;
}

// Operator <<
std::ostream & operator<<(std::ostream & os, Board const & instance)
{
    const Cell (&grid)[BOARD_SIZE][BOARD_SIZE] = instance.getBoard();

    os << "  ";
    for (int x = 0; x < instance.getSize(); ++x)
        os << std::setw(2) << x;
    os << std::endl;
    for (int y = 0; y < instance.getSize(); ++y) {
        os << std::setw(2) << y << " ";
        for (int x = 0; x < instance.getSize(); ++x) {
            os << grid[y][x] << " ";
        }
        os << std::endl;
    }
    return os;
}

// Operator <<
std::ostream & operator<<(std::ostream & os, PlayerState const & instance)
{
    int size = sizeof(instance.up) / sizeof(instance.up[0]);
    os << "right                                       left" << std::endl;
    os << "  ";
    for (int x = 0; x < size; ++x)
        os << std::setw(2) << x;
    os << "   ";
    for (int x = 0; x < size; ++x)
        os << std::setw(2) << x;
    os << std::endl;
    for (int y = 0; y < size; ++y) {
        // os << std::setw(2) << y << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.forbiddenThree[y][x] << " ";
        // }
        // os << std::setw(2) << y << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.capturable[y][x] << " ";
        // }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.right[y][x] << " ";
        // }
        os << " ";
        for (int x = 0; x < size; ++x) {
            os << instance.capturable[y][x] << " ";
        }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.broken[y][x] << " ";
        // }
        os << " ";
        for (int x = 0; x < size; ++x) {
            os << instance.figures[y][x] << " ";
        }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.up[y][x] << " ";
        // }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.down[y][x] << " ";
        // }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.upLeft[y][x] << " ";
        // }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.downLeft[y][x] << " ";
        // }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.upRight[y][x] << " ";
        // }
        // os << " ";
        // for (int x = 0; x < size; ++x) {
        //     os << instance.downRight[y][x] << " ";
        // }
        os << std::endl;
    }

    return os;
}

std::ostream & operator<<(std::ostream & os, int board[BOARD_SIZE][BOARD_SIZE])
{
    os << "  ";
    for (int x = 0; x < BOARD_SIZE; ++x)
        os << std::setw(2) << x;
    os << std::endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        os << std::setw(2) << y << " ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            os << board[y][x] << " ";
        }
        os << std::endl;
    }

    return os;
}

// Private Getter
int Board::getCapture_(Player const & player) const {
    return getPlayerState_(player).captured;
}

PlayerState& Board::getPlayerState_(Cell const & color) {
    return (color == Cell::Black) ? black_ : white_;
}

PlayerState& Board::getPlayerState_(const Player& player) {
    return (player.getColor() == Cell::Black) ? black_ : white_;
}

const PlayerState& Board::getPlayerState_(const Player& player) const {
    return (player.getColor() == Cell::Black) ? black_ : white_;
}

// Private Setter
void    Board::incrementCapture_(Player const & player) {
    getPlayerState_(player).captured += 1;
}

// Functions
bool    Board::checkInBound(int n) const {
    return (n >= 0 && n < size_);
}

bool    Board::checkInBound(int a, int b) const {
    return ((a >= 0 && a < size_) && (b >= 0 && b < size_));
}

bool Board::isCapturable(int x, int y, Cell const & color) const {
    Cell me = (color == Cell::Black) ? Cell::Black : Cell::White;
    Cell opp = (color == Cell::Black) ? Cell::White : Cell::Black;

    // Directions: horizontal, vertical, both diagonals
    static const int directions[8][2] = {
        {1, 0}, {-1, 0},   // horizontal
        {0, 1}, {0, -1},   // vertical
        {1, 1}, {-1, -1},  // diagonal ↘ ↖
        {1, -1}, {-1, 1}   // diagonal ↗ ↙
    };

    for (auto &d : directions) {
        int dx = d[0], dy = d[1];

        // --- Check forward pattern: O x X _
        int x1 = x - dx,     y1 = y - dy;     // opponent stone
        int x2 = x,          y2 = y;         // first player stone
        int x3 = x + dx,     y3 = y + dy;    // second player stone
        int x4 = x + 2*dx,   y4 = y + 2*dy;  // empty cell

        if (checkInBound(x1, y1) && checkInBound(x2, y2) && checkInBound(x3, y3) && checkInBound(x4, y4)) {
            if (board_[y1][x1] == opp &&
                board_[y2][x2] == me &&
                board_[y3][x3] == me &&
                board_[y4][x4] == Cell::Empty &&
                !isForbiddenDoubleThree({x4, y4}, opp)
            ) {
                return true;
            }
        }

        // --- Check backward pattern: O X x _
        x1 = x - 2*dx; y1 = y - 2*dy; // empty cell
        x2 = x - dx;   y2 = y - dy;   // first player stone
        x3 = x;        y3 = y;        // second player stone
        x4 = x + dx;   y4 = y + dy;   // opponent stone

        if (checkInBound(x1, y1) && checkInBound(x2, y2) && checkInBound(x3, y3) && checkInBound(x4, y4)) {
            if (board_[y1][x1] == opp &&
                board_[y2][x2] == me &&
                board_[y3][x3] == me &&
                board_[y4][x4] == Cell::Empty &&
                    !isForbiddenDoubleThree({x4, y4}, opp)) {
                return true;
            }
        }
    }

    return false;
}

std::vector<Coord> Board::getCapturingMoves(const std::vector<Coord>& targets,
                                            Cell color) const
{
    std::vector<Coord> capturingMoves;

    // Directions to check
    static const int directions[8][2] = {
        {1, 0}, {-1, 0},   // horizontal
        {0, 1}, {0, -1},   // vertical
        {1, 1}, {-1, -1},  // diagonal ↘ ↖
        {1, -1}, {-1, 1}   // diagonal ↗ ↙
    };

    Cell me  = (color == Cell::Black) ? Cell::Black : Cell::White;
    Cell opp = (color == Cell::Black) ? Cell::White : Cell::Black;

    for (auto& c : targets) {
        int x = c.x;
        int y = c.y;
        if (x == -1 || y == -1)
            continue;
        for (auto& d : directions) {
            int dx = d[0], dy = d[1];

            // Case: O x X _
            int x1 = x - dx,     y1 = y - dy;
            int x2 = x,          y2 = y;
            int x3 = x + dx,     y3 = y + dy;
            int x4 = x + 2*dx,   y4 = y + 2*dy;

            if (checkInBound(x1,y1) && checkInBound(x2,y2) &&
                checkInBound(x3,y3) && checkInBound(x4,y4))
            {
                if (board_[y1][x1] == opp &&
                    board_[y2][x2] == me &&
                    board_[y3][x3] == me &&
                    board_[y4][x4] == Cell::Empty &&
                    !isForbiddenDoubleThree({x4, y4}, opp))
                {
                    capturingMoves.push_back(Coord{x4,y4});
                }
            }

            // Case: O X x _
            x1 = x - 2*dx; y1 = y - 2*dy;
            x2 = x - dx;   y2 = y - dy;
            x3 = x;        y3 = y;
            x4 = x + dx;   y4 = y + dy;

            if (checkInBound(x1,y1) && checkInBound(x2,y2) &&
                checkInBound(x3,y3) && checkInBound(x4,y4))
            {
                if (board_[y1][x1] == opp &&
                    board_[y2][x2] == me &&
                    board_[y3][x3] == me &&
                    board_[y4][x4] == Cell::Empty &&
                    !isForbiddenDoubleThree({x4, y4}, opp))
                {
                    capturingMoves.push_back(Coord{x4,y4});
                }
            }
        }
    }
    return capturingMoves;
}

bool    Board::checkWinDirection_(Player const & player, Coord coord, Direction dir) const {
    int x0 = coord.x, y0 = coord.y, dx = dir.dx, dy = dir.dy;
    const Cell c = player.getColor();
    int power = 1;

    if (isCapturable(x0, y0, player.getColor())) {
        return false;
    }

    int x = x0 + dx;
    int y = y0 + dy;
    while (checkInBound(x, y) && c == board_[y][x] && !isCapturable(x, y, player.getColor())) {
        ++power;
        x += dx;
        y += dy;
    }
    x = x0 - dx;
    y = y0 - dy;
    while (checkInBound(x, y) && c == board_[y][x] && !isCapturable(x, y, player.getColor())) {
        ++power;
        x -= dx;
        y -= dy;
    }
    if (power >= 5) {
        return true;
    }
    return false;
}

bool    Board::checkWin(Player const & player, Coord coord) const {
    if (player.getColor() != board_[coord.y][coord.x])
        return false;
    return (checkWinDirection_(player, coord, RIGHT)
        || checkWinDirection_(player, coord, UP)
        || checkWinDirection_(player, coord, DOWN_RIGHT)
        || checkWinDirection_(player, coord, UP_RIGHT)
        || getCapture_(player) >= 5);                 
}

bool    Board::check5AlignmentDirection_(Player const & player, Coord coord, Direction dir) const {
    int x0 = coord.x, y0 = coord.y, dx = dir.dx, dy = dir.dy;
    const Cell c = player.getColor();
    int power = 1;

    int x = x0 + dx;
    int y = y0 + dy;
    while (checkInBound(x, y) && c == board_[y][x]) {
        ++power;
        x += dx;
        y += dy;
    }
    x = x0 - dx;
    y = y0 - dy;
    while (checkInBound(x, y) && c == board_[y][x]) {
        ++power;
        x -= dx;
        y -= dy;
    }
    if (power >= 5) {
        return true;
    }
    return false;
}

bool    Board::checkCaptureWinDirection_(Player const & player, Coord coord, Coord capture, Direction dir) const {
    int x0 = coord.x, y0 = coord.y, dx = dir.dx, dy = dir.dy;
    const Cell c = player.getColor();
    int power = 1;
    bool block5 = false;

    if (x0 == capture.x && y0 == capture.y) {
        return true;
    }

    int x = x0 + dx;
    int y = y0 + dy;
    while (checkInBound(x, y) && c == board_[y][x] && !(x == capture.x && y == capture.y)) {
        ++power;
        x += dx;
        y += dy;
    }
    if (x == capture.x && y == capture.y) {
        block5 = true;
    }
    x = x0 - dx;
    y = y0 - dy;
    while (checkInBound(x, y) && c == board_[y][x] && !(x == capture.x && y == capture.y)) {
        ++power;
        x -= dx;
        y -= dy;
    }
    if (x == capture.x && y == capture.y) {
        block5 = true;
    }
    if (power >= 5) {
        return false;
    }
    return block5;
}

bool    Board::checkCaptureWin(Player const & player, Coord coord, Coord capture) const {
    if (player.getColor() != board_[coord.y][coord.x])
        return false;
    return ((checkCaptureWinDirection_(player, coord, capture, RIGHT) && check5AlignmentDirection_(player, coord, RIGHT))
        || (checkCaptureWinDirection_(player, coord, capture, UP) && check5AlignmentDirection_(player, coord, UP))
        || (checkCaptureWinDirection_(player, coord, capture, DOWN_RIGHT) && check5AlignmentDirection_(player, coord, DOWN_RIGHT))
        || (checkCaptureWinDirection_(player, coord, capture, UP_RIGHT) && check5AlignmentDirection_(player, coord, UP_RIGHT)));
}

void    Board::captureDirection_(Player const & player, Player const & opponent, Coord coord, Direction dir) {
    const bool pattern[4] = {true, false, false, true};
    int i = 0, y0 = coord.y, x0 = coord.x, dx = dir.dx, dy = dir.dy, x = coord.x, y = coord.y;
    while (checkInBound(x, y) && ((player.getColor() == board_[y][x] && pattern[i]) || (opponent.getColor() == board_[y][x] && !pattern[i]))
        && i < 4)
    {
        ++i;
        x += dx;
        y += dy;
    }
    if (i == 4) {
        board_[y0 + dy][x0 + dx] = Cell::Empty;
        board_[y0 + 2 * dy][x0 + 2 * dx] = Cell::Empty;
        black_.capturable[y0 + dy][x0 + dx] = 0;
        white_.capturable[y0 + dy][x0 + dx] = 0;
        black_.capturable[y0 + 2 * dy][x0 + 2 * dx] = 0;
        white_.capturable[y0 + 2 * dy][x0 + 2 * dx] = 0;
        setBoard(Cell::Empty, {x0 + dx, y0 + dy});
        setBoard(Cell::Empty, {x0 + 2 * dx, y0 + 2 * dy});
        updateAlignment_({x0 + dx, y0 + dy});
        updateAlignment_({x0 + 2 * dx, y0 + 2 * dy});
        incrementCapture_(player);
        getPlayerState_(player).align5 = false;
        getPlayerState_(opponent).align5 = false;
    }
}

void    Board::capture(Player const & player, Player const & opponent, Coord coord) {
    for (const auto& dir : DIRECTIONS) {
        captureDirection_(player, opponent, coord, dir);
    }
}

void Board::closeAlignmentDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir) {
    int dx = dir.dx, dy = dir.dy, y = coord.y, x = coord.x;

    // Step 1: move backward to find start of possible alignment
    while (checkInBound(x, y) && (board_[y][x] == color || board_[y][x] == Cell::Empty)) {
        x -= dx;
        y -= dy;
    }

    // Step 2: count total free space
    int free_space = 0;
    x += dx;
    y += dy;
    int start_x = x;
    int start_y = y;
    while (checkInBound(x, y) && (board_[y][x] == color || board_[y][x] == Cell::Empty)) {
        x += dx;
        y += dy;
        ++free_space;
    }

    // Step 3: if not enough space, clear the alignment marks
    if (free_space < 5) {
        x = start_x;
        y = start_y;
        while (checkInBound(x, y) && (board_[y][x] == color || board_[y][x] == Cell::Empty)) {
            alignment[y][x] = 0;
            x += dx;
            y += dy;
        }
    }
}

void Board::updateAlignmentDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir) {
    int i = 0, dx = dir.dx, dy = dir.dy, y0 = coord.y - dy, x0 = coord.x - dx;
    if (!checkInBound(coord.x, coord.y))
        return;
    alignment[coord.y][coord.x] = 0;

    // Go to the start
    while (checkInBound(x0, y0) && board_[y0][x0] == color) {
        alignment[y0][x0] = 0;
        x0 -= dx;
        y0 -= dy;
    }
    
    if (checkInBound(x0, y0))
        alignment[y0][x0] = 0;

    // check if closed on the start side
    bool start_closed = !checkInBound(x0, y0) || board_[y0][x0] != Cell::Empty;
    
    int x = x0 + dx, y = y0 + dy;
    while (checkInBound(x, y)
        && (color == board_[y][x] && color != Cell::Empty))
    {
        alignment[y][x] = 0;
        x += dx;
        y += dy;
        ++i;
    }

    // check if closed on the start side
    bool end_closed = !checkInBound(x, y) || board_[y][x] != Cell::Empty;

    if (i >= 5) {
        getPlayerState_(color).align5Coord = {coord.x, coord.y};
        getPlayerState_(color).align5 = true;
    }
    // Rate the cell
    if (!end_closed && !start_closed) {
        alignment[y0][x0] = open_score[i] / 2;
    }
    else if (!start_closed) {
        alignment[y0][x0] = closed_score[i];
    }

    // check free space is at least 5
    closeAlignmentDirection_(color, alignment, coord, dir);
    closeAlignmentDirection_(color, alignment, coord, {-dx, -dy});
}

void    Board::updateFiguresDirection_(PlayerState & state, Coord coord) {
    int x = coord.x, y = coord.y;
    std::vector<DirMapping> dir = {
        {state.right, RIGHT},   {state.left, LEFT},
        {state.up, UP},         {state.down, DOWN},
        {state.upRight, UP_RIGHT}, {state.downLeft, DOWN_LEFT},
        {state.upLeft, UP_LEFT},   {state.downRight, DOWN_RIGHT}
    };
    int count = 0;
    for (const auto& d : dir) {
        int value = d.state[y][x];
        if (value >= 10) {
            count += 1;
        }
    }
    if (count >= 1) {
        state.figures[y][x] = closed_score[2];
    }
}

void    Board::updateBrokenDirection_(PlayerState & state, Coord coord) {
    int x = coord.x, y = coord.y;
    std::vector<std::pair<DirMapping, DirMapping>> dirPairs = {
        { {state.right, RIGHT},   {state.left, LEFT} },
        { {state.up, UP},         {state.down, DOWN} },
        { {state.upRight, UP_RIGHT}, {state.downLeft, DOWN_LEFT} },
        { {state.upLeft, UP_LEFT},   {state.downRight, DOWN_RIGHT} }
    };
    for (const auto& [dir1, dir2] : dirPairs) {
        int alignment1 = dir1.state[y][x];
        int alignment2 = dir2.state[y][x];
        if (alignment1 >= 1 &&
            alignment2 >= 1) {
            if (alignment1 >= closed_score[3] ||
                alignment2 >= closed_score[3] ||
                (alignment1 >= closed_score[2] && alignment2 >= closed_score[2])) {
                state.broken[y][x] = closed_score[5];
            }
        }
    }
}

void    Board::updateFigures_(Coord coord) {
    for (int i = 0; i < 4; ++i) {
        for (auto dir : DIRECTIONS) {
            int x = coord.x + dir.dx * i;
            int y = coord.y + dir.dy * i;
            if (checkInBound(x, y)) {
                updateFiguresDirection_(black_, {x, y});
                updateFiguresDirection_(white_, {x, y});
            }
        }
    }
}

void    Board::updateBroken_(Coord coord) {
    for (int i = 0; i < 4; ++i) {
        for (auto dir : DIRECTIONS) {
            int x = coord.x + dir.dx * i;
            int y = coord.y + dir.dy * i;
            if (checkInBound(x, y)) {
                updateBrokenDirection_(black_, {x, y});
                updateBrokenDirection_(white_, {x, y});
            }
        }
    }
}

void    Board::updateAlignment_(Coord coord) {
    updateAlignmentDirection_(Cell::Black, black_.right, coord, RIGHT);
    updateAlignmentDirection_(Cell::Black, black_.left, coord, LEFT);
    updateAlignmentDirection_(Cell::Black, black_.up, coord, UP);
    updateAlignmentDirection_(Cell::Black, black_.down, coord, DOWN);
    updateAlignmentDirection_(Cell::Black, black_.upRight, coord, UP_RIGHT);
    updateAlignmentDirection_(Cell::Black, black_.upLeft, coord, UP_LEFT);
    updateAlignmentDirection_(Cell::Black, black_.downRight, coord, DOWN_RIGHT);
    updateAlignmentDirection_(Cell::Black, black_.downLeft, coord, DOWN_LEFT);

    updateAlignmentDirection_(Cell::White, white_.right, coord, RIGHT);
    updateAlignmentDirection_(Cell::White, white_.left, coord, LEFT);
    updateAlignmentDirection_(Cell::White, white_.up, coord, UP);
    updateAlignmentDirection_(Cell::White, white_.down, coord, DOWN);
    updateAlignmentDirection_(Cell::White, white_.upRight, coord, UP_RIGHT);
    updateAlignmentDirection_(Cell::White, white_.upLeft, coord, UP_LEFT);
    updateAlignmentDirection_(Cell::White, white_.downRight, coord, DOWN_RIGHT);
    updateAlignmentDirection_(Cell::White, white_.downLeft, coord, DOWN_LEFT);
    
    updateFigures_(coord);
    updateBroken_(coord);
    // std::cout << black_;
    // std::cout << white_;
}


void        Board::updateCapturableDirection_(Cell const & color, int (&alignment)[BOARD_SIZE][BOARD_SIZE], Coord coord, Direction dir) {
    int x = coord.x + dir.dx, y = coord.y + dir.dy;
    if (checkInBound(x, y)) {
        if (isCapturable(x, y, color))
            alignment[y][x] = 1;
        else
            alignment[y][x] = 0;
    }
}

void        Board::updateCapturable_(Coord coord) {
    for (const auto& dir : DIRECTIONS) {
        for (int i = 0; i < 4; ++i) {
            updateCapturableDirection_(Cell::Black, black_.capturable, coord, {dir.dx * i, dir.dy * i});
            updateCapturableDirection_(Cell::White, white_.capturable, coord, {dir.dx * i, dir.dy * i});
        }
    }
}

void    Board::updateHeatMap_(Coord coord) {
    heatMap_[coord.y][coord.x] = 3;
    for (int i = 2; i > 0; --i) {
        for (auto dir : DIRECTIONS) {
            int new_x = coord.x + dir.dx * i;
            int new_y = coord.y + dir.dy * i;
            if (checkInBound(new_x, new_y))
                heatMap_[new_y][new_x] = std::max(heatMap_[new_y][new_x], 3 - i);
        }
    }
}

long long Board::evaluateAlignments_(PlayerState const & state, PlayerState const & opp_state) {
    long long score = 0;
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            score += state.right[y][x];
            score += state.left[y][x];
            score += state.up[y][x];
            score += state.down[y][x];
            score += state.upRight[y][x];
            score += state.downLeft[y][x];
            score += state.upLeft[y][x];
            score += state.downRight[y][x];

            score += state.figures[y][x];
            score += state.broken[y][x];

            score += opp_state.capturable[y][x] * capture_threat[state.captured];
        }
    }
    return score;
}

/////////////////////////////////////////////////////////////////////
///////////////////MINIMAX///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

long long Board::evaluate(Player const & player, Player const & opponent, Coord lastMove) {
    long long score = 0;
    PlayerState player_state = getPlayerState_(player);
    PlayerState opponent_state = getPlayerState_(opponent);

    // 1. Win / Lose conditions
    if (player_state.captured >= 5) return WIN;  // Capture win rule
    if (opponent_state.captured >= 5) return LOOSE;


    if (checkWin(player, lastMove))
        return WIN;
    if (checkWin(opponent, lastMove))
        return LOOSE;

    // 2. Alignment scores
    score += evaluateAlignments_(player_state, opponent_state);
    score -= evaluateAlignments_(opponent_state, player_state) * DEFENSE_MODIFIER;

    // 2.bis Closed 5
    score += player_state.align5 * closed_score[5];
    score -= opponent_state.align5 * closed_score[5];

    // 3. Capture scores
    score += capture_score[player_state.captured];
    score -= capture_score[opponent_state.captured];

    return score;
}

bool    Board::isGameOver(Player const & player, Player const & opponent, Coord last) {
    if (white_.captured >= 5 || black_.captured >= 5) {
        return true;
    }
    return checkWin(player, last) || checkWin(opponent, last);
}

bool    Board::isForbiddenDoubleThree(Coord coord, Cell const & color) const {
    PlayerState state = getPlayerState_(color);

    int n_double_three = 0, x = coord.x, y = coord.y;

    std::vector<DirMapping> dirMappings = {
        {state.right, RIGHT},
        {state.left, LEFT},
        {state.up, UP},
        {state.down, DOWN},
        {state.upRight, UP_RIGHT},
        {state.upLeft, UP_LEFT},
        {state.downRight, DOWN_RIGHT},
        {state.downLeft, DOWN_LEFT},
    };

    for (auto &dm : dirMappings) {
        if ((dm.state[coord.y][coord.x] == open_score[2] / 2 && board_[coord.y - dm.dir.dy][coord.x - dm.dir.dx] == Cell::Empty)
            || dm.state[coord.y + dm.dir.dy][coord.x + dm.dir.dx] == open_score[2] / 2) {
            n_double_three += 1;
        }
    }
    
    std::vector<std::pair<DirMapping, DirMapping>> dirPairs = {
    { {state.right, RIGHT},   {state.left, LEFT} },
    { {state.up, UP},         {state.down, DOWN} },
    { {state.upRight, UP_RIGHT}, {state.downLeft, DOWN_LEFT} },
    { {state.upLeft, UP_LEFT},   {state.downRight, DOWN_RIGHT} }

    };

    for (const auto& [dir1, dir2] : dirPairs) {
        if (dir1.state[y][x] == open_score[1] / 2 &&
            dir2.state[y][x] == open_score[1] / 2) {
            n_double_three += 1;
        }
    }
    return n_double_three >= 2;
}

std::vector<Coord>  Board::generateMoves(Player const & p1,  Player const & p2) {
    std::vector<CoordValue> coords;
    std::vector<Coord> moves;

    if (getPlayerState_(p1.getColor()).align5) {
        auto moves = getCapturingMovesToWin(p1);
        return moves;
    }
    if (getPlayerState_(p2.getColor()).align5) {
        auto moves = getCapturingMovesToWin(p2);
        return moves;
    }

    memset(heatMap_, 0, sizeof(heatMap_));
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            if (board_[y][x] != Cell::Empty)
                updateHeatMap_({x, y});
        }
    }

    // Compute total score for each cell
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            if (isForbiddenDoubleThree({x, y}, p1.getColor())) continue;
            if (board_[y][x] != Cell::Empty) continue;
            
            long long total_black = black_.right[y][x] + black_.left[y][x] + black_.up[y][x] + black_.down[y][x] +
                        black_.upRight[y][x] + black_.downLeft[y][x] + black_.upLeft[y][x] + black_.downRight[y][x] + 
                        black_.figures[y][x] + black_.broken[y][x];
            long long total_white = white_.right[y][x] + white_.left[y][x] + white_.up[y][x] + white_.down[y][x] +
                        white_.upRight[y][x] + white_.downLeft[y][x] + white_.upLeft[y][x] + white_.downRight[y][x] +
                        white_.figures[y][x] + white_.broken[y][x];

            if (total_black >= closed_score[4] || total_white >= closed_score[4]){
                std::vector<Coord> result;
                result.push_back({x, y});
                return result;
            }
            long long total = total_black + total_white;
            if (total > 0) {
                coords.push_back({x, y, total});
            }
        }
    }

    // Sort by total descending
    std::sort(coords.begin(), coords.end(), [](const CoordValue &a, const CoordValue &b) {
        return a.value > b.value;
    });

    moves.reserve(coords.size());
    for (auto &c : coords) {
        moves.push_back({c.x, c.y});
    }

    if (moves.size() <= 0)
        throw AiException("No move generated the normal way");

    return moves;
}

std::vector<Coord> Board::getCapturingMovesToWin(Player const & player) {
    std::vector<Coord> ret;
    auto& state = getPlayerState_(player.getColor());

    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
            if (isCapturable(x, y, player.getColor()) && checkCaptureWin(player, {x, y}, state.align5Coord)) {
                ret.push_back({x, y});
            }
        }
    }

    return getCapturingMoves(ret, player.getColor());
}
