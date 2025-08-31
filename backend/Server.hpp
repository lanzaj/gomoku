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
        std::string init_mode();
        Coord       getCoord();
        void        send_response(Board const & board, bool win, bool authorized);

        // Exception
        class ProtocolError : public std::runtime_error {
            public:
                explicit ProtocolError(const std::string& msg)
                    : std::runtime_error("Protocol error: " + msg) {}
        };
};

std::ostream & operator<<(std::ostream & os, Server const & instance);

#endif