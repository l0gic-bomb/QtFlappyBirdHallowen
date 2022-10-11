TEMPLATE = app
QT += core gui widgets

CONFIG += c++17
INCLUDEPATH += $$PWD/src

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

win32:RC_ICONS += res/icon_bird.ico

SOURCES += \
    src/MainWindow.cpp \
    src/main.cpp

HEADERS += \
    src/MainWindow.h

RESOURCES += \
    res/Resource.qrc

