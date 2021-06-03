#include "settingwindow.h"
#include "ui_settingwindow.h"

SettingWindow::SettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    connect(ui->BWSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabels()));
    connect(ui->tempSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabels()));
    connect(ui->eqheightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabels()));
    connect(ui->noiseSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLabels()));
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

QMap<QString, qreal> SettingWindow::getSettings()
{
    QMap<QString, qreal> settingsDict;
    settingsDict["BW"] = (qreal)((ui->BWSlider->value()*10)*1e6);
    settingsDict["height"] = (qreal)(1 + ui->eqheightSlider->value()*0.1);
    settingsDict["temp"] = (qreal)(-20 + ui->tempSlider->value() + 273.15);
    settingsDict["noisefig"] = (qreal)(ui->noiseSlider->value());
    return settingsDict;
}

void SettingWindow::updateLabels()
{
    int BW = ui->BWSlider->value()*10;
    qreal eqheight = 1 + ui->eqheightSlider->value()*0.1;
    int temp = -20 + ui->tempSlider->value();
    int noisefig = ui->noiseSlider->value();

    ui->BWLabel->setText(QString::number(BW)+" MHz");
    ui->eqheightLabel->setText(QString::number(eqheight) + " m");
    ui->tempLabel->setText(QString::number(temp) + " °C");
    ui->noiseLabel->setText(QString::number(noisefig) + " dB");

}


