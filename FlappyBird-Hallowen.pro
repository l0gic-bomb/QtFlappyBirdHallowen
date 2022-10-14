TEMPLATE = app
QT += core gui widgets

CONFIG += c++14
INCLUDEPATH += $$PWD/src

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_ICONS += res/icon_bird.ico

SOURCES += \
    src/MainWindow.cpp \
    src/gameObjects/Bird.cpp \
    src/Button.cpp \
    src/gameObjects/Game.cpp \
    src/Physics.cpp \
    src/gameObjects/Scene.cpp \
    src/main.cpp \
    src/View.cpp

HEADERS += \
    src/MainWindow.h \
    src/gameObjects/Bird.h \
    src/Button.h \
    src/gameObjects/Game.h \
    src/Physics.h \
    src/gameObjects/Scene.h \
    src/common.h \
    src/View.h

RESOURCES += \
    res/Resource.qrc

