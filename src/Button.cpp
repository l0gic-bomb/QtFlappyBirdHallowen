#include "Button.h"

#include <QMessageBox>

#include "gameObjects/Scene.h"

void ButtonFunctions::aboutAction(Game* instance)
{
    QWidget *gameQObject = dynamic_cast<QWidget *>(instance);
    QMessageBox::about(gameQObject, "Об игре", QString("Клон игры Flappy Bird - Flappy Bird Hallowen. Ссылка на исходный код https://github.com/l0gic-bomb"));
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
    bool toggleStatus = (pixmap().data_ptr() == firstPixmap.data_ptr());
    if (parent == nullptr) {
        if (toggle && !toggleStatus)
            secondFunction(game);
        else
            firstFunction(game);
    } else
        if (toggle && !toggleStatus)
            secondFunction(parent);
        else
            firstFunction(parent);

    if (toggle)
        setPixmap(toggleStatus ? secondPixmap : firstPixmap);
}
