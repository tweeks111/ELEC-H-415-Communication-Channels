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
    connect(ui->addWall, SIGNAL(toggled(bool)), this, SLOT(placeWall(bool)));
    connect(ui->addRX, SIGNAL(triggered(bool)), this, SLOT(placeRX()));
    connect(ui->addBuilding, SIGNAL(toggled(bool)), this, SLOT(placeBuilding(bool)));
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
    this->drawing_scene->setSceneState(SceneState::TX);
    this->drawing_scene->addBS();
}

void MainWindow::placeRX()
{
    this->drawing_scene->setSceneState(SceneState::RX);
    this->drawing_scene->createRX();
}

void MainWindow::placeWall(bool event)
{
    SceneState::SceneState state =SceneState::Disabled;
    if(event==true){
        state = SceneState::Wall;
        ui->addBuilding->setChecked(false);
    }

    this->drawing_scene->setSceneState(state);
}

void MainWindow::placeBuilding(bool event)
{
    SceneState::SceneState state =SceneState::Disabled;
    if(event==true){
        state =SceneState::Building;
        ui->addWall->setChecked(false);
    }
    this->drawing_scene->setSceneState(state);
}
