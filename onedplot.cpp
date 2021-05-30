#include "onedplot.h"
#include "ui_onedplot.h"
#include <QtCharts>

OneDPlot::OneDPlot(DrawingScene *drawingscene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OneDPlot)
{
    ui->setupUi(this);
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
        seriePWR->append(dist,rect->power);
        serieSNR->append(dist,rect->SNR);
        serieDelaySpread->append(dist,rect->delayspread/(10e-6));
        serieRiceFactor->append(dist,rect->rice);
    }
    QChart *chartPWR = new QChart();
    QChart *chartSNR = new QChart();
    QChart *chartDelaySpread = new QChart();
    QChart *chartRiceFactor = new QChart();
    chartPWR->legend()->hide();
    chartSNR->legend()->hide();
    chartDelaySpread->legend()->hide();
    chartRiceFactor->legend()->hide();
    chartPWR->addSeries(seriePWR);
    chartSNR->addSeries(serieSNR);
    chartDelaySpread->addSeries(serieDelaySpread);
    chartRiceFactor->addSeries(serieRiceFactor);
    chartPWR->createDefaultAxes();
    chartSNR->createDefaultAxes();
    chartDelaySpread->createDefaultAxes();
    chartRiceFactor->createDefaultAxes();
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

