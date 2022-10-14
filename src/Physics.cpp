#include "Physics.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QRandomGenerator>

#include "View.h"

#include "gameObjects/Scene.h"


Physics::Physics(Game *parent_game, const int& tickRate, const bool& complexAnalyse, const bool& isOnlyGround,
                 const double& speedFactor, const bool& isCollisionDetectionDisabled)
 :  _speedfactor(speedFactor), collisionDetectionDisabled(isCollisionDetectionDisabled), game(parent_game), complexAnalysis(complexAnalyse),
   onlyGround(isOnlyGround), updateInterval(tickRate)
{
    _transform = new QTransform();

    bird = game->scene->bird;

    _pipeCriticX = pipeCriticXFinder(*(game->scene->pipe[0][0]));

    pipeMarkers[0] = 0;
    pipeMarkers[1] = 0;
    pipeMarkers[2] = 0;
    pipeMarkers[3] = 0;

    markers[0] = 0;
    markers[1] = 0;
    markers[2] = 0;

    game->scene->pipe[0][0]->setPos(game->getScreenWidth(), 0);
    game->scene->pipe[0][1]->setPos(game->getScreenWidth(), 0);
    game->scene->pipe[0][2]->setPos(game->getScreenWidth(), 0);
    game->scene->pipe[1][0]->setPos(game->getScreenWidth(), 0);
    game->scene->pipe[1][1]->setPos(game->getScreenWidth(), 0);
    game->scene->pipe[1][2]->setPos(game->getScreenWidth(), 0);

    _physicsTimer = new QTimer();

    QObject::connect(_physicsTimer, &QTimer::timeout, [this](){
        if (!onlyGround) {
            if (collisionCheck()) {
                bird->fall();
                game->gameOver();
                return;
            }
            movePipes();
        }
        moveGround();
        QApplication::processEvents();
    });

    _physicsTimer->setInterval(onlyGround ? updateInterval * 5 : updateInterval);
    _physicsTimer->start();
}

Physics::~Physics() noexcept
{
    delete _transform;
    delete _physicsTimer;
}

void Physics::stop()
{
    _physicsTimer->stop();
}

void Physics::start()
{
    _physicsTimer->start();
}

void Physics::moveGround()
{
    game->scene->updateGround();
    game->scene->ground->setPos(game->scene->ground->pos().x() - (((game->getScreenWidth()) * (1 / 350.0)) * _speedfactor),
                                game->scene->ground->pos().y());
}


int Physics::pipeCriticXFinder(const QGraphicsPixmapItem& item_pipe)
{
    int xVal = 0;
    QImage refPipe = item_pipe.pixmap().toImage();

    while (refPipe.pixel(xVal, refPipe.height() / 2) == 0)
        ++xVal;

    ++xVal;

    return xVal;
}

bool Physics::isOnlyGround() const noexcept
{
    return onlyGround;
}

bool Physics::collisionCheckComplex(const QGraphicsPixmapItem& item_pipe)
{
    _itemPipe.x = item_pipe.x();
    _itemPipe.y = item_pipe.y();
    _itemPipe.width = item_pipe.pixmap().width();
    _itemPipe.height = item_pipe.pixmap().height();

    *_transform = (game->graphicsView->transform());

    if ((bird->x() > _itemPipe.x) && (bird->x() < _itemPipe.x + _itemPipe.width)) {
        for (auto j = 0; j <= (_itemPipe.width); ++j) {
            if (((game->scene->itemAt(_itemPipe.x + j, _itemPipe.y, *_transform) == bird) ||
                (game->scene->itemAt(_itemPipe.x + j, _itemPipe.y + _itemPipe.height, *_transform))) &&
                    ((bird->pixmap().toImage().pixel(bird->mapFromScene(_itemPipe.x + j, _itemPipe.y).toPoint()) != 0) ||
                     (bird->pixmap().toImage().pixel(bird->mapFromScene(_itemPipe.x + j, _itemPipe.y + _itemPipe.height).toPoint()) != 0)))
                return true;
        }
    }

    for (auto i = 0; i <= (_itemPipe.height); ++i) {
        if((game->scene->itemAt(_itemPipe.x + _pipeCriticX, _itemPipe.y + i, *_transform) == bird) &&
                (bird->pixmap().toImage().pixel(bird->mapFromScene(_itemPipe.x + _pipeCriticX, _itemPipe.y + i).toPoint()) != 0))
            return true;
    }

    return false;
}

bool Physics::collisionCheck()
{
    bool result = true;

    if (collisionDetectionDisabled)
        result = false;

    birdRect.x = bird->x();
    birdRect.y = bird->y();
    birdRect.width  = bird->x() + bird->boundingRect().width();
    birdRect.height = bird->y() + bird->boundingRect().height();

    if (((birdRect.width > game->scene->pipe[1][1]->pos().x()) &&
        (birdRect.width < game->scene->pipe[1][1]->boundingRect().width() + game->scene->pipe[1][1]->pos().x())) &&
            (birdRect.height > game->scene->pipe[1][1]->pos().y() ||
             birdRect.height < game->scene->pipe[0][1]->pos().y() + game->scene->pipe[0][1]->boundingRect().height())) {
        if((complexAnalysis && (collisionCheckComplex(*game->scene->pipe[1][1]) ||
                                collisionCheckComplex(*game->scene->pipe[0][1]))) || !complexAnalysis)
            return result;
    } else if((birdRect.width >= game->scene->pipe[1][1]->pos().x() &&
             birdRect.x <= game->scene->pipe[1][1]->pos().x() + game->scene->pipe[1][1]->boundingRect().width()) &&
            (birdRect.height > game->scene->pipe[1][1]->pos().y() ||
             birdRect.y < game->scene->pipe[0][1]->pos().y() + game->scene->pipe[0][1]->boundingRect().height()))
        if((complexAnalysis && (collisionCheckComplex(*game->scene->pipe[1][1]) ||
                                collisionCheckComplex(*game->scene->pipe[0][1]))) || !complexAnalysis)
            return result;



    if (((birdRect.width > game->scene->pipe[1][2]->pos().x()) &&
        (birdRect.width < game->scene->pipe[1][2]->boundingRect().width() + game->scene->pipe[1][2]->pos().x())) &&
            (birdRect.height > game->scene->pipe[1][2]->pos().y() ||
             birdRect.height < game->scene->pipe[0][2]->pos().y() + game->scene->pipe[0][2]->boundingRect().height())) {
        if((complexAnalysis && (collisionCheckComplex(*game->scene->pipe[1][2]) ||
                                collisionCheckComplex(*game->scene->pipe[0][2]))) || !complexAnalysis)

            return result;

    } else if ((birdRect.width >= game->scene->pipe[1][2]->pos().x() &&
             birdRect.x <= game->scene->pipe[1][2]->pos().x() + game->scene->pipe[1][2]->boundingRect().width()) &&
            (birdRect.height > game->scene->pipe[1][2]->pos().y() ||
             birdRect.y < game->scene->pipe[0][2]->pos().y() + game->scene->pipe[0][2]->boundingRect().height())) {
        if((complexAnalysis && (collisionCheckComplex(*game->scene->pipe[1][2]) ||
                                collisionCheckComplex(*game->scene->pipe[0][2]))) || !complexAnalysis)
            return result;

    }

    if (((birdRect.width > game->scene->pipe[1][0]->pos().x()) &&
        (birdRect.width < game->scene->pipe[1][0]->boundingRect().width() + game->scene->pipe[1][0]->pos().x())) &&
            (birdRect.height > game->scene->pipe[1][0]->pos().y() ||
             birdRect.height < game->scene->pipe[0][0]->pos().y() + game->scene->pipe[0][0]->boundingRect().height())) {
        if ((complexAnalysis && (collisionCheckComplex(*game->scene->pipe[1][0]) ||
                                collisionCheckComplex(*game->scene->pipe[0][0]))) || !complexAnalysis)
            return result;

    } else if ((birdRect.width >= game->scene->pipe[1][0]->pos().x() &&
             birdRect.x <= game->scene->pipe[1][0]->pos().x() + game->scene->pipe[1][0]->boundingRect().width()) &&
            (birdRect.height > game->scene->pipe[1][0]->pos().y() ||
             birdRect.y < game->scene->pipe[0][0]->pos().y() + game->scene->pipe[0][0]->boundingRect().height()))
        if ((complexAnalysis && (collisionCheckComplex(*game->scene->pipe[1][0]) ||
                                collisionCheckComplex(*game->scene->pipe[0][0]))) || !complexAnalysis)
            return result;



    if ((birdRect.y < -bird->boundingRect().height()) || (birdRect.height > game->scene->ground->pos().y()))
        return result;


    if ((birdRect.x >= game->scene->pipe[1][0]->pos().x() + game->scene->pipe[1][0]->boundingRect().width()) &&
            markers[0] == 0 && (birdRect.width < game->scene->pipe[1][1]->pos().x())) {
        game->updateScore();
        markers[0] = 1;
        markers[2] = 0;
        game->birdClosestPipe = 2;
    } else if((birdRect.x >= game->scene->pipe[1][1]->pos().x() + game->scene->pipe[1][1]->boundingRect().width()) &&
            markers[1] == 0 && (birdRect.width < game->scene->pipe[1][2]->pos().x())) {
        game->updateScore();
        markers[0] = 0;
        markers[1] = 1;
        game->birdClosestPipe = 0;
    } else if((birdRect.x >= game->scene->pipe[1][2]->pos().x() + game->scene->pipe[1][2]->boundingRect().width()) &&
            markers[2] == 0 && (birdRect.width < game->scene->pipe[1][0]->pos().x())) {
        game->updateScore();
        markers[1] = 0;
        markers[2] = 1;
        game->birdClosestPipe = 1;
    }

    return false;
}

int Physics::randInt(const int& low, const int& high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(*QRandomGenerator::global());
}

void Physics::movePipes()
{
    if ((game->scene->pipe[1][1]->pos().x() >= game->getScreenWidth()) && (pipeMarkers[0] == 0)) {
        if (pipeMarkers[1])
            game->scene->pipe[1][1]->setPos(game->getScreenWidth() + (game->scene->pipe[1][1]->boundingRect().width()) + (game->getScreenWidth() / 2.6),
                    randInt(game->getScreenHeight() / 3.7,game->getScreenHeight() / 1.4));
        else
            game->scene->pipe[1][1]->setPos(game->getScreenWidth() + (game->scene->pipe[1][1]->boundingRect().width()),
                    randInt(game->getScreenHeight() / 3.5,
                            game->getScreenHeight() / 1.4));

        game->scene->pipe[0][1]->setPos(game->scene->pipe[1][1]->pos().x(),
                game->scene->pipe[1][1]->pos().y() - (game->getScreenHeight() / 5.05) - game->scene->pipe[1][1]->boundingRect().height());
        pipeMarkers[0] = 1;
    }

    if ((game->scene->pipe[1][0]->pos().x() >= game->getScreenWidth()) && (pipeMarkers[2] == 0)) {
        if (pipeMarkers[1])
            game->scene->pipe[1][0]->setPos(game->getScreenWidth() + game->scene->pipe[1][0]->boundingRect().width() + (game->getScreenWidth() / 2.6),
                    randInt(game->getScreenHeight()/3.7,game->getScreenHeight() / 1.4));
        else
            game->scene->pipe[1][0]->setPos(game->scene->pipe[1][1]->pos().x() + (game->getScreenWidth() / 2.6) + (game->scene->pipe[1][0]->boundingRect().width()),
                    randInt(game->getScreenHeight() / 3.7,game->getScreenHeight() / 1.4));

        game->scene->pipe[0][0]->setPos(game->scene->pipe[1][0]->pos().x(),game->scene->pipe[1][0]->pos().y() - (game->getScreenHeight() / 5.05) - game->scene->pipe[1][0]->boundingRect().height());
        pipeMarkers[2] = 1;
    }
    if (game->scene->pipe[1][2]->pos().x() >= game->getScreenWidth() && (pipeMarkers[3] == 0)) {
        if (pipeMarkers[1])
            game->scene->pipe[1][2]->setPos(game->getScreenWidth() + game->scene->pipe[1][2]->boundingRect().width() + (game->getScreenWidth() / 2.6),
                    randInt(game->getScreenHeight() / 3.7,game->getScreenHeight() / 1.4));

        else
            game->scene->pipe[1][2]->setPos(game->scene->pipe[1][0]->pos().x() + (game->getScreenWidth() / 2.6) + (game->scene->pipe[1][2]->boundingRect().width()),
                    randInt(game->getScreenHeight() / 3.7,game->getScreenHeight() / 1.4));

        game->scene->pipe[0][2]->setPos(game->scene->pipe[1][2]->pos().x(),game->scene->pipe[1][2]->pos().y() - (game->getScreenHeight() / 5.05) - game->scene->pipe[0][2]->boundingRect().height());
        pipeMarkers[3] = 1;
    }

    auto distance = game->getScreenWidth() * (1 / 350.0);
    game->scene->pipe[1][1]->setPos(game->scene->pipe[1][1]->pos().x() - (distance * _speedfactor), game->scene->pipe[1][1]->y());
    game->scene->pipe[0][1]->setPos(game->scene->pipe[0][1]->pos().x() - (distance * _speedfactor), game->scene->pipe[0][1]->y());
    game->scene->pipe[1][0]->setPos(game->scene->pipe[1][0]->pos().x() - (distance * _speedfactor), game->scene->pipe[1][0]->y());
    game->scene->pipe[0][0]->setPos(game->scene->pipe[0][0]->pos().x() - (distance * _speedfactor), game->scene->pipe[0][0]->y());
    game->scene->pipe[1][2]->setPos(game->scene->pipe[1][2]->pos().x() - (distance * _speedfactor), game->scene->pipe[1][2]->y());
    game->scene->pipe[0][2]->setPos(game->scene->pipe[0][2]->pos().x() - (distance * _speedfactor), game->scene->pipe[0][2]->y());

    if (game->scene->pipe[1][1]->pos().x() < -game->scene->pipe[1][1]->boundingRect().width()) {
        pipeMarkers[0] = 0;
        pipeMarkers[1] = 1;
        game->scene->pipe[1][1]->setPos(game->getScreenWidth(), 0);
    }
    if (game->scene->pipe[1][0]->pos().x() < -game->scene->pipe[1][0]->boundingRect().width()) {
        pipeMarkers[2] = 0;
        pipeMarkers[1] = 1;
        game->scene->pipe[1][0]->setPos(game->getScreenWidth(), 0);
    }
    if (game->scene->pipe[1][2]->pos().x() < -game->scene->pipe[1][2]->boundingRect().width()) {
        pipeMarkers[3] = 0;
        pipeMarkers[1] = 1;
        game->scene->pipe[1][2]->setPos(game->getScreenWidth(), 0);
    }
}

void Physics::switchOnlyGroundMove()
{
    _physicsTimer->setInterval(onlyGround ? updateInterval : updateInterval * 5);
    onlyGround = !onlyGround;
}
