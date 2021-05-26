#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingwindow.h"


class DrawingScene;
class LegendScene;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void placeBS();
    void placeRX();
    void placeBuilding(bool);
    void clearBS();
    void clearBuilding();
    void saveProject();
    void openProject();
    void runSimulation();
    void updateMapSize();
    void openDialog();
    void acceptSettings();
    void simulationFinished();
    void clearSimulation();
    void changeMap(int);

private:
    Ui::MainWindow *ui;
    DrawingScene *drawing_scene;
    LegendScene *legend_scene;
    SettingWindow* settingWindow;
    void saveProjectDataToFile(QString filename);

    void openProjectDataFile(QString filename);
};


#endif // MAINWINDOW_H
