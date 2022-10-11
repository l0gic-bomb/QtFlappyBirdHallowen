#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTimer>

//!* Главное окно игры *//
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
    std::unique_ptr<QTimer> _resizer;
    std::unique_ptr<QTimer> _renderTimer;
    bool _resized;

private:
    void prepareManualViewportUpdate();

private slots:
    void resizeTriggered();

protected:
    std::unique_ptr<QSettings> config;
    double scaleFactor;

    int screenWidth, screenHeight;

};


#endif // MAINWINDOW_H
