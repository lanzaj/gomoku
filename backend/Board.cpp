#include "Board.hpp"

Board::Board() : size_(BOARD_SIZE) 
{
    for (int i = 0; i < size_; ++i)
        for (int j = 0; j < size_; ++j)
            board_[i][j] = Cell::Empty;
}

Board::~Board() {}

// Copy constructor
Board::Board(Board const & src) : size_(src.size_) {
    for (int i = 0; i < size_; ++i)
        for (int j = 0; j < size_; ++j)
            board_[i][j] = src.board_[i][j];
}

// Copy assignment
Board & Board::operator=(Board const & rhs) {
    if (this != &rhs) {
        size_ = rhs.size_;
        for (int y = 0; y < size_; ++y)
            for (int x = 0; x < size_; ++x)
                board_[y][x] = rhs.board_[y][x];
    }
    return *this;
}

// Getter
const Cell (&Board::getBoard() const)[BOARD_SIZE][BOARD_SIZE] {
    return board_;
}

Cell  Board::getCell(int x, int y) const {
    if (!checkInBound(x) || !checkInBound(y)) {
        return Cell::Empty;
    }
    return board_[y][x];
}

int   Board::getSize() const {
    return size_;
}

// Setter
void Board::setBoard(Cell c, int x, int y) {
    board_[y][x] = c;
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

// Functions
bool    Board::checkInBound(int n) const {
    return (n >= 0 && n < size_);
}

bool    Board::checkWinDirections_(Player const & player, int x0, int y0, int dx, int dy) const {
    const Cell c = player.getSymbol();
    int power = 1;

    int x = x0 + dx;
    int y = y0 + dy;
    while (checkInBound(x) && checkInBound(y) && c == board_[y][x]) {
        ++power;
        x += dx;
        y += dy;
    }
    x = x0 - dx;
    y = y0 - dy;
    while (checkInBound(x) && checkInBound(y) && c == board_[y][x]) {
        ++power;
        x -= dx;
        y -= dy;
    }
    if (power >= 5) {
        std::cout << "WIN!" << std::endl;
        return true;
    }
    return false;
}

bool    Board::checkWin(Player const & player, int x, int y) const {
    return (checkWinDirections_(player, x, y, 1, 0)   // Horizontal
        || checkWinDirections_(player, x, y, 0, 1)    // Vertical
        || checkWinDirections_(player, x, y, 1, 1)    // Diagonal decreasing
        || checkWinDirections_(player, x, y, 1, -1)
        || player.getCapture() >= 5); // Diagonal increasing
}

void    Board::captureDirection_(Player & player, Player & opponent, int x0, int y0, int dx, int dy) {
    const bool pattern[4] = {true, false, false, true};
    int i = 0;
    int x = x0;
    int y = y0;
    while (((player.getSymbol() == board_[y][x] && pattern[i]) || (opponent.getSymbol() == board_[y][x] && !pattern[i]))
        && checkInBound(x) && checkInBound(y) && i < 4)
    {
        ++i;
        x += dx;
        y += dy;
    }
    if (i == 4) {
        setBoard(Cell::Empty, x0 + dx, y0 + dy);
        setBoard(Cell::Empty, x0 + 2 * dx, y0 + 2 * dy);
        player.incrementCapture();
    }
}

void    Board::capture(Player &player, Player &opponent, int x, int y) {
    static const std::pair<int, int> directions[] = {
        // Left  // Center // Right
        {-1, -1}, {0, -1}, {1, -1},  // UP
        {-1,  0},          {1,  0},  // Center
        {-1,  1}, {0,  1}, {1,  1},  // Down
        // 8 directions around the placed stone
    };
    for (const auto& [dx, dy] : directions) {
        captureDirection_(player, opponent, x, y, dx, dy);
    }
}