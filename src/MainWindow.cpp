#include "MainWindow.h"

#include <QGraphicsView>
#include <QResizeEvent>
#include <QScreen>
#include <QDir>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    config  = std::make_unique<QSettings>(new QSettings(QApplication::applicationDirPath() + QDir::separator() + "config.ini",
                                                        QSettings::IniFormat, this));
    setFixedSize(600, 800);

    if (config->value("Fullscreen", 0).toBool()) {
        // для игры на весь экран
    } else {
        // если view будет задавать свои параметры
    }

    _renderTimer = std::make_unique<QTimer>(new QTimer(this));
    _resizer     = std::make_unique<QTimer>(new QTimer(this));

    prepareManualViewportUpdate();

    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);
    adjustSize();

    connect(_resizer.get(), &QTimer::timeout, this, &MainWindow::resizeTriggered);
}

MainWindow::~MainWindow()
{
}

void MainWindow::prepareManualViewportUpdate()
{
    _renderTimer->setInterval(0);
    _renderTimer->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (!_resizer->isActive())
        _resizer->start(100);
    _resized = true;
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

void MainWindow::resizeTriggered()
{
    if (!_resizer) {
        adjustSize();

        _resizer->stop();
    }

    _resized = false;
}
