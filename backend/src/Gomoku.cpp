#include "Gomoku.hpp"

Gomoku::Gomoku() :  p_black_(Player(Cell::Black)), p_white_(Player(Cell::White)), mode_(""), first_move_centered_(false) {
}
Gomoku::~Gomoku() {
    while (!board_.empty()) {
        board_.pop();
    }
}

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
    json data = server_.init_mode();
    if (data.contains("mode"))
        mode_ = data["mode"];
    else
        throw Server::ProtocolError("Missing 'mode' field in client message");

    // if (data.contains("rules"))
    //     rules_ = data["rules"];
    // else
    //     throw Server::ProtocolError("Missing 'rules' field in client message");

    if (data.contains("board_size")) {
        size_ = data["board_size"];
        board_.push(Board(size_));
    }
    else
        throw Server::ProtocolError("Missing 'board_size' field in client message");

    std::cout << "MODE DE JEU " << mode_ << std::endl;
    if (mode_ == "ai") {
        p_white_.setIsHuman(false);
    } else if (mode_ == "human") {}
    else if (mode_ == "demo") {
        p_black_.setIsHuman(false);
        p_white_.setIsHuman(false);
        first_move_centered_ = true;
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

Coord   Gomoku::playHumanTurn_(Player const & player, Player const & opponent) {
    Board board = getBoard();
    Coord coord;

    coord = server_.getCoord();
    if (!board.checkInBound(coord.x, coord.y)) {
        server_.send_response(board, false, false, 0, player, opponent);
    }

    while (board.getCell(coord) != Cell::Empty || board.isForbiddenDoubleThree(coord, player.getColor())) {
        server_.send_response(board, false, false, 0, player, opponent);
        coord = server_.getCoord();
        if (!board.checkInBound(coord.x, coord.y)) {
            server_.send_response(board, false, false, 0, player, opponent);
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
        for (auto move : board_.top().generateMoves(depth, ai, opponent)) {
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
        for (auto move : board_.top().generateMoves(depth, opponent, ai)) {
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


AiMoveResult   Gomoku::playAiTurn_(Player const & player, Player const & opponent) {
    [[maybe_unused]] Board board = getBoard();
    if (mode_ == "demo") {
        server_.waitDemoFront();
    }
    // Timer
    auto start = std::chrono::high_resolution_clock::now();
    MoveEval move = minimax(9, -INF, INF, true, player, opponent, {-1, -1});
    auto end = std::chrono::high_resolution_clock::now();
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    Coord ret = move.bestMove;

    if (ret.x == -1 || ret.y == -1)
        throw Board::AiException("Move not found");
    return {ret, elapsed};
}

bool    Gomoku::playTurn_(Player &player, Player &opponent) {
    Coord coord;
    AiMoveResult result{};
    Board board = Board(getBoard());

    if (player.isHuman()) {
        coord = playHumanTurn_(player, opponent);
    }
    else {
        if (first_move_centered_) {
            coord = Coord{board_.top().getSize() / 2, board_.top().getSize() / 2};
            first_move_centered_ = false;
        }
        else {
            result = playAiTurn_(player, opponent);
            coord = result.move;
        }
    }
    int x = coord.x, y = coord.y;
    board.setBoard(player.getColor(), {x, y});
    board.capture(player, opponent, {x, y});
    bool win = board.checkWin(player, {x, y});

    std::cout << "x:" << coord.x 
        << ", y: " << coord.y 
        << ", score: " << board.evaluate(player, opponent, coord)
        << ", white capture: " << board.getPlayerState_(Cell::White).captured 
        << ", black capture: " << board.getPlayerState_(Cell::Black).captured 
        << std::boolalpha
        << ", white 5s: " << board.getPlayerState_(Cell::White).align5
        << ", black 5s: " << board.getPlayerState_(Cell::Black).align5
        << ", white coord: "
        << board.getPlayerState_(Cell::White).align5Coord.x << " " << board.getPlayerState_(Cell::Black).align5Coord.y << ", "
        << ", black coord: "
        << board.getPlayerState_(Cell::Black).align5Coord.x << " " << board.getPlayerState_(Cell::Black).align5Coord.y << ", "
        << std::endl;

    board_.push(board);
    server_.send_response(board, win, true, result.timeMs, player, opponent);

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
