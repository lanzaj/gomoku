#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define PORT 65432
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = (client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break; // Client disconnected
        }

        try {
            json move = json::parse(buffer);
            std::cout << "Reçu du client : " << move.dump() << std::endl;

            int x = move["x"];
            int y = move["y"];

            json ai_response = {
                {"to_place", {
                    {{"x", (x + 1 < 19 ? x + 1 : x)}, {"y", y}, {"color", "white"}}
                }},
                {"to_remove", {
                    {{"x", x - 1}, {"y", y}},
                    {{"x", x - 2}, {"y", y}}
                }}
            };

            std::string response_str = ai_response.dump();
            send(client_socket, response_str.c_str(), response_str.length(), 0);
        } catch (const std::exception& e) {
            std::cerr << "Erreur JSON: " << e.what() << std::endl;
        }
    }

    close(client_socket);
}

void run_server() {
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
        std::thread(handle_client, client_socket).detach();
    }
}

int main() {
    run_server();
    return 0;
}
