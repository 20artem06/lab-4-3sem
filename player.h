#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>

class Player {
public:
    Player(char symbol, const QColor& color);
    char getSymbol() const;
    QColor getColor() const;
    void setColor(const QColor& color);

private:
    char symbol;
    QColor color;
};

#endif // PLAYER_H
