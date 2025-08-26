#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define PORT 65433
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    std::string mode = "";

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break; // Client disconnected
        }

        try {
            json data = json::parse(buffer);
            std::cout << "Reçu du client : " << data.dump() << std::endl;

            if (data.contains("mode"))
                mode = data["mode"];
            else
            {
                // [human vs ai] & [human vs human] : coup autorisé ?

                int x = data["x"];
                int y = data["y"];

                json json_game_state = json::array();

                json_game_state.push_back({
                    {"x", x},
                    {"y", y},
                    {"color", "black"}
                });

                json response = {
                    {"game_state", json_game_state},
                    {"win", false},
                    {"authorized", true},
                };

                std::string response_str = response.dump();
                send(client_socket, response_str.c_str(), response_str.length(), 0);
            }
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
        handle_client(client_socket);
    }
}

int main() {
    run_server();
    return 0;
}
