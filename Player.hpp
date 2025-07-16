#ifndef PLAYER_HPP
# define PLAYER_HPP

# include <iostream>

class Player
{

private:
    /*data*/

public:
    Player(/* args */);
    ~Player();

    // Player(Player const & src);
    // Player & operator=(Player const & rhs);
};

std::ostream & operator<<(std::ostream & os, Player const & instance);

#endif