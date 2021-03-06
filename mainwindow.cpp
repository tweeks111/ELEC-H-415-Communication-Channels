#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawingscene.h"
#include "legendscene.h"
#include <QFileDialog>
#include <QGraphicsTextItem>

#define PROGRAM_SIGNATURE   "CC-RayTracing"
#define PROJECT_FILE_EXT    "Ray File (*.ray)"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Ray Tracing");
    this->settingWindow = new SettingWindow(this);
    // Create graphics scene
    this->drawing_scene = new DrawingScene(this);
    this->legend_scene = new LegendScene(this);
    // Put scene in the view
    ui->graphicsView->setScene(this->drawing_scene);

    ui->legendView->setStyleSheet("background: transparent");
    ui->legendView->setScene(this->legend_scene);

    // Enable mouse tracking
    ui->graphicsView->setMouseTracking(true);

    // Enable antialiasing for the view
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    ui->graphicsView->show();

    ui->progressBar->setAlignment(Qt::AlignCenter);

    connect(ui->addBS, SIGNAL(triggered(bool)), this, SLOT(placeBS()));
    connect(ui->addRX, SIGNAL(triggered(bool)), this, SLOT(placeRX()));
    connect(ui->clearBS, SIGNAL(triggered(bool)), this, SLOT(clearBS()));
    connect(ui->addBuilding, SIGNAL(toggled(bool)), this, SLOT(placeBuilding(bool)));
    connect(ui->clearBuilding, SIGNAL(triggered(bool)), this, SLOT(clearBuilding()));
    connect(ui->actionRun, SIGNAL(triggered(bool)), this, SLOT(runSimulation()));
    connect(ui->open, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    connect(ui->save, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
    connect(ui->widthSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMapSize()));
    connect(ui->heightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMapSize()));
    connect(ui->runBtn, SIGNAL(released()), this, SLOT(runSimulation()));
    connect(this->drawing_scene, SIGNAL(updateBar(int)), ui->progressBar, SLOT(setValue(int)));
    connect(ui->settingsBtn, SIGNAL(released()), this, SLOT(openDialog()));
    connect(ui->impulseResponse, SIGNAL(released()), this, SLOT(impulseResponse()));
    connect(ui->oneDPlot, SIGNAL(released()), this, SLOT(oneDPlot()));
    connect(this->settingWindow, SIGNAL(accept()), this, SLOT(acceptSettings()));
    connect(ui->mapBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMap(int)));
    connect(this->drawing_scene, SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    this->drawing_scene->setSceneRect(QRectF(0,
                                             0,
                                             this->drawing_scene->map_width*this->drawing_scene->px_per_m,
                                             this->drawing_scene->map_height* this->drawing_scene->px_per_m));
    ui->graphicsView->fitInView(this->drawing_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    ui->graphicsView->fitInView(this->drawing_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::placeBS()
{
    ui->addBuilding->setChecked(false);
    this->drawing_scene->setSceneState(SceneState::TX);
    this->drawing_scene->addBS();
}

void MainWindow::placeRX()
{
    ui->addBuilding->setChecked(false);
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
    ui->addBuilding->setChecked(false);
    this->drawing_scene->clearBS();
    this->drawing_scene->setSceneState(SceneState::Disabled);
}


void MainWindow::clearBuilding()
{
    ui->addBuilding->setChecked(false);
    this->drawing_scene->clearBuilding();
    this->drawing_scene->setSceneState(SceneState::Disabled);
}

void MainWindow::runSimulation()
{
    int map_width = ui->widthSlider->value()*50;
    int map_height = ui->heightSlider->value()*50;

    ui->progressBar->setRange(0, map_width*map_height);

    ui->widthSlider->setEnabled(false);
    ui->heightSlider->setEnabled(false);
    ui->settingsBtn->setEnabled(false);

    ui->runBtn->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->addBS->setEnabled(false);
    ui->clearBS->setEnabled(false);
    ui->addBuilding->setEnabled(false);
    ui->clearBuilding->setEnabled(false);
    this->drawing_scene->runSimulation();
}

void MainWindow::updateMapSize()
{
    int map_width = ui->widthSlider->value()*50;
    int map_height = ui->heightSlider->value()*50;
    ui->widthLabel->setText(QString::number(map_width)+'m');
    ui->heightLabel->setText(QString::number(map_height)+'m');
    this->drawing_scene->updateMapSize(map_width, map_height);
    ui->graphicsView->fitInView(this->drawing_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::openDialog()
{
    this->settingWindow->show();
}

void MainWindow::impulseResponse()
{
    this->impulseWindow = new ImpulseWindow(this->drawing_scene->rayTracing,this);
    this->impulseWindow->show();
}

void MainWindow::oneDPlot()
{
    this->oneDPlotWindow = new OneDPlot(this->drawing_scene,this);
    this->oneDPlotWindow->show();
}

void MainWindow::acceptSettings()
{
    this->settingWindow->hide();
    QMap<QString, qreal> settingsDict = this->settingWindow->getSettings();
    this->drawing_scene->setSettings(settingsDict);
}

void MainWindow::simulationFinished()
{
    ui->runBtn->setText("Clear");
    ui->runBtn->setEnabled(true);
    ui->runBtn->disconnect();
    connect(ui->runBtn, SIGNAL(released()), this, SLOT(clearSimulation()));
}

void MainWindow::clearSimulation()
{
    this->drawing_scene->clearSimulation();
    ui->runBtn->setText("Run");
    ui->runBtn->setEnabled(true);
    ui->widthSlider->setEnabled(true);
    ui->heightSlider->setEnabled(true);
    ui->settingsBtn->setEnabled(true);
    ui->addBS->setEnabled(true);
    ui->clearBS->setEnabled(true);
    ui->addBuilding->setEnabled(true);
    ui->clearBuilding->setEnabled(true);
    ui->runBtn->disconnect();
    connect(ui->runBtn, SIGNAL(released()), this, SLOT(runSimulation()));
}

void MainWindow::changeMap(int idx)
{
    this->drawing_scene->changeMap(idx);
    this->legend_scene->changeLegend();
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
    out << this->drawing_scene->getMapDim();
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
    // ----- Base images ----- //
    // Map dimensions
    in >> map_dim;
    ui->widthSlider->setValue(map_dim[0]/50);
    ui->heightSlider->setValue(map_dim[1]/50);
    this->updateMapSize();
    // ----- Pasted items ----- //
    in >> building_list;
    // Add the new items to the scene
    this->drawing_scene->building_list.clear();
    foreach (Building *building, building_list) {
       this->drawing_scene->building_list.append(building);
       this->drawing_scene->draw(false);
    }

}



