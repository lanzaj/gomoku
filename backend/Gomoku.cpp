#include "Gomoku.hpp"

Gomoku::Gomoku() :  p_black_(Player(Cell::Black)), p_white_(Player(Cell::White)), mode_("") {}
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
    mode_ = server_.init_mode();

    std::cout << "MODE DE JEU " << mode_ << std::endl;
    if (mode_ == "ai") {
        p_white_.setIsHuman(false);
    } else if (mode_ == "human") {}
    else if (mode_ == "demo") {
        p_black_.setIsHuman(false);
        p_white_.setIsHuman(false);
    } else {
        throw Server::ProtocolError("Mode invalid: " + mode_);
    }
}

void    Gomoku::play(void) {
    init_game_();
    while (true) {
        if (playTurn_(p_black_, p_white_))
            break;
        if (playTurn_(p_white_, p_black_))
            break;
    }
    // while (!(playTurn_(p_black_, p_white_)
    //     || playTurn_(p_white_, p_black_))) {
    // }
}

Coord   Gomoku::playHumanTurn_() {
    Coord coord;
    coord = server_.getCoord();

    if (!board_.checkInBound(coord.x) || !board_.checkInBound(coord.y)) {
        server_.send_response(board_, false, false);
        throw std::out_of_range("Coordinates are out of bounds");
    }
    while (board_.getCell(coord.x, coord.y) != Cell::Empty) {
        server_.send_response(board_, false, false);
        coord = server_.getCoord();
        if (!board_.checkInBound(coord.x) || !board_.checkInBound(coord.y)) {
            server_.send_response(board_, false, false);
            throw std::out_of_range("Coordinates are out of bounds");
        }
    }
    return coord;
}

Coord   Gomoku::playAiTurn_() {
    std::cout << "DOES WAIT" << mode_ << std::endl;
    if (mode_ == "demo") {
        server_.waitDemoFront();
    }
    for (int y = 0; y <  board_.getSize(); ++y)
        for (int x = 0; x < board_.getSize(); ++x)
            if (board_.getCell(x, y) == Cell::Empty) {
                std::cout << x << " " << y << std::endl;
                return Coord{x, y};
            }
    return Coord{-1, -1};
}

bool    Gomoku::playTurn_(Player &player, Player &opponent) {
    Coord coord;

    if (player.isHuman()) {
        coord = playHumanTurn_();
    }
    else {
        coord = playAiTurn_();
    }

    int x = coord.x, y = coord.y;
    board_.setBoard(player.getColor(), x, y);
    board_.capture(player, opponent, x, y);
    bool win = board_.checkWin(player, x, y);

    server_.send_response(board_, win, true);

    return win;
}

