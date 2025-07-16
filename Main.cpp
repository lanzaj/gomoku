#include <iostream>
#include "Gomoku.hpp"

int main(void) {
    Gomoku game = Gomoku();

    std::cout << game.getBoard();
    
    return (0);
}