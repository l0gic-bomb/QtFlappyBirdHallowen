#include "Game.h"

#include <QTimer>
#include <QImage>

#include "gameObjects/Bird.h"
#include "Button.h"
#include "gameObjects/Scene.h"
#include "Physics.h"
#include "common.h"

Game::Game(View *GraphicsView, QSettings* const cfg, const int& windowWidth, const int& windowHeight)
    : graphicsView(GraphicsView), screenWidth(windowWidth), screenHeight(windowHeight), config(cfg)
{
    loadConfiguration();

    scoreRecord = 0;
    score = 0;
    birdClosestPipe = 1;

    gameFinished = false;
    gameStarted  = false;
    gameActuallyStarted = false;

    scaleFactor = DEFAULT_SCALEFACTOR;

    scene.reset(new Scene(this, QRectF(0, 0, screenWidth, screenHeight)));
    physics.reset(new Physics(this, physicsTickRate, physicsComplexAnalysis, true, physicsSpeedFactor, physicsDisableCollisionDetection));
}

Game::~Game() noexcept
{
}

void Game::loadConfiguration()
{
    scaleFactor = config->value("ScaleFactor", scaleFactor).toDouble();

    // Задаем настройки игры
    config->beginGroup("Physics");
    physicsTickRate = config->value("ComplexAnalyse", 7.5).toInt();
    physicsComplexAnalysis = config->value("TickRate", true).toBool();
    physicsSpeedFactor = config->value("SpeedFactor", 1).toDouble();
    physicsDisableCollisionDetection = config->value("DisableCollisionDetection", false).toBool();
    config->endGroup();
}

int Game::getScore() const noexcept
{
    return score;
}

int Game::getScoreRecord() const noexcept
{
    return scoreRecord;
}

int Game::getScreenHeight() const noexcept
{
    return screenHeight;
}

int Game::getScreenWidth() const noexcept
{
    return screenWidth;
}

qreal Game::getScaleFactor() const noexcept
{
    return scaleFactor;
}

void Game::clickEvent()
{
    if (gameActuallyStarted) {
        if (!isGameStarted()) {

            scene->bird->stopOscillate();
            scene->fadeGroup(GROUP_NEWROUND, false, 100);

            if (physics)
                physics->switchOnlyGroundMove();

            gameStarted = true;
        }

        if (scene->isGroupVisible(GROUP_NEWROUND))
            scene->fadeGroup(GROUP_NEWROUND, false, 5);
        scene->bird->rise();

        return;
    }
}

void Game::prepareNewRound()
{
    scene->flash(Qt::black, 500, QEasingCurve::Linear);

    QTimer::singleShot(550, [&]() {
        graphicsView->setBackgroundBrush(QBrush(QPixmap(":/graphics/background_night.png").scaled(screenWidth, screenHeight)));

        scene->fadeGroup(GROUP_NEWROUND, true, 1, GROUP_ROUNDEND);

        physics.reset(new Physics(this, physicsTickRate, physicsComplexAnalysis, false, physicsSpeedFactor, physicsDisableCollisionDetection));

        score = -1;
        updateScore();

        gameStarted = false;

        scene->bird->setPos(scene->bird->boundingRect().width() * 2.75, POS_Y_LOGO(screenHeight) + QPixmap(":/graphics/bird_up.png").height() * 5);

        prepareGame();

        scene->bird->setRotation(0);

        physics->start();
        physics->switchOnlyGroundMove();
    });
}

void Game::prepareGame()
{
    scene->bird->startOscillate();

    gameFinished = false;
    gameActuallyStarted = true;
}

void Game::gameOver()
{
    physics->stop();

    if (score > scoreRecord)
        scoreRecord = score;

    scene->gameOver(score, scoreRecord);

    gameFinished = true;
    gameActuallyStarted = false;
    birdClosestPipe = 1;
}

bool Game::isGameFinished() const noexcept
{
    return gameFinished;
}

bool Game::isGameStarted() const noexcept
{
    return gameStarted;
}

bool Game::isGameActuallyStarted() const noexcept
{
    return gameActuallyStarted;
}

void Game::updateScore()
{
    ++score;
    scene->updateScore(score);
}
