#include "Server.hpp"

Server::Server()
{
    run_server_();
}

Server::~Server()
{
}

Server::Server(Server const & src)
{
}

Server & Server::operator=(Server const & rhs)
{
}

void Server::run_server_() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

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
            perror("accept");
            continue;
        }
        if (client_socket > 0) {
            std::cout << "cool" << std::endl;
            client_socket_ = client_socket;
        }
    }
}
