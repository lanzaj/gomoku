#ifndef PLAYER_HPP
# define PLAYER_HPP

# include <iostream>
# include "Cell.hpp"

class Player
{
    private:
        int     capture_; // Number of captures made by the player. According to the game rules, 5 captures = win.
        Cell    color_;   // Determines if it's the black or white player
        bool    is_human_;// True if the player is human, False if it's a bot

    public:
        Player(Cell color);
        ~Player();

        Player(Player const & src);
        Player & operator=(Player const & rhs);

        // Getter
        int     getCapture() const;
        Cell    getColor() const;
        bool    isHuman() const;

        // Setter
        void setIsHuman(bool is_human);
        void incrementCapture();
    };

std::ostream & operator<<(std::ostream & os, Player const & instance);

#endif