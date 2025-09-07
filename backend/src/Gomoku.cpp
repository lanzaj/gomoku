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

MoveEval Gomoku::minimax(int depth, long long alpha, long long beta, bool maximizing, 
                 Player const & ai, Player const & opponent, Coord lastMove) {
    if (depth == 0 || board_.top().isGameOver(ai, opponent, lastMove)) {
        return { board_.top().evaluate(ai, opponent, lastMove), lastMove };
    }

    if (maximizing) {
        MoveEval best = { -INF, Coord{-1, -1} };
        for (auto move : board_.top().generateMoves(depth, ai)) {
            play_(move, ai, opponent);
            MoveEval eval = minimax(depth-1, alpha, beta, false, ai, opponent, move);
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
        for (auto move : board_.top().generateMoves(depth, opponent)) {
            play_(move, opponent, ai);
            MoveEval eval = minimax(depth-1, alpha, beta, true, ai, opponent, move);
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
    if (mode_ == "demo") {
        server_.waitDemoFront();
    }
    MoveEval move = minimax(8, -INF, INF, true, player, opponent, {-1, -1});
    Coord ret = move.bestMove;

    std::cout << "x:" << ret.x 
        << ", y: " << ret.y 
        << ", score: " << move.score
        << ", white capture: " << board_.top().getPlayerState_(Cell::White).captured 
        << ", black capture: " << board_.top().getPlayerState_(Cell::Black).captured 
        << ", white 5s: " << board_.top().getPlayerState_(Cell::White).closed5 
        << ", black 5s: " << board_.top().getPlayerState_(Cell::Black).closed5 
        << std::endl;

    if (ret.x == -1)
        throw AiException("Move not found");
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

    std::cout << "x:" << coord.x 
        << ", y: " << coord.y 
        << ", score: " << board.evaluate(player, opponent, coord)
        << ", white capture: " << board_.top().getPlayerState_(Cell::White).captured 
        << ", black capture: " << board_.top().getPlayerState_(Cell::Black).captured 
        << ", white 5s: " << board_.top().getPlayerState_(Cell::White).closed5 
        << ", black 5s: " << board_.top().getPlayerState_(Cell::Black).closed5 
        << std::endl;

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
