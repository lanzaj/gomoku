#include "Server.hpp"

Server::Server()
{
    run_server_();
}

Server::~Server()
{
    if (client_socket_ >= 0)
        close(client_socket_);
}

void Server::run_server_() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // DEV //
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    /////////
    
    sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // localhost
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            throw Server::ProtocolError("Accept");
        }
        if (client_socket > 0) {
            std::cout << "Connected" << std::endl;
            this->client_socket_ = client_socket;
            if (close(server_fd) < 0) {
                perror("close server_fd");
            }
            break;
        }
    }
}

json    Server::recv_json_() {
    char buffer[BUFFER_SIZE];
    std::string mode = "";
    memset(buffer, 0, BUFFER_SIZE);

    ssize_t bytes_received = 0;
    
    while (recv(client_socket_, buffer, BUFFER_SIZE, 0) == 0) {
        if (bytes_received < 0) {
            throw Server::ProtocolError("Connection lost");
        }
    }
    json data = json::parse(buffer);

    if (data.contains("exit")) {
        close(client_socket_);
        throw Server::ExitButton();
    }

    return data;
}

void    Server::send_response(Board const & board, bool win, bool authorized, long long timeMs, Coord suggestion, bool swap, int count_turn) {

    json json_game_state = json::array();

    for (int y = 0; y < board.getSize(); ++y) {
        for (int x = 0; x < board.getSize(); ++x) {
            if (board.getCell({x, y}) == Cell::Black)
                json_game_state.push_back({
                    {"x", x},
                    {"y", y},
                    {"color", "black"}
                });
            if (board.getCell({x, y}) == Cell::White)
                json_game_state.push_back({
                    {"x", x},
                    {"y", y},
                    {"color", "white"}
                });
        }
    }

    if (suggestion.x != -1 && suggestion.y != -1) {
        json_game_state.push_back({
                        {"x", suggestion.x},
                        {"y", suggestion.y},
                        {"color", "sugg"}
        });
    }

    json response = {
        {"game_state", json_game_state},
        {"delay", timeMs},
        {"win", win},
        {"authorized", authorized},
        {"swap", swap},
        {"count_turn", count_turn}
    };

    std::string response_str = response.dump();
    if (send(client_socket_, response_str.c_str(), response_str.length(), 0) < 0)
        throw ProtocolError("Failed to send response");
}

json     Server::init_mode() {
    json data = recv_json_();
    return data;
}

Coord   Server::getCoord() {
    json data = recv_json_();

    if (data.contains("x") && data.contains("y"))
        return (Coord{data["x"], data["y"]});
    else
        throw Server::ProtocolError("Missing 'x' or 'y' field in client message");
}

void   Server::waitDemoFront() {
    json data = recv_json_();

    if (data.contains("waiting"))
        return;
    else
        throw Server::ProtocolError("Missing 'x' or 'y' field in client message");
}