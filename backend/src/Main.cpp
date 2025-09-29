#include <iostream>
#include "Gomoku.hpp"

int main(void) {
    try
    {
        Gomoku game;
        game.play();
    }
    catch(const Server::ExitButton& e)
    {
        std::cout << e.what() << std::endl;
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}