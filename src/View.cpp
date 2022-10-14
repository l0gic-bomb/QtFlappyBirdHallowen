#include "View.h"

View::View(QWidget *parent) : QGraphicsView(parent)
{
    timer.reset(new QElapsedTimer());
    timer->start();
}

void View::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

}

