#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include <QMainWindow>
#include <QSettings>
#include <QTimer>

#include "View.h"
#include "gameObjects/Game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    //! Переопределение метода QWidget::resizeEvent
    void resizeEvent(QResizeEvent *event) override;
    //! Переопределение метода QWidget::moveEvent
    void moveEvent(QMoveEvent *event) override;

private:
    std::unique_ptr<QTimer> resizer;
    std::unique_ptr<QTimer> renderTimer;
    bool resized;

private:
    void prepareManualViewportUpdate();

private slots:
    void resizeTriggered();

protected:
    std::unique_ptr<View> graphicsView;
    std::unique_ptr<Game> game;
    std::unique_ptr<QSettings> config;
    double scaleFactor;

    int screenWidth, screenHeight;

};


#endif // MAINWINDOW_H
