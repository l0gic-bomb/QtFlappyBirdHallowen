#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>

#include "common.h"
#include "gameObjects/Game.h"

struct ButtonFunctions
{
    static void aboutAction(Game* instance);
    static void playAction(Game* instance);
};

class Button : public QGraphicsPixmapItem
{
    typedef void (*Function)(Game* instance);

public:
    Button(Game* parent_game, const QPixmap& first, Function firstFn, bool toggleMode = false, const QPixmap& second = QPixmap(), Function secondFn = nullptr)
        : QGraphicsPixmapItem(first), firstFunction(firstFn), secondFunction(secondFn), game(parent_game), _firstPixmap(first), _secondPixmap(second), _toggle(toggleMode) {}

    virtual void invoke(Game* parent = nullptr);

protected:
    Function firstFunction;
    Function secondFunction;
    Game* game;

private:
    QPixmap _firstPixmap;
    QPixmap _secondPixmap;
    bool _toggle;
};

#endif // BUTTON_H
