#include <iostream>
#include "Gomoku.hpp"

// To Do ajouter le fait que l'on peut ne pas win si l'autre peut capturer
// To Do check isForbiddenDoubleThree pour capture

// CHECK out of bound du front 
// GERER receive exit: true
// timer
// suggestion de move

// Proposer des coups

// gerer couleur du joueur vs ia (bonus)
// voir pour reduire profondeur sur les premiers coups

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