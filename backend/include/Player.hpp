#ifndef PLAYER_HPP
# define PLAYER_HPP

# include <iostream>
# include "Cell.hpp"

class Player
{
    private:
        Cell    color_;   // Determines if it's the black or white player
        bool    is_human_;// True if the player is human, False if it's a bot

    public:
        Player(Cell color);
        ~Player();

        Player(Player const & src);
        Player & operator=(Player const & rhs);

        // Getter
        Cell    getColor() const;
        bool    isHuman() const;

        // Setter
        void    setIsHuman(bool is_human);
    };

std::ostream & operator<<(std::ostream & os, Player const & instance);

#endif