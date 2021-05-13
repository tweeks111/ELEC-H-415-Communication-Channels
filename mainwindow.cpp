#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawingscene.h"

#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create graphics scene
    this->drawing_scene = new DrawingScene(this);

    // Put scene in the view
    ui->graphicsView->setScene(this->drawing_scene);

    // Enable mouse tracking
    ui->graphicsView->setMouseTracking(true);

    // Enable antialiasing for the view
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    ui->graphicsView->show();

    connect(ui->addBS, SIGNAL(triggered(bool)), this, SLOT(placeBS()));
    connect(ui->addWall, SIGNAL(toggled(bool)), this, SLOT(placeWall()));
    connect(ui->addRX, SIGNAL(triggered(bool)), this, SLOT(placeRX()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->graphicsView->fitInView(this->drawing_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    ui->graphicsView->fitInView(this->drawing_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::placeBS()
{

}

void MainWindow::placeRX()
{

}

void MainWindow::placeWall()
{
    this->drawing_scene->setSceneState(SceneState::Wall);
}
