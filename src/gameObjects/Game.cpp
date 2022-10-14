#include "Game.h"

#include <QTimer>
#include <QImage>

#include "gameObjects/Bird.h"
#include "Button.h"
#include "gameObjects/Scene.h"
#include "Physics.h"
#include "common.h"

Game::Game(View *GraphicsView, QSettings* const cfg, const int& windowWidth, const int& windowHeight)
    : graphicsView(GraphicsView), _screenWidth(windowWidth), _screenHeight(windowHeight), config(cfg)
{
    loadConfiguration();

    _scoreRecord = 0;
    _score = 0;
    birdClosestPipe = 1;

    _gameFinished = false;
    _gameStarted  = false;
    _gameActuallyStarted = false;

    _scaleFactor = DEFAULT_SCALEFACTOR;

    scene.reset(new Scene(this, QRectF(0, 0, _screenWidth, _screenHeight)));
    physics.reset(new Physics(this, _physicsTickRate, _physicsComplexAnalysis, true, _physicsSpeedFactor, _physicsDisableCollisionDetection));
}

Game::~Game() noexcept
{
}

void Game::loadConfiguration()
{
    _scaleFactor = config->value("ScaleFactor", _scaleFactor).toDouble();

    // Задаем настройки игры
    config->beginGroup("Physics");
    _physicsTickRate = config->value("ComplexAnalyse", 7.5).toInt();
    _physicsComplexAnalysis = config->value("TickRate", true).toBool();
    _physicsSpeedFactor = config->value("SpeedFactor", 1).toDouble();
    _physicsDisableCollisionDetection = config->value("DisableCollisionDetection", false).toBool();
    config->endGroup();
}

int Game::getScore() const noexcept
{
    return _score;
}

int Game::getScoreRecord() const noexcept
{
    return _scoreRecord;
}

int Game::getScreenHeight() const noexcept
{
    return _screenHeight;
}

int Game::getScreenWidth() const noexcept
{
    return _screenWidth;
}

qreal Game::getScaleFactor() const noexcept
{
    return _scaleFactor;
}

void Game::clickEvent()
{
    if (_gameActuallyStarted) {
        if (!isGameStarted()) {

            scene->bird->stopOscillate();
            scene->fadeGroup(GROUP_NEWROUND, false, 100);

            if (physics)
                physics->switchOnlyGroundMove();

            _gameStarted = true;
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

    QTimer::singleShot(550, [this]() {
        graphicsView->setBackgroundBrush(QBrush(QPixmap(BACKGROUND_NIGHT).scaled(_screenWidth, _screenHeight)));

        scene->fadeGroup(GROUP_NEWROUND, true, 1, GROUP_ROUNDEND);

        physics.reset(new Physics(this, _physicsTickRate, _physicsComplexAnalysis, false, _physicsSpeedFactor, _physicsDisableCollisionDetection));

        _score = -1;
        updateScore();

        _gameStarted = false;

        scene->bird->setPos(scene->bird->boundingRect().width() * 2.75, POS_Y_LOGO(_screenHeight) + QPixmap(BIRD_UP).height() * 5);

        prepareGame();

        scene->bird->setRotation(0);

        physics->start();
        physics->switchOnlyGroundMove();
    });
}

void Game::prepareGame()
{
    scene->bird->startOscillate();

    _gameFinished = false;
    _gameActuallyStarted = true;
}

void Game::gameOver()
{
    physics->stop();

    if (_score > _scoreRecord)
        _scoreRecord = _score;

    scene->gameOver(_score, _scoreRecord);

    _gameFinished = true;
    _gameActuallyStarted = false;
    birdClosestPipe = 1;
}

bool Game::isGameFinished() const noexcept
{
    return _gameFinished;
}

bool Game::isGameStarted() const noexcept
{
    return _gameStarted;
}

bool Game::isGameActuallyStarted() const noexcept
{
    return _gameActuallyStarted;
}

void Game::updateScore()
{
    ++_score;
    scene->updateScore(_score);
}
