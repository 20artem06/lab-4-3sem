#ifndef GAMEUI_H
#define GAMEUI_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QComboBox>
#include "game.h"
#include "Player.h"

class GameUI : public QWidget {
    Q_OBJECT

public:
    GameUI(QWidget *parent = nullptr);
    ~GameUI();
    void setupUI();

private slots:
    void handleCellClick(int x, int y);
    void resetGame();
    void startNewGame();

private:
    Game *game;
    Player *playerX;
    Player *playerO;
    Player *currentPlayer;
    QPushButton ***buttons;
    int size;
    QColor colorX;
    QColor colorO;
    QComboBox *sizeComboBox;
    QComboBox *colorComboBoxX;
    QComboBox *colorComboBoxO;
};

#endif // GAMEUI_H
