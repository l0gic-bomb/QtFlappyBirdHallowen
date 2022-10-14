#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPainter>
#include <QList>

#include "common.h"
#include "Button.h"
#include "View.h"

#include "gameObjects/Game.h"
#include "gameObjects/Bird.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(Game* parent, const QRectF& rect);
    virtual ~Scene();

    QGraphicsPixmapItem* pipe[2][3];
    std::unique_ptr<QGraphicsPixmapItem> ground;

    QGraphicsItem* groupItem[8][GROUP_MAX_ITEM_COUNT];

    Bird* bird;

    //! Вспыкша при смерти и переприсовка элементов
    void flash(const QColor& color, const int& duration, const QEasingCurve& curve);
    //! Обновление элементов в зависимости от группы
    void fadeGroup(const int& groupIndex, const bool& show, const int& duration, const int& group2Index = -1);
    //! Обновляем окошко при проигрыше
    void gameOver(const int& score, const int& scoreRecord);
    //! Перерисовка земли
    void updateGround();
    //! Обновление отображения счета
    void updateScore(const int& score);
    //! Проверка элемента на видимость
    bool isGroupVisible(const int& groupIndex) const noexcept;

private:

    QPainter *groundPainter;

    QPixmap _bigZero;
    QPixmap _smallZero;

    QImage _mainScore;
    QImage _endScore;
    QImage _groundImage;

    bool _flashStatus;
    bool _isClickAvailable[2];

    QGraphicsOpacityEffect*  opacityEffect;
    QPropertyAnimation*  opacityAnim;

    QGraphicsOpacityEffect** opacityEffectGroup[2];
    QPropertyAnimation** opacityAnimGroup[2];

//    void pixmapScale(QPixmap* pixmap, const double& scale);

protected:
    Game* game;

    QList<Button*> _buttons;
    Button* btnPlay;
    Button* btnAbout;

    QGraphicsPixmapItem* itemGameOver;
    QGraphicsPixmapItem* itemScoreBoard;
    QGraphicsPixmapItem* itemEndScore;
    QGraphicsPixmapItem* itemEndScoreRecord;
    QGraphicsPixmapItem* itemLogo;
    QGraphicsPixmapItem* itemGameInfo;
    QGraphicsPixmapItem* itemScore;
    QGraphicsPixmapItem* itemFlash;

    //! Методы для получения картинок счетов
    QPixmap makeMainScore(const int& score);
    QPixmap makeEndingScore(const int& score, int* pos);

    //! Переопределение метода QGraphicsScene::mousePressEvent
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //! Переопределение метода QGraphicsScene::mouseDoubleClickEvent
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    //! Переопределение метода QGraphicsScene::keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // SCENE_H
