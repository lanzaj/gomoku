#include "Gomoku.hpp"

Gomoku::Gomoku() :  p_black_(Player('X')), p_white_(Player('O')) {}
Gomoku::~Gomoku() {}

// Getter
const Board &Gomoku::getBoard(void) const
{
    return board_;
}

// Operator <<
std::ostream & operator<<(std::ostream & os, Gomoku const & instance)
{
    os << instance.getBoard();
    return os;
}

// Functions
void    Gomoku::init_game_() {
    std::string mode = server_.init_mode().c_str();

    std::cout << "MODE DE JEU " << mode << std::endl;
    if (mode == "ai") {
        p_white_.setIsHuman(false);
    } else if (mode == "human") {;}
    else if (mode == "demo") {
        p_black_.setIsHuman(false);
        p_white_.setIsHuman(false);
    } else {
        throw Server::ProtocolError("Mode invalid");
    }
}

void    Gomoku::play(void) {
    init_game_();
    while (!(playTurn_(p_black_, p_white_)
        || playTurn_(p_white_, p_black_))) {
    }
}

std::tuple<int, int>    Gomoku::playHumanTurn_() {
    int x, y;
    std::tie(x, y) = server_.getCoord();

    if (!board_.checkInBound(x) || !board_.checkInBound(y)) {
        throw std::out_of_range("Coordinates are out of bounds");
        server_.send_response(board_, false, false);
    }
    while (board_.getCell(x, y) != '.') {
        server_.send_response(board_, false, false);
        std::tie(x, y) = server_.getCoord();
    }
    return std::make_tuple(x, y);
}

std::tuple<int, int>    Gomoku::playAiTurn_() {
    server_.getCoord();
    for (int y = 0; y <  board_.getSize(); ++y)
        for (int x = 0; x < board_.getSize(); ++x)
            if (board_.getCell(x, y) == '.')
                return std::make_tuple(x, y);
    return std::make_tuple(-1, -1);
}

bool    Gomoku::playTurn_(Player player, Player opponent) {
    std::tuple<int, int> coord;
    int x, y;

    if (player.isHuman()) {
        coord = playHumanTurn_();
        std::cout << "Human turn" << std::endl;
    }
    else {
        coord = playAiTurn_();
        std::cout << "Ai turn" << std::endl;
    }

    std::tie(x, y) = coord;
    board_.setBoard(player.getChar(), x, y);
    board_.capture(player, opponent, x, y);
    bool win = board_.checkWin(player, x, y);

    server_.send_response(board_, win, true);

    return win;
}

