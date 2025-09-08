#include <iostream>
#include "Gomoku.hpp"

// To Do ajouter le fait que l'on peut ne pas win si l'autre peut capturer
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