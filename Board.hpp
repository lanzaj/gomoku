#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>

class Board
{

private:
    char board[19][19];

public:
    Board(/* args */);
    ~Board();

    // Board(Board const & src);
    // Board & operator=(Board const & rhs);

    // Getter
    const char (&getBoard() const)[19][19];
};

std::ostream & operator<<(std::ostream & os, Board const & instance);

#endif