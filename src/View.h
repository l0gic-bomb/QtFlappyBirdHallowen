#ifndef VIEW_H
#define VIEW_H

#include "common.h"

#include <QElapsedTimer>
#include <QGraphicsView>

class View : public QGraphicsView
{
    Q_OBJECT

private:
    QElapsedTimer* _timer;

public:
    View(QWidget *parent);
    ~View();

protected:
    //! Переопределение метода для сцены
    void paintEvent(class QPaintEvent *event) override;
};

#endif // VIEW_H
