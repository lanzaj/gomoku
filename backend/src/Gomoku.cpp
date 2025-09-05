#include "Gomoku.hpp"

Gomoku::Gomoku() :  p_black_(Player(Cell::Black)), p_white_(Player(Cell::White)), mode_("") {
    board_.push(Board());
}
Gomoku::~Gomoku() {}

// Getter
const Board &Gomoku::getBoard(void) const
{
    return board_.top();
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
    Board board = getBoard();
    Coord coord;
    coord = server_.getCoord();

    if (!board.checkInBound(coord.x, coord.y)) {
        server_.send_response(board, false, false);
    }
    while (board.getCell(coord) != Cell::Empty) {
        server_.send_response(board, false, false);
        coord = server_.getCoord();
        if (!board.checkInBound(coord.x, coord.y)) {
            server_.send_response(board, false, false);
        }
    }
    return coord;
}

MoveEval Gomoku::minimax(int depth, int alpha, int beta, bool maximizing, 
                 Player const & ai, Player const & player) {
    if (depth == 0 || board_.top().isGameOver()) {
        if (maximizing)
            return { board_.top().evaluate(ai, player), Coord{-1,-1} };
        else
            return { board_.top().evaluate(player, ai), Coord{-1,-1} };
    }

    if (maximizing) {
        MoveEval best = { -INF, Coord{-1, -1} };
        for (auto move : board_.top().generateMoves()) {
            play_(move, ai, player);
            MoveEval eval = minimax(depth-1, alpha, beta, false, ai, player);
            undo_();

            if (eval.score > best.score) {
                best.score = eval.score;
                best.bestMove = move;   // store the move
            }
            alpha = std::max(alpha, best.score);
            if (beta <= alpha) break;  // prune
        }
        return best;
    } else {
        MoveEval best = { +INF, Coord{-1, -1} };
        for (auto move : board_.top().generateMoves()) {
            play_(move, player, ai);
            MoveEval eval = minimax(depth-1, alpha, beta, true, ai, player);
            undo_();

            if (eval.score < best.score) {
                best.score = eval.score;
                best.bestMove = move;
            }
            beta = std::min(beta, best.score);
            if (beta <= alpha) break;  // prune
        }
        return best;
    }
}


Coord   Gomoku::playAiTurn_(Player const & player, Player const & opponent) {
    Board board = getBoard();
    std::cout << "DOES WAIT " << mode_ << std::endl;
    if (mode_ == "demo") {
        server_.waitDemoFront();
    }
    Coord ret = minimax(4, NEG_INF, INF, true, player, opponent).bestMove;
    std::cout << "x:" << ret.x << ",y: " << ret.y << std::endl;
    return ret;
}

bool    Gomoku::playTurn_(Player &player, Player &opponent) {
    Coord coord;
    Board board = Board(getBoard());

    if (player.isHuman()) {
        coord = playHumanTurn_();
    }
    else {
        coord = playAiTurn_(player, opponent);
    }

    int x = coord.x, y = coord.y;
    board.setBoard(player.getColor(), {x, y});
    board.capture(player, opponent, {x, y});
    bool win = board.checkWin(player, {x, y});

    board_.push(board);
    server_.send_response(board, win, true);

    return win;
}


void    Gomoku::play_(Coord coord, Player const & player,  Player const & opponent) {
    Board newBoard = board_.top();
    int x = coord.x, y = coord.y;

    newBoard.setBoard(player.getColor(), {x, y});
    newBoard.capture(player, opponent, {x, y});
    board_.push(newBoard);
}

void    Gomoku::undo_() {
    board_.pop();
}
