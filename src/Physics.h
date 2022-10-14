#ifndef PHYSICS_H
#define PHYSICS_H

#include <QTimer>
#include <QTransform>

#include "common.h"

#include "gameObjects/Game.h"
#include "gameObjects/Bird.h"

struct Rectangle {
    double x;
    double y;
    double width;
    double height;
};

class Physics
{
public:
    Physics(Game *parent_game, const int& tickRate = 5, const bool& complexAnalyse = 0, const bool& isOnlyGround = 0,
            const double& speedFactor = 1.0, const bool& isCollisionDetectionDisabled = 0);
    virtual ~Physics() noexcept;

    //! Запускаем таймер
    void start();
    //! Останавливаем таймер
    void stop();
    //! Переключчатель
    void switchOnlyGroundMove();
    bool isOnlyGround() const noexcept;
    //! СЛучайное число из диапозона
    static int randInt(const int& low, const int& high) ;

private:
    int _pipeCriticX;
    double _speedfactor;

    //! Перемещение земил
    void moveGround();
    //! перемещение пайпов
    void movePipes();

    bool collisionDetectionDisabled;
    bool pipeMarkers[4];
    bool markers[3];

    Rectangle birdRect;
    Rectangle _itemPipe;

    QTimer* _physicsTimer;

    static int pipeCriticXFinder(const class QGraphicsPixmapItem& item_pipe);

    QTransform* _transform;

protected:
    Game *game;

    Bird* bird;

    bool complexAnalysis;
    bool onlyGround;

    int updateInterval;

    //!  Обработка столконвений
    virtual bool collisionCheck();
    virtual bool collisionCheckComplex(const class QGraphicsPixmapItem& item_pipe);
};

#endif // PHYSICS_H
