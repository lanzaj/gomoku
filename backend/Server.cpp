#include "Server.hpp"

Server::Server()
{
    run_server_();
}

Server::~Server()
{
    close(client_socket_);
}

Server::Server(Server const & src) : client_socket_(src.client_socket_) {}

Server & Server::operator=(Server const & rhs)
{
    if (this != &rhs) {
        this->client_socket_ = rhs.client_socket_;
    }
    return *this;
}

void Server::run_server_() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
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
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Serveur IA en écoute sur le port " << PORT << std::endl;

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

    ssize_t bytes_received = recv(client_socket_, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
        throw Server::ProtocolError("Connection lost");
    }

    json data = json::parse(buffer);
    std::cout << "Reçu du client : " << data.dump() << std::endl;
    return data;
}

void    Server::send_response(Board board, bool win, bool authorized) {

    json json_game_state = json::array();

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board.getCell(x, y) == 'X')
                json_game_state.push_back({
                    {"x", x},
                    {"y", y},
                    {"color", "black"}
                });
            if (board.getCell(x, y) == 'O')
                json_game_state.push_back({
                    {"x", x},
                    {"y", y},
                    {"color", "white"}
                });
        }
    }

    json response = {
        {"game_state", json_game_state},
        {"win", win},
        {"authorized", authorized},
    };

    std::string response_str = response.dump();
    send(client_socket_, response_str.c_str(), response_str.length(), 0);
}

std::string     Server::init_mode() {
    json data = recv_json_();

    if (data.contains("mode"))
        return (data["mode"]);
    else
        throw Server::ProtocolError("Missing 'mode' field in client message");
}

std::tuple<int, int>    Server::getCoord() {
    json data = recv_json_();

    if (data.contains("x") && data.contains("y"))
        return (std::make_tuple(data["x"], data["y"]));
    else
        throw Server::ProtocolError("Missing 'mode' field in client message");
}