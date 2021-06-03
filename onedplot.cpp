#include "onedplot.h"
#include "ui_onedplot.h"
#include <QtCharts>

OneDPlot::OneDPlot(DrawingScene *drawingscene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OneDPlot)
{
    ui->setupUi(this);
    this->setWindowTitle("Main street 1-D plot");
    QLineSeries *seriePWR = new QLineSeries();
    QLineSeries *serieSNR = new QLineSeries();
    QLineSeries *serieDelaySpread = new QLineSeries();
    QLineSeries *serieRiceFactor = new QLineSeries();
    qreal dist = 0;
    for(ReceiverRect* rect:drawingscene->mainStreetList){
        if(drawingscene->MS_h){
            dist = abs(rect->x - drawingscene->tx_item->center.x())/drawingscene->px_per_m;
        }
        else{
            dist = abs(rect->y - drawingscene->tx_item->center.y())/drawingscene->px_per_m;
        }
        seriePWR->append(log10(dist),rect->power);
        serieSNR->append(log10(dist),rect->SNR);
        serieDelaySpread->append(log10(dist),rect->delayspread);
        serieRiceFactor->append(log10(dist),rect->rice);
    }
    QChart *chartPWR = new QChart();
    QChart *chartSNR = new QChart();
    QChart *chartDelaySpread = new QChart();
    QChart *chartRiceFactor = new QChart();
    chartPWR->addSeries(seriePWR);
    chartSNR->addSeries(serieSNR);
    chartDelaySpread->addSeries(serieDelaySpread);
    chartRiceFactor->addSeries(serieRiceFactor);
    chartPWR->setMargins(QMargins(0,0,0,0));
    chartSNR->setMargins(QMargins(0,0,0,0));
    chartDelaySpread->setMargins(QMargins(0,0,0,0));
    chartRiceFactor->setMargins(QMargins(0,0,0,0));
    QValueAxis *axisXPWR = new QValueAxis();
    chartPWR->addAxis(axisXPWR,Qt::AlignBottom);
    seriePWR->attachAxis(axisXPWR);
    axisXPWR->setTitleText("Log(d");
    QValueAxis *axisYPWR = new QValueAxis();
    chartPWR->addAxis(axisYPWR,Qt::AlignLeft);
    seriePWR->attachAxis(axisYPWR);
    axisYPWR->setTitleText("Power (dBm)");
    chartPWR->legend()->hide();
    QValueAxis *axisXSNR = new QValueAxis();
    chartSNR->addAxis(axisXSNR,Qt::AlignBottom);
    serieSNR->attachAxis(axisXSNR);
    axisXSNR->setTitleText("Log(d");
    QValueAxis *axisYSNR = new QValueAxis();
    chartSNR->addAxis(axisYSNR,Qt::AlignLeft);
    serieSNR->attachAxis(axisYSNR);
    axisYSNR->setTitleText("SNR (dB)");
    chartSNR->legend()->hide();
    QValueAxis *axisXDP = new QValueAxis();
    chartDelaySpread->addAxis(axisXDP,Qt::AlignBottom);
    serieDelaySpread->attachAxis(axisXDP);
    axisXDP->setTitleText("Log(d)");
    QValueAxis *axisYDP = new QValueAxis();
    chartDelaySpread->addAxis(axisYDP,Qt::AlignLeft);
    serieDelaySpread->attachAxis(axisYDP);
    axisYDP->setTitleText("Time (ns)");
    chartDelaySpread->legend()->hide();
    QValueAxis *axisXRF = new QValueAxis();
    chartRiceFactor->addAxis(axisXRF,Qt::AlignBottom);
    serieRiceFactor->attachAxis(axisXRF);
    axisXRF->setTitleText("Log(d)");
    QValueAxis *axisYRF = new QValueAxis();
    chartRiceFactor->addAxis(axisYRF,Qt::AlignLeft);
    serieRiceFactor->attachAxis(axisYRF);
    axisYRF->setTitleText("Rice factor (dB)");
    chartRiceFactor->legend()->hide();
    chartPWR->setTitle("Power");
    chartSNR->setTitle("SNR");
    chartDelaySpread->setTitle("Delay spread");
    chartRiceFactor->setTitle("Rice factor");
    ui->PWR->setChart(chartPWR);
    ui->SNR->setChart(chartSNR);
    ui->DelaySpread->setChart(chartDelaySpread);
    ui->RiceFactor->setChart(chartRiceFactor);
    ui->PWR->setRenderHint(QPainter::Antialiasing);
    ui->SNR->setRenderHint(QPainter::Antialiasing);
    ui->DelaySpread->setRenderHint(QPainter::Antialiasing);
    ui->RiceFactor->setRenderHint(QPainter::Antialiasing);
    ui->PWR->show();
    ui->SNR->show();
    ui->DelaySpread->show();
    ui->RiceFactor->show();
}

OneDPlot::~OneDPlot()
{
    delete ui;
}

