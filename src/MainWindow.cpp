#include "MainWindow.h"

#include <QGraphicsView>
#include <QResizeEvent>
#include <QScreen>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    config  = std::make_unique<QSettings>(new QSettings(QApplication::applicationDirPath() + QDir::separator() + "config.ini",
                                                        QSettings::IniFormat, this));
    graphicsView = std::make_unique<View>(new View(this));

    setFixedSize(600, 800);
    graphicsView->setFixedSize(600, 800);

    if (config->value("Fullscreen", 0).toBool()) {
        // для игры на весь экран
    } else {
        screenHeight = graphicsView->height();
        screenWidth = graphicsView->width();
    }

    renderTimer = std::make_unique<QTimer>(new QTimer(this));
    resizer     = std::make_unique<QTimer>(new QTimer(this));

    prepareManualViewportUpdate();

    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);

    setCentralWidget(graphicsView.get());
    adjustSize();

    game.reset(new Game(graphicsView.get(), config.get(), screenWidth, screenHeight));

    connect(resizer.get(), &QTimer::timeout, this, &MainWindow::resizeTriggered);
}

MainWindow::~MainWindow()
{
}

void MainWindow::prepareManualViewportUpdate()
{
    graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

    renderTimer->setInterval(0);

    connect(renderTimer.get(), &QTimer::timeout, [&]() {
        graphicsView->viewport()->update();
    });
    renderTimer->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (!resizer->isActive())
        resizer->start(100);
    resized = true;
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

void MainWindow::resizeTriggered()
{
    if (!resizer) {
        int newWidth  = width();
        int newHeight = height();

        graphicsView->setFixedSize(newWidth, newHeight);
        adjustSize();
        graphicsView->setMinimumSize(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

        game.reset(new Game(graphicsView.get(), config.get(), newWidth, newHeight));
        resizer->stop();
    }

    resized = false;
}
