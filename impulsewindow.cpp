#include "impulsewindow.h"
#include "ui_impulsewindow.h"
#include <QtCharts>


ImpulseWindow::ImpulseWindow(RayTracing *raytracing,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImpulseWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Impulse responses");
    this->rayData = &(raytracing->rayData);
    this->maxDelay = raytracing->delay_max;
    this->minDelay = raytracing->delay_min;
    this->BW = raytracing->BW*1e-9;
    this->deltaT = 1/(2*this->BW);
    this->Ra = raytracing->Ra;
    this->makePhysical();
    this->makeTDL();
    this->makeUSTDL();
}

ImpulseWindow::~ImpulseWindow()
{
    delete ui;
}


void ImpulseWindow::makePhysical()
{
    QScatterSeries *seriePhy = new QScatterSeries();
    seriePhy->setMarkerSize(7);
    qreal max = 0;
    for(QPair<qreal,std::complex<qreal>> data :*(this->rayData)){
        seriePhy->append(QPointF(data.first/this->deltaT,(1/(2*Ra)) * pow(abs(data.second),2)));
        if((1/(2*Ra)) * pow(abs(data.second),2) > max){
            max = (1/(2*Ra)) * pow(abs(data.second),2);
        }
    }
    QChart *chartPhy = new QChart();
    chartPhy->setMargins(QMargins(0,0,0,0));
    chartPhy->legend()->hide();
    chartPhy->addSeries(seriePhy);
    chartPhy->setTitle("Physical impulse response");
    QValueAxis *axisX = new QValueAxis();
    chartPhy->addAxis(axisX,Qt::AlignBottom);
    seriePhy->attachAxis(axisX);
    axisX->setRange(round(minDelay/deltaT) - 5, round(maxDelay/deltaT) +2);
    axisX->setTitleText("Taps");
    QValueAxis *axisY = new QValueAxis();
    chartPhy->addAxis(axisY,Qt::AlignLeft);
    seriePhy->attachAxis(axisY);
    axisY->setLabelFormat("%.2e");
    axisY->setRange(0,max*1.1);
    axisY->setTitleText("|h(t)|");
    ui->Physical->setChart(chartPhy);
    ui->Physical->setRenderHint(QPainter::Antialiasing);
    ui->Physical->show();

}

void ImpulseWindow::makeTDL()
{
    QScatterSeries *serieTDL = new QScatterSeries();
    serieTDL->setMarkerSize(7);
    qreal max = 0;
    for(int i= round(minDelay/this->deltaT)-1;i<round(maxDelay/this->deltaT)+1;i++){
        std::complex<qreal> hl = 0;
        for(QPair<qreal,std::complex<qreal>> data :*(this->rayData)){
            hl += (data.second*this->sinc(2*this->BW*(data.first - i*this->deltaT)));
        }
        if((1/(2*Ra)) * pow(abs(hl),2) > max){
            max = (1/(2*Ra)) * pow(abs(hl),2);
        }
        serieTDL->append(QPointF(i,(1/(2*Ra)) * pow(abs(hl),2)));
    }
    QChart *chartTDL = new QChart();
    chartTDL->setMargins(QMargins(0,0,0,0));
    chartTDL->legend()->hide();
    chartTDL->addSeries(serieTDL);
    chartTDL->setTitle("TDL impulse response");
    QValueAxis *axisX = new QValueAxis();
    chartTDL->addAxis(axisX,Qt::AlignBottom);
    serieTDL->attachAxis(axisX);
    axisX->setRange(round(minDelay/deltaT) - 5, round(maxDelay/deltaT) +2);
    axisX->setTitleText("Taps");
    QValueAxis *axisY = new QValueAxis();
    chartTDL->addAxis(axisY,Qt::AlignLeft);
    serieTDL->attachAxis(axisY);
    axisY->setRange(0,max*1.1);
    axisY->setTitleText("|h(t)|");
    axisY->setLabelFormat("%.2e");
    ui->TDL->setChart(chartTDL);
    ui->TDL->setRenderHint(QPainter::Antialiasing);
    ui->TDL->show();
}

void ImpulseWindow::makeUSTDL()
{
    QScatterSeries *serieUSTDL = new QScatterSeries();
    qreal max = 0;
    serieUSTDL->setMarkerSize(7);
    for(int i= round(minDelay/this->deltaT)-1;i<round(maxDelay/this->deltaT)+1;i++){
        std::complex<qreal> hl = 0;
        for(QPair<qreal,std::complex<qreal>> data :*(this->rayData)){//TODO copy list and remove elem
            if(data.first >= (i-0.5)*this->deltaT && data.first < (i+0.5)*this->deltaT){
                hl += data.second;
            }
        }
        if((1/(2*Ra)) * pow(abs(hl),2) > max){
            max = (1/(2*Ra)) * pow(abs(hl),2);
        }
        serieUSTDL->append(QPointF(i,(1/(2*Ra)) * pow(abs(hl),2)));
    }
    QChart *chartUSTDL = new QChart();
    chartUSTDL->setMargins(QMargins(0,0,0,0));
    chartUSTDL->legend()->hide();
    chartUSTDL->addSeries(serieUSTDL);
    chartUSTDL->setTitle("Uncorrelated scattering TDL impulse response");
    QValueAxis *axisX = new QValueAxis();
    chartUSTDL->addAxis(axisX,Qt::AlignBottom);
    serieUSTDL->attachAxis(axisX);
    axisX->setRange(round(minDelay/deltaT) - 5, round(maxDelay/deltaT) +2);
    axisX->setTitleText("Taps");
    QValueAxis *axisY = new QValueAxis();
    chartUSTDL->addAxis(axisY,Qt::AlignLeft);
    serieUSTDL->attachAxis(axisY);
    axisY->setRange(0,max*1.1);
    axisY->setTitleText("|h(t)|");
    axisY->setLabelFormat("%.2e");
    ui->USTDL->setChart(chartUSTDL);
    ui->USTDL->setRenderHint(QPainter::Antialiasing);
    ui->USTDL->show();
}

qreal ImpulseWindow::sinc(qreal x)
{
    return sin(3.1415*x)/(3.1415*x);
}
