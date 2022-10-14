#include "View.h"

View::View(QWidget *parent) : QGraphicsView(parent)
{
    _timer = new QElapsedTimer();
    _timer->start();
}

View::~View()
{
    delete _timer;
}

void View::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

}

