#include "Board.hpp"

Board::Board() : size_(BOARD_SIZE) 
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board_[i][j] = '.';
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
        for (int i = 0; i < size_; ++i)
            for (int j = 0; j < size_; ++j)
                board_[i][j] = rhs.board_[i][j];
    }
    return *this;
}

const char (&Board::getBoard() const)[BOARD_SIZE][BOARD_SIZE] {
    return board_;
}

void    Board::playTurn(Player player) {
    char c = '.';
    std::cout << "Enter two integers: ";

    c = player.getChar();

    int x = -1, y = -1;
    while (y < 0 || x < 0 || y > size_ || x > size_ || board_[y][x] != '.')
        std::cin >> x >> y;
    if (y < 0 || x < 0 || y > size_ || x > size_) {
        throw std::out_of_range("Coordinates are out of bounds");
    }
    if (board_[y][x] != '.') {
        throw std::runtime_error("Cell is already occupied");
    }
    board_[y][x] = c;


    this->checkWin(player, x, y);

    std::cout << *this;
}

bool    Board::checkOutOfBound(int n) {
    return (n >= 0 && n < BOARD_SIZE);
}

void    Board::checkWinDir(Player player, int x0, int y0, int dx, int dy){
    const char c = player.getChar();
    int power = 1;

    int x = x0 + dx;
    int y = y0 + dy;
    while (c == board_[y][x] && this->checkOutOfBound(x) && this->checkOutOfBound(y)) {
        ++power;
        x += dx;
        y += dy;
    }
    x = x0 - dx;
    y = y0 - dy;
    while (c == board_[y][x] && this->checkOutOfBound(x) && this->checkOutOfBound(y)) {
        ++power;
        x -= dx;
        y -= dy;
    }
    if (power >= 5) {
        std::cout << "WIN!" << std::endl;
    }
}

void    Board::checkWin(Player player, int x, int y) {
    this->checkWinDir(player, x, y, 1, 0); // Horizontal
    this->checkWinDir(player, x, y, 0, 1); // Vertical
    this->checkWinDir(player, x, y, 1, 1); // Diagonal decreasing
    this->checkWinDir(player, x, y, 1, -1);// Diagonal increasing
}

std::ostream & operator<<(std::ostream & os, Board const & instance)
{
    const char (&grid)[BOARD_SIZE][BOARD_SIZE] = instance.getBoard();

    os << "  ";
    for (int x = 0; x < BOARD_SIZE; ++x)
        os << std::setw(2) << x;
    os << "\n";
    for (int y = 0; y < BOARD_SIZE; ++y) {
        os << std::setw(2) << y << " ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            os << grid[y][x] << " ";
        }
        os << std::endl;
    }
    return os;
}