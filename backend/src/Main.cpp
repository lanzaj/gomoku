#include <iostream>
#include "Gomoku.hpp"

    /// DONE ///
// Ajouter le fait que l'on peut ne pas win si l'autre peut capturer
// Check isForbiddenDoubleThree pour capture
// CHECK out of bound du front
// faire les figures ! 

    /// TODO ///
// fix le double three
// faire des figures non lineaires

// gerer couleur du joueur vs ia (bonus)
// voir pour reduire profondeur sur les premiers coups

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