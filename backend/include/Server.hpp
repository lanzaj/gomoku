#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <string>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>
# include <nlohmann/json.hpp>
# include "Board.hpp"

constexpr int PORT = 65433;
constexpr std::size_t BUFFER_SIZE = 1024;

using json = nlohmann::json;

class Server
{
    private:
        int client_socket_;

        void    run_server_();
        json    recv_json_();

    public:
        Server();
        ~Server();

        // Functions
        json    init_mode();
        Coord   getCoord();
        void    waitDemoFront();
        void    send_response(Board const & board, bool win, bool authorized, long long timeMs, Coord suggestion, bool swap, int count_turn);

        // Exception
        class ProtocolError : public std::runtime_error {
            public:
                explicit ProtocolError(const std::string& msg)
                    : std::runtime_error("Protocol error: " + msg) {}
        };

        class ExitButton : public std::exception {
            public:
                const char* what() const noexcept override {
                    return "Game exit requested";
                }
        };
};

std::ostream & operator<<(std::ostream & os, Server const & instance);

#endif