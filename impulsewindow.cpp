#include "impulsewindow.h"
#include "ui_impulsewindow.h"
#include <QtCharts>


ImpulseWindow::ImpulseWindow(RayTracing *raytracing,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImpulseWindow)
{
    ui->setupUi(this);
    this->rayData = &(raytracing->rayData);
    this->maxDelay = raytracing->delay_max;
    this->minDelay = raytracing->delay_min;
    this->BW = raytracing->BW;
    this->deltaT = 1/(2*raytracing->BW);
    this->XAxis = new QValueAxis();
    this->XAxis->setRange(this->minDelay/this->deltaT,this->maxDelay/this->deltaT);
    this->XAxis->setLabelFormat("%g");
    this->YAxis = new QValueAxis();
    this->YAxis->setRange(0,this->totalPower);
    this->YAxis->setLabelFormat("%g");

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
    seriePhy->setMarkerSize(10);
    for(QPair<qreal,std::complex<qreal>> data :*(this->rayData)){
        seriePhy->append(QPointF(data.first/this->deltaT,abs(data.second)));
    }
    QChart *chartPhy = new QChart();
    chartPhy->legend()->hide();
    chartPhy->addSeries(seriePhy);
    chartPhy->createDefaultAxes();
    chartPhy->setTitle("Physical impulse response");
    ui->Physical->setChart(chartPhy);
    ui->Physical->setRenderHint(QPainter::Antialiasing);
    ui->Physical->show();

}

void ImpulseWindow::makeTDL()
{
    QScatterSeries *serieTDL = new QScatterSeries();
    serieTDL->setMarkerSize(10);
    for(int i= round(minDelay/this->deltaT)-2;i<round(maxDelay/this->deltaT)+1;i++){
        std::complex<qreal> hl = 0;
        for(QPair<qreal,std::complex<qreal>> data :*(this->rayData)){
            hl += (data.second*this->sinc(2*this->BW*(data.first - i*this->deltaT)));
        }
        serieTDL->append(QPointF(i,abs(hl)));
    }
    QChart *chartTDL = new QChart();
    chartTDL->legend()->hide();
    chartTDL->addSeries(serieTDL);
    chartTDL->createDefaultAxes();
    chartTDL->setTitle("TDL impulse response");
    ui->TDL->setChart(chartTDL);
    ui->TDL->setRenderHint(QPainter::Antialiasing);
    ui->TDL->show();
}

void ImpulseWindow::makeUSTDL()
{
    QScatterSeries *serieUSTDL = new QScatterSeries();
    serieUSTDL->setMarkerSize(10);
    for(int i= round(minDelay/this->deltaT)-2;i<round(maxDelay/this->deltaT)+1;i++){
        std::complex<qreal> hl = 0;
        bool isThereData = false;
        for(QPair<qreal,std::complex<qreal>> data :*(this->rayData)){//TODO copy list and remove elem
            if(data.first >= i*this->deltaT && data.first < (i+1)*this->deltaT){
                hl += data.second;
                isThereData = true;
            }
        }
        if(isThereData){
            serieUSTDL->append(QPointF(i,abs(hl)));
        }
    }
    QChart *chartUSTDL = new QChart();
    chartUSTDL->legend()->hide();
    chartUSTDL->addSeries(serieUSTDL);
    chartUSTDL->createDefaultAxes();
    chartUSTDL->setTitle("Uncorrelated scattering TDL impulse response");
    ui->USTDL->setChart(chartUSTDL);
    ui->USTDL->setRenderHint(QPainter::Antialiasing);
    ui->USTDL->show();
}

qreal ImpulseWindow::sinc(qreal x)
{
    return sin(x)/x;
}
