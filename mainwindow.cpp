#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawingscene.h"
#include <QFileDialog>
#include <QGraphicsTextItem>

#define PROGRAM_SIGNATURE   "CC-RayTracing"
#define PROJECT_FILE_EXT    "Ray File (*.ray)"


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
    connect(ui->addRX, SIGNAL(triggered(bool)), this, SLOT(placeRX()));
    connect(ui->clearBS, SIGNAL(triggered(bool)), this, SLOT(clearBS()));
    connect(ui->addBuilding, SIGNAL(toggled(bool)), this, SLOT(placeBuilding(bool)));
    connect(ui->clearBuilding, SIGNAL(triggered(bool)), this, SLOT(clearBuilding()));
    connect(ui->open, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    connect(ui->save, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
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

void MainWindow::placeBuilding(bool event)
{
    SceneState::SceneState state =SceneState::Disabled;
    if(event==true){
        state =SceneState::Building;
    }
    this->drawing_scene->setSceneState(state);
}

void MainWindow::clearBS()
{
    this->drawing_scene->clearBS();
}


void MainWindow::clearBuilding()
{
    this->drawing_scene->clearBuilding();
}

void MainWindow::saveProject() {
    // Ask for export location
    QString filename = QFileDialog::getSaveFileName(
                this,
                "Save the project as...",
                QDir::homePath(),
                PROJECT_FILE_EXT);

    // If the file dialog was canceled
    if (filename.isEmpty())
        return;

    // Write project's data into the selected file
    saveProjectDataToFile(filename);
}

void MainWindow::saveProjectDataToFile(QString filename) {
    // Handle the file name
    QFile out_f(filename);

    // Open the file in write only mode
    if (!out_f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(
                    this,
                    "Project file opening error",
                    "Unable to open the destination project file");
        return;
    }

    // Stream the data file
    QDataStream out(&out_f);

    // ----- Signature ----- //
    QString signature = PROGRAM_SIGNATURE;
    out << signature;

    // ----- Scene ----- //
    // Map dimensions
    qDebug()<< 'save';
    out << this->drawing_scene->getMapDim();
    qDebug() << this->drawing_scene->getMapDim();
    // Buildings
    out << this->drawing_scene->getSceneBuildingList();

}

void MainWindow::openProject() {
    // Open existing image file
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Open a source image",
                QDir::homePath(),
                PROJECT_FILE_EXT);

    // If dialog was closed
    if (filename.isEmpty())
        return;

    // Check if file exists and is readable
    if (!QFile::exists(filename)) {
        QMessageBox::critical(
                    this,
                    "File opening error",
                    "The selected file does not exist");
        return;
    }

    // Extract the project's data
    openProjectDataFile(filename);
}

void MainWindow::openProjectDataFile(QString filename) {
    // Handle the file name
    QFile in_f(filename);
    QList<int> map_dim;
    QList<Building*> building_list;

    // Open the file in read only mode
    if (!in_f.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(
                    this,
                    "Project file opening error",
                    "Unable to open the project file");
        return;
    }

    // Stream the data file
    QDataStream in(&in_f);

    // ----- Signature ----- //
    QString signature;
    in >> signature;

    if (signature != PROGRAM_SIGNATURE) {
        QMessageBox::critical(
                    this,
                    "Project file opening error",
                    "The file you are trying to open appears not to be a compatible project file.");
        return;
    }
    qDebug() << "open";
    // ----- Base images ----- //
    // Map dimensions
    in >> map_dim;
    qDebug() << map_dim;
    // ----- Pasted items ----- //
    in >> building_list;
    qDebug() << building_list;
    // Add the new items to the scene
    foreach (Building *building, building_list) {
       this->drawing_scene->building_list.append(building);
       this->drawing_scene->draw();
    }

}
