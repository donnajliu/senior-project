#pragma once

#include <QMainWindow>
#include <mygl.h>
#include <FileReader.h>

#include <QtCore>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer *timer_uProcess;
    QTimer *timer_uProcessAnim;
    QTimer *timer_pRope;

    int animationCounter;

private slots:
    void on_actionQuit_triggered();

public slots:
    void slot_uProcess_sin();
    void slot_uProcess_cont();
    void slot_uProcessTimer();
    void slot_uProcessTimerStop();
    void slot_uProcessAnimTimer();
    void slot_uProcess_animate();

    void slot_pRope_new();
    void slot_pRope_sin();
    void slot_pRope_cont();
    void slot_pRopeTimer();

private:
    Ui::MainWindow *ui;
};
