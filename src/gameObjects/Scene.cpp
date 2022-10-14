#include "Scene.h"

#include <QTransform>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

Scene::Scene(Game* parent, const QRectF& rect) : QGraphicsScene(rect), game(parent)
{
    QPainter _painter;

    _flashStatus = false;
    _isClickAvailable[0] = true;
    _isClickAvailable[1] = true;

    QPixmap background(BACKGROUND_NIGHT);
    background = background.scaled(game->getScreenWidth(), game->getScreenHeight());
    game->graphicsView->setBackgroundBrush(QBrush(background));
    game->graphicsView->setScene(this);
    game->graphicsView->setSceneRect(sceneRect());

    QPixmap logo(LOGO);
    PIXMAP_SCALE(logo, game->getScaleFactor())
    itemLogo = new QGraphicsPixmapItem(logo);
    itemLogo->setPos((game->getScreenWidth() / 2) - (logo.width() / 2),
                             POS_Y_LOGO(game->getScreenHeight()));

    QPixmap pxmGround(GROUND_SEGMENT);
    PIXMAP_SCALE(pxmGround, game->getScaleFactor())
    _groundImage = QImage(game->getScreenWidth() * 2, pxmGround.height(), QImage::Format_RGB32);
    _groundImage.fill(0);
    groundPainter = new QPainter();
    groundPainter->begin(&_groundImage);
    int indicator = 0;
    int counter = 0;
    while ( counter < ((game->getScreenWidth() * 2) / pxmGround.width())) {
        groundPainter->drawPixmap(indicator, 0, pxmGround.width(), pxmGround.height(), pxmGround);
        indicator += pxmGround.width();
        ++counter;
    }
    groundPainter->end();

    QImage buffer((--counter) * pxmGround.width(), pxmGround.height(), QImage::Format_RGB32);
    buffer = _groundImage.copy(0,0,(--counter) * pxmGround.width(), pxmGround.height());
    _groundImage = buffer.scaledToWidth(_groundImage.width());
    ground = std::make_unique<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap::fromImage(_groundImage)));
    ground->setPos(0, game->getScreenHeight() - _groundImage.height() + _groundImage.height() / 7);

    QPixmap pixmap_play(BUTTON_PLAY);
    PIXMAP_SCALE(pixmap_play, game->getScaleFactor())
    btnPlay = new Button(game, pixmap_play, ButtonFunctions::playAction);
    btnPlay->setPos(game->getScreenWidth() / 2 - pixmap_play.width() * 1.2, ground->y() - pixmap_play.height());

    QPixmap pixmapAbout(BUTTON_ABOUT);
    PIXMAP_SCALE(pixmapAbout, game->getScaleFactor())
    btnAbout = new Button(game, pixmapAbout, ButtonFunctions::aboutAction);
    btnAbout->setPos(game->getScreenWidth() - btnPlay->x() - pixmapAbout.width(), btnPlay->y());

    QPixmap pipeDown(PIPE_DOWN);
    QPixmap pipeUp(PIPE_UP);
    PIXMAP_SCALE(pipeDown, game->getScaleFactor())
    PIXMAP_SCALE(pipeUp,   game->getScaleFactor())

    pipe[0][0] = new QGraphicsPixmapItem(pipeDown);
    pipe[0][1] = new QGraphicsPixmapItem(pipeDown);
    pipe[0][2] = new QGraphicsPixmapItem(pipeDown);

    pipe[1][0] = new QGraphicsPixmapItem(pipeUp);
    pipe[1][1] = new QGraphicsPixmapItem(pipeUp);
    pipe[1][2] = new QGraphicsPixmapItem(pipeUp);

    QPixmap gameInfo(GAMEINFO);
    PIXMAP_SCALE(gameInfo, game->getScaleFactor())
    itemGameInfo = new QGraphicsPixmapItem(gameInfo);
    itemGameInfo->setPos(game->getScreenWidth() / 2 - gameInfo.width() / 2,
                         game->getScreenHeight() / 2 - gameInfo.height() / 3);
    itemGameInfo->setVisible(false);

    QPixmap pixmap_gameOver(GAMEOVER);
    PIXMAP_SCALE(pixmap_gameOver, game->getScaleFactor())
    itemGameOver = new QGraphicsPixmapItem(pixmap_gameOver);
    itemGameOver->setPos(itemLogo->pos());
    itemGameOver->setVisible(false);

    QPixmap pixmap_scoreBoard(SCOREBOARD);
    PIXMAP_SCALE(pixmap_scoreBoard, game->getScaleFactor())
    itemScoreBoard = new QGraphicsPixmapItem(pixmap_scoreBoard);
    itemScoreBoard->setPos(game->getScreenWidth() / 2 - pixmap_scoreBoard.width() / 2,
                                   itemGameOver->y() + itemGameOver->boundingRect().height() + (game->getScreenHeight() / 20.69));
    itemScoreBoard->setVisible(false);

    _bigZero = QPixmap(BIGNUMBER_ZERO);
    PIXMAP_SCALE(_bigZero, game->getScaleFactor())
    _mainScore = QImage(_bigZero.width() * 3, _bigZero.height(), QImage::Format_ARGB32);
    _mainScore.fill(qRgba(0, 0, 0, 0));

    _painter.begin(&_mainScore);
    _painter.drawPixmap(0, 0, _bigZero.width(), _bigZero.height(), _bigZero);
    _painter.end();
    itemScore = new QGraphicsPixmapItem(_bigZero);
    itemScore->setVisible(false);
    itemScore->setPos(game->getScreenWidth() / 2 - _bigZero.width() / 2,
                      _bigZero.height() * 2);

    _smallZero = QPixmap(SMALLNUMBER_ZERO);
    PIXMAP_SCALE(_smallZero, game->getScaleFactor())
    _endScore = QImage(pixmap_scoreBoard.width(), _smallZero.height(), QImage::Format_ARGB32);
    _endScore.fill(qRgba(0,0,0,0));
    itemEndScore = new QGraphicsPixmapItem();
    itemEndScore->setVisible(false);

    itemEndScoreRecord = new QGraphicsPixmapItem();
    itemEndScoreRecord->setVisible(false);


    QPixmap pixmap_bird(BIRD_UP);
    QPointF posBird(game->getScreenWidth() / 2 - pixmap_bird.width() / 2,
                    itemLogo->pos().y() + pixmap_bird.height() * 5);
    PIXMAP_SCALE(pixmap_bird, game->getScaleFactor());
    bird = new Bird(posBird, pixmap_bird,
                    game->getScreenHeight() - ground->boundingRect().height(),
                    game->getScreenWidth(),
                    game->getScreenHeight(),
                    game->getScaleFactor());

    _buttons.append(btnPlay);
    _buttons.append(btnAbout);

    groupItem[GROUP_NEWROUND][0] = itemGameInfo;
    groupItem[GROUP_NEWROUND][1] = nullptr;

    groupItem[GROUP_GAMEPLAY][0] = itemGameInfo;
    groupItem[GROUP_GAMEPLAY][1] = itemScore;
    groupItem[GROUP_GAMEPLAY][2] = nullptr;

    groupItem[GROUP_ROUNDEND][0] = itemScoreBoard;
    groupItem[GROUP_ROUNDEND][1] = itemGameOver;
    groupItem[GROUP_ROUNDEND][2] = btnPlay;
    groupItem[GROUP_ROUNDEND][3] = itemEndScore;
    groupItem[GROUP_ROUNDEND][4] = itemEndScoreRecord;
    groupItem[GROUP_ROUNDEND][5] = nullptr;

    groupItem[GROUP_FIRSTSCREEN][0] = itemLogo;
    groupItem[GROUP_FIRSTSCREEN][1] = btnPlay;
    groupItem[GROUP_FIRSTSCREEN][2] = btnAbout;
    groupItem[GROUP_FIRSTSCREEN][3] = nullptr;

    addItem(itemLogo);
    addItem(pipe[0][0]);
    addItem(pipe[0][1]);
    addItem(pipe[0][2]);
    addItem(pipe[1][0]);
    addItem(pipe[1][1]);
    addItem(pipe[1][2]);
    addItem(btnAbout);
    addItem(itemGameInfo);
    addItem(bird);
    addItem(ground.get());
    addItem(itemScoreBoard);
    addItem(itemGameOver);
    addItem(itemEndScore);
    addItem(itemEndScoreRecord);
    addItem(btnPlay);
    addItem(itemScore);
}

Scene::~Scene()
{
    delete groundPainter; 
    delete bird;
    delete itemGameOver;
    delete itemScoreBoard;
    delete itemEndScore;
    delete itemEndScoreRecord;
    delete itemLogo;
    delete itemGameInfo;
    delete itemScore;
    delete itemFlash;
    delete opacityAnim;
    delete opacityEffect;

}

bool Scene::isGroupVisible(const int& groupIndex) const noexcept
{
    return groupItem[groupIndex][0]->isVisible();
}

//void Scene::pixmapScale(QPixmap* pixmap, const double &scale)
//{
//    auto tmp = (pixmap->size()) * scale;
//    pixmap->scaled(tmp);
//}

void Scene::gameOver(const int& score, const int& scoreRecord)
{
    flash(Qt::red, 300, QEasingCurve::Linear);

    QPixmap reward = QPixmap(SCOREBOARD);

    PIXMAP_SCALE(reward, game->getScaleFactor())
    itemScoreBoard->setPixmap(reward);

    // Вычисление позиций элементов
    int postionScore;
    itemEndScore->setPixmap(makeEndingScore(score, &postionScore));
    itemEndScore->setPos(itemScoreBoard->x() + itemScoreBoard->boundingRect().width() - ((postionScore + 1) * _smallZero.width()) - 22 * game->getScaleFactor(),
                                 itemScoreBoard->y() + _smallZero.height() * 2.7);

    int postionScoreRecord;
    itemEndScoreRecord->setPixmap(makeEndingScore(scoreRecord, &postionScoreRecord));
    itemEndScoreRecord->setPos(itemEndScore->x() - postionScoreRecord * _smallZero.width() + postionScore * _smallZero.width(),
                                       itemEndScore->y() + itemEndScore->boundingRect().height() + 28.5 * game->getScaleFactor());

    btnPlay->setPos(game->getScreenWidth() / 2 - btnPlay->boundingRect().width() / 2,
                    itemScoreBoard->y() + itemScoreBoard->boundingRect().height() + 20);

    fadeGroup(GROUP_ROUNDEND, true, 500);
}

QPixmap Scene::makeMainScore(const int& score)
{
    QPainter painter;
    _mainScore.fill(qRgba(0, 0, 0, 0));
    painter.begin(&_mainScore);
    QString strScore = QString::number(score);

    int zeroWidth = _bigZero.width();
    int zeroHeight = _bigZero.height();


    for (int i = 0; i < strScore.length(); ++i) {
        QPixmap generatedNumber;
        generatedNumber.load(QString(BIGNUMBER_GENERIC).arg(strScore.mid(i, 1)));

        if (i == 0)
            painter.drawPixmap(i * zeroWidth, 0, zeroWidth, zeroHeight, generatedNumber);
        else
            painter.drawPixmap(i * zeroWidth - zeroWidth / 9.25, 0, zeroWidth, zeroHeight, generatedNumber);
    }
    painter.end();

    return QPixmap::fromImage(_mainScore);
}

void Scene::updateScore(const int& score)
{
    itemScore->setPixmap(makeMainScore(score));
}

QPixmap Scene::makeEndingScore(const int& score, int* pos)
{
    QString strScore = QString::number(score);
    _endScore.fill(qRgba(0, 0, 0, 0));

    QPainter painter;
    painter.begin(&_endScore);

    int zeroWidth = _smallZero.width();
    int zeroHeight = _smallZero.height();

    QPixmap generatedNumber;

    for (int counter = 0; counter < strScore.length(); ++counter) {
        generatedNumber.load(QString(SMALLNUMBER_GENERIC).arg(strScore.mid(counter, 1)));
        painter.drawPixmap(counter * zeroWidth + 3, 0, zeroWidth, zeroHeight, generatedNumber);
    }
    painter.end();

    if (pos)
        *pos = strScore.length();

    return QPixmap::fromImage(_endScore);
}

void Scene::updateGround()
{
    if (ground->x() <= -(ground->boundingRect().width() / 2)) {
        groundPainter->begin(&_groundImage);
        groundPainter->drawPixmap(_groundImage.width(), 0, _groundImage.width() / 2,
                                  _groundImage.height(), QPixmap::fromImage(_groundImage.copy(0,
                                                                            0,
                                                                            _groundImage.width() / 2,
                                                                            _groundImage.height())));

        ground->setPixmap(QPixmap::fromImage(_groundImage));
        ground->setPos(0, ground->y());
        groundPainter->end();
    }
}

void Scene::fadeGroup(const int& groupIndex, const bool& show, const int& duration, const int& group2Index)
{
    if (!_isClickAvailable[0])
        return;

    _isClickAvailable[0] = false;

    int endMark = 0;
    while (groupItem[groupIndex][endMark] != nullptr)
        endMark++;

    opacityEffectGroup[0] = new QGraphicsOpacityEffect*[endMark];
    opacityAnimGroup[0]   = new QPropertyAnimation*[endMark];

    for (int k = 0; k < endMark; ++k) {
        opacityEffectGroup[0][k] = new QGraphicsOpacityEffect(this);
        opacityAnimGroup[0][k]   = new QPropertyAnimation(opacityEffectGroup[0][k], "opacity", this);
        groupItem[groupIndex][k]->setGraphicsEffect(opacityEffectGroup[0][k]);

        if (!groupItem[groupIndex][k]->isVisible())
            groupItem[groupIndex][k]->show();

        groupItem[groupIndex][k]->setOpacity(1.0);

        if (show) {
            opacityAnimGroup[0][k]->setStartValue(0.0);
            opacityAnimGroup[0][k]->setEndValue(1.0);
        } else {
            opacityAnimGroup[0][k]->setStartValue(1.0);
            opacityAnimGroup[0][k]->setEndValue(0.0);
        }

        QObject::connect(opacityAnimGroup[0][k], &QPropertyAnimation::finished, [=](){
               groupItem[groupIndex][k]->setGraphicsEffect(nullptr);
               if (!show)
                   groupItem[groupIndex][k]->hide();

               if (group2Index == -1)
                _isClickAvailable[0] = true;
            });

        opacityAnimGroup[0][k]->setEasingCurve(QEasingCurve::Linear);
        opacityAnimGroup[0][k]->setDuration(duration);

        opacityAnimGroup[0][k]->start(QAbstractAnimation::DeleteWhenStopped);
    }

    if (group2Index != -1) {
        int endMark2 = 0;
        while (groupItem[group2Index][endMark2] != nullptr)
            ++endMark2;

        opacityEffectGroup[1] = new QGraphicsOpacityEffect*[endMark2];
        opacityAnimGroup[1]   = new QPropertyAnimation*[endMark2];


        for (int k = 0; k < endMark2; ++k) {
            opacityEffectGroup[1][k] = new QGraphicsOpacityEffect(this);
            opacityAnimGroup[1][k] = new QPropertyAnimation(opacityEffectGroup[1][k], "opacity", this);
            groupItem[group2Index][k]->setGraphicsEffect(opacityEffectGroup[1][k]);

            if (!groupItem[group2Index][k]->isVisible())
                groupItem[group2Index][k]->show();

            groupItem[group2Index][k]->setOpacity(1.0);

            if (show) {
                opacityAnimGroup[1][k]->setStartValue(1.0);
                opacityAnimGroup[1][k]->setEndValue(0.0);
            } else {
                opacityAnimGroup[1][k]->setStartValue(0.0);
                opacityAnimGroup[1][k]->setEndValue(1.0);
            }

            QObject::connect(opacityAnimGroup[1][k], &QPropertyAnimation::finished, [=](){
                   groupItem[group2Index][k]->setGraphicsEffect(nullptr);
                   if (show)
                       groupItem[group2Index][k]->hide();

                   _isClickAvailable[0] = true;
                });

            opacityAnimGroup[1][k]->setEasingCurve(QEasingCurve::Linear);
            opacityAnimGroup[1][k]->setDuration(duration);

            opacityAnimGroup[1][k]->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

    QTimer::singleShot(duration + 5, [this, group2Index]() {
        delete[] opacityEffectGroup[0];
        delete[] opacityAnimGroup[0];
        if (group2Index != -1) {
            delete[] opacityEffectGroup[1];
            delete[] opacityAnimGroup[1];
        }
    });
}

void Scene::flash(const QColor &color, const int& duration, const QEasingCurve& curve)
{
    if(!_isClickAvailable[1])
        return;

    QImage refImg = QImage(width(), height(), QImage::Format_RGB32);

    _isClickAvailable[1] = false;

    opacityEffect =  new QGraphicsOpacityEffect(this);
    opacityAnim = new QPropertyAnimation(opacityEffect, "opacity", this);

    itemFlash = new QGraphicsPixmapItem();
    itemFlash->setGraphicsEffect(opacityEffect);

    _flashStatus = false;

    QPainter painter;
    painter.begin(&refImg);
    painter.fillRect(QRect(0, 0, width(), height()), color);
    painter.end();

    itemFlash->setPixmap(QPixmap::fromImage(refImg));

    addItem(itemFlash);

    opacityAnim->setStartValue(0.0);
    opacityAnim->setDuration(duration);
    opacityAnim->setEasingCurve(curve);
    opacityAnim->setEndValue(1.0);

    connect(opacityAnim, &QPropertyAnimation::finished, [=](){
        if (_flashStatus == false) {
            QTimer::singleShot(75, [=]() {
               if (opacityAnim) {
                    opacityAnim->setStartValue(1.0);
                    opacityAnim->setEndValue(0.0);
                    opacityAnim->setDuration(duration);
                    opacityAnim->start(QAbstractAnimation::DeleteWhenStopped);
               }
            });
            _flashStatus = true;
        } else {
            removeItem(itemFlash);
            delete itemFlash;
            _flashStatus = false;
            _isClickAvailable[1] = true;
        }
});
    opacityAnim->start();
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ((!_isClickAvailable[0] || !_isClickAvailable[1]) && !(game->isGameActuallyStarted())) {
        event->accept();
        return;
    }

    QPointF pos = event->scenePos();

    for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        QApplication::processEvents();
        if (*it == nullptr)
            break;
        else if (itemAt(pos, game->graphicsView->transform()) == *it) {
            (*it)->invoke();
            return;
        }
    }

    game->clickEvent();
    event->accept();
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    mousePressEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if ((!_isClickAvailable[0] || !_isClickAvailable[1]) && !(game->isGameActuallyStarted())) {
        event->accept();
        return;
    }
    game->clickEvent();
    event->accept();
}
