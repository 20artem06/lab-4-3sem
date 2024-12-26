#include "player.h"

Player::Player(char symbol, const QColor& color) : symbol(symbol), color(color) {}

char Player::getSymbol() const {
    return symbol;
}

QColor Player::getColor() const {
    return color;
}

void Player::setColor(const QColor& color) {
    this->color = color;
}
