#ifndef VIEW_H
#define VIEW_H

#include "common.h"

#include <QElapsedTimer>
#include <QGraphicsView>

class View final : public QGraphicsView
{
    Q_OBJECT

private:
    std::unique_ptr<QElapsedTimer> timer;

public:
    View(QWidget *parent);
    ~View() = default;

protected:
    //! Переопределение метода для сцены
    void paintEvent(class QPaintEvent *event) override;
};

#endif // VIEW_H
