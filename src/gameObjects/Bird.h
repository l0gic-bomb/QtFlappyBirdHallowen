#ifndef BIRD_H
#define BIRD_H

#include "common.h"

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPropertyAnimation>

class Bird : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(double rotation READ rotation WRITE setRotation)
    Q_PROPERTY(double y READ y WRITE setY)

public:
    Bird(const QPointF& position, const QPixmap& pixmap, const double& groundStartPosY,
         const int& scrWidth, const int& scrHeight, const double& scaleF);
    virtual ~Bird();

    //! Задаем поворот
    void setRotation(const double& angle) noexcept;
    //! Запуск полета
    void startOscillate();
    //! Окончание полета
    void stopOscillate();
    //! Взлет птички
    void rise();
    //! Падение
    void fall();

private:
    enum WingStates{
        up,
        middle,
        down
    };

    WingStates _wingState;

    bool _wingDirection;
    bool _oscillateDirection;

    double _currentRotation;

    QTimer* _birdTimer;
    QPropertyAnimation* _yAnimator;
    QPropertyAnimation* _oscillator;
    QPropertyAnimation* _rotator;

private slots:
    //! Слот имитирующий гравитацию
    void slGravitation();
    //! Слот для анимации крыльев
    void slDesignBird();
    //! Слот задания тракетории полета
    void slOscillate();

protected:
    double groundYPos;
    double scaleFactor;
    int screenWidth, screenHeight;

    //! Метод разворта птицы при снижении высоты
    void rotate(const double& end, const int& duration, const QEasingCurve& curve);
};

#endif // BIRD_H
