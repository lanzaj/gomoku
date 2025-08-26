#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <thread>
# include <vector>
# include <string>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>
# include <nlohmann/json.hpp>

# define PORT 65433
# define BUFFER_SIZE 1024

using json = nlohmann::json;

class Server
{

private:
    int client_socket_;
    void run_server_();

public:
    Server();
    ~Server();

    Server(Server const & src);
    Server & operator=(Server const & rhs);
};

std::ostream & operator<<(std::ostream & os, Server const & instance);

#endif