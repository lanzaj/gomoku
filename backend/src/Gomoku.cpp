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
void    Gomoku::init_rule_() {
    if (start_option_ == "standard")
        p_white_.setIsHuman(false);
    else if (start_option_ == "ai_first") {
        first_move_centered_ = true;
        p_black_.setIsHuman(false);
    }
    else if (start_option_ == "pro") {
        first_move_centered_ = true;
        p_white_.setIsHuman(false);
    }
    else if (start_option_ == "swap") {}
}

void    Gomoku::init_game_() {
    json data = server_.init_mode();
    if (data.contains("mode"))
        mode_ = data["mode"];
    else
        throw Server::ProtocolError("Missing 'mode' field in client message");

    if (data.contains("board_size")) {
        size_ = data["board_size"];
        board_.push(Board(size_));
    }
    else
        throw Server::ProtocolError("Missing 'board_size' field in client message");

    if (data.contains("start_option")) {
        start_option_ = data["start_option"];
    }
    else
        throw Server::ProtocolError("Missing 'start_option' field in client message");


    std::cout << "MODE DE JEU " << mode_ << std::endl;
    if (mode_ == "ai") {
        init_rule_();
    } 
    else if (mode_ == "human") {}
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

Coord   Gomoku::playHumanTurn_(Player const & player, Player const & opponent, Board & board) {
    Coord coord;

    while (true) {
        coord = server_.getCoord();
        if (first_move_centered_) {
            first_move_centered_ = false;
            coord = {size_ / 2, size_ / 2};
            pro_flag_5x5_ = true;
            break;
        }

        if (!board.checkInBound(coord.x, coord.y))
        {
            server_.send_response(board, false, false, 0, {-1, -1}, false, n_turn);
            continue;
        }
        if (board.getCell(coord) != Cell::Empty)
        {
            server_.send_response(board, false, false, 0, {-1, -1}, false, n_turn);
            continue;
        }
        if (board.isForbiddenDoubleThree(coord, player.getColor()))
        {
            server_.send_response(board, false, false, 0, {-1, -1}, false, n_turn);
            continue;
        }
        if (board.getPlayerState_(opponent).align5) {
            auto capturingMoves = board.getCapturingMovesToWin(opponent);
            if (capturingMoves.size() == 0)
                throw Board::AiException("No capturing move found");
            
            if (std::find(capturingMoves.begin(), capturingMoves.end(), coord) 
                == capturingMoves.end())
            {
                server_.send_response(board, false, false, 0, {-1, -1}, false, n_turn);
                continue;
            }
        }

        if (pro_flag_5x5_) {
            if (!(size_ / 2 - 2 <= coord.x && coord.x <= size_ / 2 + 2 
             && size_ / 2 - 2 <= coord.y && coord.y <= size_ / 2 + 2)) {
                pro_flag_5x5_ = false;
            }
            else {
                server_.send_response(board, false, false, 0, {-1, -1}, false, n_turn);
                continue;
            }

        }

        // All checks passed: valid move
        break;
    }

    return coord;
}

MoveEval Gomoku::minimax(int depth, long long alpha, long long beta, bool maximizing, 
                 Player const & p1, Player const & p2, Coord lastMove) {
                    if (depth >= 9 || board_.top().isGameOver(p1, p2, lastMove)) {
        return { board_.top().evaluate(p1, p2, lastMove), {-1, -1} };
    }
    int limit = board_.top().beam_search[std::min(depth, 9)];

    if (maximizing) {
        MoveEval best = { -INF, Coord{-1, -1} };
        int count = 0;
        for (auto move : board_.top().generateMoves(p1, p2)) {
            if (count >= limit && best.bestMove.x != -1 && best.bestMove.y != -1)
                break;
            play_(move, p1, p2);
            MoveEval eval = minimax(depth + 1, alpha, beta, false, p1, p2, move);
            undo_();

            if (eval.score > best.score) {
                best.score = eval.score;
                best.bestMove = move;   // store the move
            }
            alpha = std::max(alpha, best.score);
            if (beta <= alpha) break;  // prune
            ++count;
        }
        return best;
    } else {
        MoveEval best = { +INF, Coord{-1, -1} };
        int count = 0;
        for (auto move : board_.top().generateMoves(p2, p1)) {
            if (count >= limit && best.bestMove.x != -1 && best.bestMove.y != -1)
                break;
            play_(move, p2, p1);
            MoveEval eval = minimax(depth + 1, alpha, beta, true, p1, p2, move);
            undo_();

            if (eval.score < best.score) {
                best.score = eval.score;
                best.bestMove = move;
            }
            beta = std::min(beta, best.score);
            if (beta <= alpha) break;  // prune
            ++count;
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
    MoveEval move = minimax(n_turn < 10 ? 1 : 0, -INF, INF, true, player, opponent, {-1, -1});
    auto end = std::chrono::high_resolution_clock::now();
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    Coord ret = move.bestMove;
    
    if (ret.x == -1 || ret.y == -1) {
        throw Board::AiException("Move not found");
    }
    // std::cout << board;
    return {ret, elapsed};
}

bool    Gomoku::playTurn_(Player &player, Player &opponent) {
    Coord coord;
    AiMoveResult result{};
    Board board = Board(getBoard());

    if (player.isHuman()) {
        coord = playHumanTurn_(player, opponent, board);
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
    board.setBoardWithCapture(player.getColor(), {x, y}, player, opponent);
    bool win = board.checkWin(player, {x, y});


    board_.push(board);

    Coord suggestion = {-1, -1};
    if (mode_ == "human")
        suggestion = minimax(0, -INF, INF, true, player, opponent, {-1, -1}).bestMove;

    std::cout << "x:" << coord.x 
        << ", y: " << coord.y 
        << ", score: " << board.evaluate(player, opponent, coord)
        << ", white capture: " << board.getPlayerState_(Cell::White).captured 
        << ", black capture: " << board.getPlayerState_(Cell::Black).captured 
        << std::boolalpha
        << ", white 5s: " << board.getPlayerState_(Cell::White).align5
        << ", black 5s: " << board.getPlayerState_(Cell::Black).align5
        << ", white coord: "
        << board.getPlayerState_(Cell::White).align5Coord.x << " " << board.getPlayerState_(Cell::White).align5Coord.y << ", "
        << "black coord: "
        << board.getPlayerState_(Cell::Black).align5Coord.x << " " << board.getPlayerState_(Cell::Black).align5Coord.y << ", "
        << "suggestion: (" << suggestion.x << ", " << suggestion.y << ")"
        << std::endl;

    n_turn += 1;

    if (n_turn == 3 && start_option_ == "swap") {
        int score = board.evaluate(player, opponent, {9, 9});
        std::cout << "score is : " << score << std::endl;
        if (score >= 0) {
            server_.send_response(board, win, true, 0, {-1, -1}, true, n_turn);
            player.setIsHuman(false); // Un humain joue au prochain tour
        }
        else {
            server_.send_response(board, win, true, 0, {-1, -1}, false, n_turn);
            opponent.setIsHuman(false); // Une IA joue au prochain tour
        }
        return win;
    }
    server_.send_response(board, win, true, result.timeMs, suggestion, false, n_turn);

    return win;
}


void    Gomoku::play_(Coord coord, Player const & player,  Player const & opponent) {
    Board newBoard = board_.top();
    int x = coord.x, y = coord.y;

    newBoard.setBoardWithCapture(player.getColor(), {x, y}, player, opponent);
    board_.push(newBoard);
}

void    Gomoku::undo_() {
    board_.pop();
}
