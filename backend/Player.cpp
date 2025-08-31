#include "Player.hpp"

Player::Player(char c) : capture_(0), c_(c), is_human_(true) {}

Player::~Player() {}

// Copy constructor
Player::Player(Player const & src) : capture_(src.capture_), c_(src.c_), is_human_(src.is_human_) {}

// Copy assignment
Player & Player::operator=(Player const & rhs) {
    if (this != &rhs) {
        this->capture_ = rhs.capture_;
        this->c_ = rhs.c_;
        this->is_human_ = rhs.is_human_;
    }
    return *this;
}

// Getter
int Player::getCapture() const {
    return capture_;
}

char    Player::getChar() const {
    return c_;
}

bool    Player::isHuman() const {
    return is_human_;
}

// Setter
void    Player::setIsHuman(bool is_human) {
    is_human_ = is_human;
}

void    Player::addCapture(int count) {
    capture_ += count;
}

// Ostream
std::ostream & operator<<(std::ostream & os, Player const & instance) {
    os << "Player " << instance.getChar() << " | Captures: " << instance.getCapture();
    return os;
}