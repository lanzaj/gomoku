#include "Player.hpp"

Player::Player(Cell color) : color_(color), is_human_(true) {}

Player::~Player() {}

// Copy constructor
Player::Player(Player const & src) : color_(src.color_), is_human_(src.is_human_) {}

// Copy assignment
Player & Player::operator=(Player const & rhs) {
    if (this != &rhs) {
        this->color_ = rhs.color_;
        this->is_human_ = rhs.is_human_;
    }
    return *this;
}

// Getter}
Cell    Player::getColor() const { return color_; }
bool    Player::isHuman()   const { return is_human_; }

// Setter
void    Player::setIsHuman(bool is_human)   { is_human_ = is_human; }

// Ostream

std::ostream & operator<<(std::ostream & os, Player const & instance) {
    os  << "Player " << instance.getColor()
        << " | Human: " << std::boolalpha << instance.isHuman();
    return os;
}