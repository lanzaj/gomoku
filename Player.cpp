#include "Player.hpp"

Player::Player(char c) : capture_(0), c_(c) {}

Player::~Player() {}

// Copy constructor
Player::Player(Player const & src) : capture_(src.capture_), c_(src.c_) {}

// Copy assignment
Player & Player::operator=(Player const & rhs) {
    if (this != &rhs) {
        this->capture_ = rhs.capture_;
        this->c_ = rhs.c_;
    }
    return *this;
}

// Getter
int Player::getCapture() const {
    return capture_;
}

char Player::getChar() const {
    return c_;
}

// Setter
void Player::addCapture(int count) {
    capture_ += count;
}

// Ostream
std::ostream & operator<<(std::ostream & os, Player const & instance) {
    os << "Player " << instance.getChar() << " | Captures: " << instance.getCapture();
    return os;
}