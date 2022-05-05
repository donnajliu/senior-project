#include "mainwindow.h"
#include <ui_mainwindow.h>

#include <QtCore>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->uProcess_sin, SIGNAL(clicked()), this, SLOT(slot_uProcess_sin()));
    connect(ui->uProcess_cont, SIGNAL(clicked()), this, SLOT(slot_uProcess_cont()));
    connect(ui->uProcess_stop, SIGNAL(clicked()), this, SLOT(slot_uProcessTimerStop()));
    connect(ui->uProcess_animate, SIGNAL(clicked()), this, SLOT(slot_uProcessAnimTimer()));

    connect(ui->pRope_new, SIGNAL(clicked()), this, SLOT(slot_pRope_new()));
    connect(ui->pRope_sin, SIGNAL(clicked()), this, SLOT(slot_pRope_sin()));
    connect(ui->pRope_cont, SIGNAL(clicked()), this, SLOT(slot_pRope_cont()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

// simulate 1 step of pushing process
void MainWindow::slot_uProcess_sin() {
    ui->mygl->rope->simulate();
}

// simulate entire pushing process
void MainWindow::slot_uProcess_cont() {
    timer_uProcess = new QTimer(this);
    connect(timer_uProcess, SIGNAL(timeout()), this, SLOT(slot_uProcessTimer()));
    timer_uProcess->start(500);
}

void MainWindow::slot_uProcessTimer() {
    ui->mygl->rope->simulate();
}

void MainWindow::slot_uProcessTimerStop() {
    timer_uProcess->stop();
}

// immediate new procedural knot creation
void MainWindow::slot_pRope_new() {
    ui->mygl->rope->proceduralRopeGenerator();
}

// simulate single step of procedural knot creation
void MainWindow::slot_pRope_sin() {
    ui->mygl->rope->proceduralRopeGenerator();

    ui->mygl->rope->proceduralRopeValuesGenerator();
    ui->mygl->rope->generateRopeValues();
}

// simulate entire procedural knot creation
void MainWindow::slot_pRope_cont() {

    timer_pRope = new QTimer(this);
    connect(timer_pRope, SIGNAL(timeout()), this, SLOT(slot_pRopeTimer()));
    timer_pRope->start(5);

}

// simulate procedural knot creation - timer
void MainWindow::slot_pRopeTimer() {
    cout << "rope idx: " << ui->mygl->rope->ropeValuesIdx << ", total values: " << ui->mygl->rope->ropeValues.size() - 1 << endl;

    ui->mygl->rope->proceduralRopeValuesGenerator();
    ui->mygl->rope->generateRopeValues();

    if (ui->mygl->rope->ropeValuesIdx == (ui->mygl->rope->ropeValues.size())) {
        timer_pRope->stop();
    }
}

void MainWindow::slot_uProcessAnimTimer() {
    animationCounter = 0;

    timer_uProcessAnim = new QTimer(this);
    connect(timer_uProcessAnim, SIGNAL(timeout()), this, SLOT(slot_uProcess_animate()));
    timer_uProcessAnim->start(300);
}

void MainWindow::slot_uProcess_animate() {
    int i = animationCounter % 36;

    ui->mygl->rope->simulate();
    ui->mygl->currentNode->setYRotation(ui->mygl->currentNode->rotVec.y + (10.0f * PI / 180.0f));
    if (i >= 0 && i <= 8) {
        //ui->mygl->currentNode->setXTrans(ui->mygl->currentNode->transVec.x - .7);
    } else if (i >= 9 && i <= 17) {
        //ui->mygl->currentNode->setXTrans(ui->mygl->currentNode->transVec.x + .7);
    } else if (i >= 18 && i <= 26) {
        //ui->mygl->currentNode->setXTrans(ui->mygl->currentNode->transVec.x + .7);
    } else {
        //ui->mygl->currentNode->setXTrans(ui->mygl->currentNode->transVec.x - .7);
    }

    animationCounter++;
}
