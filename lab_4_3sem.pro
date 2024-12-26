QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    game.cpp \
    gameGraph.cpp \
    gameUI.cpp \
    main.cpp \
    mainTests.cpp \
    player.cpp \
    testSuite.cpp \
    utils.cpp

HEADERS += \
    board.h \
    game.h \
    gameGraph.h \
    gameUI.h \
    player.h \
    testSuite.h \
    utils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
