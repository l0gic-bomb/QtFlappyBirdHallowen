#include "Bird.h"

Bird::Bird(const QPointF& position, const QPixmap& pixmap, const double& groundStartPosY,
           const int& scrWidth, const int& scrHeight, const double& scaleF)
    : QGraphicsPixmapItem(pixmap), groundYPos(groundStartPosY), scaleFactor(scaleF), screenWidth(scrWidth), screenHeight(scrHeight)
{
    setCacheMode(ItemCoordinateCache);

    setPos(position);

    _birdTimer = new QTimer(this);

    _rotator = new QPropertyAnimation(this, "rotation", this);
    _yAnimator = new QPropertyAnimation(this, "y", this);
    _oscillator = new QPropertyAnimation(this, "pos", this);

    _oscillator->setDuration(550);
    _oscillator->setEndValue(QPointF(boundingRect().width() * 2.75, y()));
    _oscillator->setEasingCurve(QEasingCurve::OutQuad);

    _wingState = WingStates::up;

    _birdTimer->start(75);
    _currentRotation = 0;

    _oscillateDirection = true;

    connect(_birdTimer, &QTimer::timeout,              this, &Bird::slDesignBird);
    connect(_yAnimator, &QAbstractAnimation::finished, this, &Bird::slGravitation);
    connect(_oscillator,&QAbstractAnimation::finished, this, &Bird::slOscillate);
}

Bird::~Bird()
{
    delete _rotator;
    delete _yAnimator;
    delete _oscillator;
}

void Bird::slOscillate()
{
    _oscillator->setEasingCurve(QEasingCurve::OutCubic);

    if (_oscillateDirection) {
        _oscillator->setEndValue(QPointF(x(), y() + screenHeight / 40));
        _oscillateDirection = false;
    } else {
        _oscillator->setEndValue(QPointF(x(), y() - screenHeight / 40));
        _oscillateDirection = true;
    }
    startOscillate();
}

void Bird::slDesignBird()
{
    QPixmap design;

    if (_wingState == WingStates::middle) {
        if (_wingDirection) {
            design.load(":/graphics/bird_down.png");
            _wingState = WingStates::down;
            _wingDirection = false;
        } else {
            design.load(":/graphics/bird_up.png");
            _wingState = WingStates::up;
            _wingDirection = true;
        }
    } else {
        design.load(":/graphics/bird_middle.png");
        _wingState = WingStates::middle;
    }
    setPixmap(design.scaled(design.size() * scaleFactor));
}

void Bird::slGravitation()
{
    _birdTimer->setInterval(100);

    rotate(120, 700, QEasingCurve::InCubic);

    double endPos = groundYPos;
    double currentY = y();

    _yAnimator->setStartValue(currentY);
    _yAnimator->setEasingCurve(QEasingCurve::InQuad);
    _yAnimator->setEndValue(endPos);


    if (abs(currentY - endPos) < 0.01) {
        _birdTimer->stop();
        _yAnimator->stop();
        _rotator->stop();
        return;
    }

    _yAnimator->setDuration(800);
    _yAnimator->start();
}

void Bird::fall()
{
    _yAnimator->stop();
    slGravitation();
}

void Bird::rotate(const double &end, const int& duration, const QEasingCurve& curve)
{
    _rotator->setDuration(duration);
    _rotator->setStartValue(_currentRotation);
    _rotator->setEndValue(end);
    _rotator->setEasingCurve(curve);
    _rotator->start();
}

void Bird::setRotation(const double& angle) noexcept
{
    if (abs(_currentRotation - angle) > 0.01) {
        _currentRotation = angle;
        QPointF currentPoint = boundingRect().center();
        QTransform t;
        t.translate(currentPoint.x(), currentPoint.y());
        t.rotate(angle);
        t.translate(-currentPoint.x(), -currentPoint.y());
        setTransform(t);
    }
}

void Bird::startOscillate()
{
    _birdTimer->start(85);
    _oscillator->start();
}

void Bird::stopOscillate()
{
    _oscillator->stop();
}

void Bird::rise()
{
    double currentY = y();
    _yAnimator->stop();
    _yAnimator->setStartValue(currentY);
    _yAnimator->setEasingCurve(QEasingCurve::OutQuad);
    _yAnimator->setEndValue(currentY - (screenHeight / 10));

    _yAnimator->setDuration(285);

    _yAnimator->start();

    _birdTimer->setInterval(35);

    rotate(-20, 95, QEasingCurve::OutQuad);
}

