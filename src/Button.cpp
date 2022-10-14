#include "Button.h"

#include <QMessageBox>

#include "gameObjects/Scene.h"

void ButtonFunctions::aboutAction(Game* instance)
{
    QWidget *gameQObject = dynamic_cast<QWidget *>(instance);
    QMessageBox::about(gameQObject, "Об игре", QString("Flappy Bird Hallowen от https://github.com/l0gic-bomb"));
}

void ButtonFunctions::playAction(Game* instance)
{
    if(!instance->isGameFinished()) {
         instance->prepareGame();
         instance->scene->fadeGroup(GROUP_FIRSTSCREEN, false, 500, GROUP_GAMEPLAY);
    } else
        instance->prepareNewRound();
}

void Button::invoke(Game* parent)
{
    bool toggleStatus = (pixmap().data_ptr() == _firstPixmap.data_ptr());
    if (parent == nullptr) {
        if (_toggle && !toggleStatus)
            secondFunction(game);
        else
            firstFunction(game);
    } else
        if (_toggle && !toggleStatus)
            secondFunction(parent);
        else
            firstFunction(parent);

    if (_toggle)
        setPixmap(toggleStatus ? _secondPixmap : _firstPixmap);
}
