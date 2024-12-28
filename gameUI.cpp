
#include "gameUI.h"
#include <QVBoxLayout>
#include <QLabel>

GameUI::GameUI(QWidget *parent)
    : QWidget(parent), size(5), colorX(Qt::red), colorO(Qt::blue), isAIEnabled(false) {

    playerX = new Player('X', colorX);
    playerO = new Player('O', colorO);
    currentPlayer = playerX;

    buttons = nullptr;

    setupUI();
}

void GameUI::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Выберите размер поля:");
    sizeComboBox = new QComboBox();
    sizeComboBox->addItem("5x5", 5);
    sizeComboBox->addItem("10x10", 10);
    connect(sizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index){
                size = sizeComboBox->itemData(index).toInt();
                startNewGame();
            });


    QLabel *colorLabelX = new QLabel("Выберите цвет для игрока X:");
    colorComboBoxX = new QComboBox();
    colorComboBoxX->addItem("Красный", QColor(Qt::red));
    colorComboBoxX->addItem("Зеленый", QColor(Qt::green));
    colorComboBoxX->addItem("Синий", QColor(Qt::blue));
    connect(colorComboBoxX, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index){
                colorX = colorComboBoxX->itemData(index).value<QColor>();
                playerX->setColor(colorX);
            });


    QLabel *colorLabelO = new QLabel("Выберите цвет для игрока O:");
    colorComboBoxO = new QComboBox();
    colorComboBoxO->addItem("Синий", QColor(Qt::blue));
    colorComboBoxO->addItem("Оранжевый", QColor(255, 165, 0));
    colorComboBoxO->addItem("Фиолетовый", QColor(148, 0, 211));
    connect(colorComboBoxO, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index){
                colorO = colorComboBoxO->itemData(index).value<QColor>();
                playerO->setColor(colorO);
            });


    QLabel *aiLabel = new QLabel("Выберите глубину ИИ:");
    aiDepthComboBox = new QComboBox();
    aiDepthComboBox->addItem("1", 1);
    aiDepthComboBox->addItem("3", 3);
    aiDepthComboBox->addItem("5", 5);

    pvpRadioButton = new QRadioButton("Игрок против игрока");
    aiRadioButton = new QRadioButton("Игрок против ИИ");
    pvpRadioButton->setChecked(true);

    connect(pvpRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        isAIEnabled = !checked;
    });

    connect(aiRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        isAIEnabled = checked;
    });

    QPushButton *newGameButton = new QPushButton("Начать новую игру");
    connect(newGameButton, &QPushButton::clicked, this, &GameUI::startNewGame);

    mainLayout->addWidget(label);
    mainLayout->addWidget(sizeComboBox);
    mainLayout->addWidget(colorLabelX);
    mainLayout->addWidget(colorComboBoxX);
    mainLayout->addWidget(colorLabelO);
    mainLayout->addWidget(colorComboBoxO);
    mainLayout->addWidget(aiLabel);
    mainLayout->addWidget(aiDepthComboBox);
    mainLayout->addWidget(pvpRadioButton);
    mainLayout->addWidget(aiRadioButton);
    mainLayout->addWidget(newGameButton);

    setLayout(mainLayout);
}

void GameUI::startNewGame() {

    if (buttons) {
        for (int i = 0; i < size; ++i) {
            delete[] buttons[i];
        }
        delete[] buttons;
    }


    game = new Game(size);


    buttons = new QPushButton**[size];
    for (int i = 0; i < size; ++i) {
        buttons[i] = new QPushButton*[size];
    }


    QLayout* currentLayout = layout();
    if (currentLayout) {
        QLayoutItem* item;
        while ((item = currentLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete currentLayout;
    }


    QGridLayout* layout = new QGridLayout(this);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            buttons[i][j] = new QPushButton(" ");
            buttons[i][j]->setFixedSize(50, 50);
            connect(buttons[i][j], &QPushButton::clicked, this, [=]() { handleCellClick(i, j); });
            layout->addWidget(buttons[i][j], i, j);
        }
    }

    QPushButton *resetButton = new QPushButton("Сбросить игру");
    connect(resetButton, &QPushButton::clicked, this, &GameUI::resetGame);
    layout->addWidget(resetButton, size, 0, 1, size); // Кнопка сброса

    setLayout(layout); // Устанавливаем новый layout
}

void GameUI::handleCellClick(int x, int y) {
    if (game->makeMove(x, y, currentPlayer->getSymbol())) {
        buttons[x][y]->setText(QString(currentPlayer->getSymbol()));
        buttons[x][y]->setStyleSheet("background-color: " + currentPlayer->getColor().name() + ";");

        if (game->checkWin(currentPlayer->getSymbol())) {
            QMessageBox::information(this, "Победа!", QString("Игрок %1 победил!").arg(currentPlayer->getSymbol()));
            resetGame();
            return;
        }


        currentPlayer = (currentPlayer == playerX) ? playerO : playerX;

        if(isAIEnabled) {
            int depth = aiDepthComboBox->currentData().toInt();
            auto bestMove = game->getBestAIMove(depth, currentPlayer->getSymbol());
            if(bestMove.first != -1) {
                if (game->makeMove(bestMove.first, bestMove.second, currentPlayer->getSymbol())) {
                    buttons[bestMove.first][bestMove.second]->setText(QString(currentPlayer->getSymbol()));
                    buttons[bestMove.first][bestMove.second]->setStyleSheet("background-color: " + currentPlayer->getColor().name() + ";");

                    if (game->checkWin(currentPlayer->getSymbol())) {
                        QMessageBox::information(this, "Победа!", QString("Игрок %1 победил!").arg(currentPlayer->getSymbol()));
                        resetGame();
                        return;
                    }
                    currentPlayer = (currentPlayer == playerX) ? playerO : playerX;
                }
            }
        }
    }
}

void GameUI::resetGame() {
    game->reset();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            buttons[i][j]->setText(" ");
            buttons[i][j]->setStyleSheet("");
        }
    }
    currentPlayer = playerX;
}

GameUI::~GameUI() {
    for (int i = 0; i < size; ++i) {
        delete[] buttons[i];
    }
    delete[] buttons;
    delete game;
    delete playerX;
    delete playerO;
}

