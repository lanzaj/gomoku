#ifndef PLAYER_HPP
# define PLAYER_HPP

# include <iostream>

class Player
{
    private:
        int     capture_; // 5 capture is a win
        char    c_;
        bool    is_human_;

    public:
        Player(char c);
        ~Player();

        Player(Player const & src);
        Player & operator=(Player const & rhs);

        // Getter
        int     getCapture() const;
        char    getChar() const;
        bool    isHuman() const;

        // Setter
        void setIsHuman(bool is_human);
        void addCapture(int count);
    };

std::ostream & operator<<(std::ostream & os, Player const & instance);

#endif