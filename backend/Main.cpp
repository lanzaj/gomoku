#include <iostream>
#include "Gomoku.hpp"

int main(void) {
    try
    {
        Gomoku game;
        game.play();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}