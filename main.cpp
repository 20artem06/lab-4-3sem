#include <QApplication>
#include "gameUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GameUI window;
    window.setWindowTitle("Крестики-нолики");
    window.show();
    return app.exec();
}
