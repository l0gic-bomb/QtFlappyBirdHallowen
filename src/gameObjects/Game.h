#ifndef GAME_H
#define GAME_H

#include <QSettings>

#include "View.h"

class Physics;
class Scene;

class Game
{
public:
    Game(View *GraphicsView, QSettings* const cfg, const int& windowWidth, const int& windowHeight);
    virtual ~Game() noexcept;

   //! Событие клика
   void clickEvent();
   //! Обновление текущего рекорда
   void updateScore();
   //! Игра закончена
   void gameOver();
   //! Подготовка перед первой игрой
   void prepareGame();
   //! Подготовка нового раунда
   void prepareNewRound();

   //! Метод возвращает bool игра закончена или нет
   bool isGameFinished() const noexcept;
   //! Проверяем игра начата
   bool isGameStarted() const noexcept;
   //! Проверка играем ли прямо сейчас
   bool isGameActuallyStarted() const noexcept;

   //! Получаем текущее количество очков
   int getScore() const noexcept;
   //! Получаем рекорд
   int getScoreRecord() const noexcept;
   //! Получаем ширину экрана
   int getScreenWidth() const noexcept;
   //! Получаем высоту экрана
   int getScreenHeight() const noexcept;
   //! Возвращает масштаб
   double getScaleFactor() const noexcept;

   int birdClosestPipe;
   std::unique_ptr<View> graphicsView;
   std::unique_ptr<Scene> scene;

private:
    bool _gameFinished;
    bool _gameStarted;
    bool _gameActuallyStarted;
    bool _physicsComplexAnalysis;
    bool _physicsDisableCollisionDetection;

    int _score;
    int _scoreRecord;
    int _screenWidth;
    int _screenHeight;
    int _physicsTickRate;

    double _physicsSpeedFactor;
    double _scaleFactor;

    // Загрузка данных из конфига (ЕСЛИ ЕСТЬ!!!)
    void loadConfiguration();

protected:
    std::unique_ptr<QSettings> config;
    std::unique_ptr<Physics> physics;
};

#endif // GAME_H
