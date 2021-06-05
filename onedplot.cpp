#include "onedplot.h"
#include "ui_onedplot.h"
#include <QtCharts>

OneDPlot::OneDPlot(DrawingScene *drawingscene, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OneDPlot)
{
    ui->setupUi(this);
    this->setWindowTitle("Main street 1-D plot");
    if(drawingscene->tx_items.size() == 1){
        QLineSeries *seriePWR = new QLineSeries();
        QLineSeries *seriePWRLine = new QLineSeries();
        seriePWRLine->setColor(QColor(Qt::red));
        QLineSeries *serieSNR = new QLineSeries();
        QLineSeries *serieDelaySpread = new QLineSeries();
        QLineSeries *serieRiceFactor = new QLineSeries();
        qreal dist = 0;
        qreal distMax = 0;
        QList<qreal> x = QList<qreal>();
        QList<qreal> y = QList<qreal>();
        for(ReceiverRect* rect:drawingscene->mainStreetList){
            if(drawingscene->MS_h){
                dist = abs(rect->x - drawingscene->tx_items.first()->center.x())/drawingscene->px_per_m;
            }
            else{
                dist = abs(rect->y - drawingscene->tx_items.first()->center.y())/drawingscene->px_per_m;
            }
            if(dist > distMax){
                distMax = dist;
            }
            x.append(log10(dist)-log10(10));
            y.append(rect->power);
            seriePWR->append(log10(dist),rect->power);
            serieSNR->append(log10(dist),rect->SNR);
            serieDelaySpread->append(log10(dist),rect->delayspread);
            serieRiceFactor->append(log10(dist),rect->rice);
        }
        QPair<qreal,qreal> reg = this->linearReg(x,y);
        seriePWRLine->append(1,reg.second);
        seriePWRLine->append(distMax,reg.first*distMax);
        QChart *chartPWR = new QChart();
        QChart *chartSNR = new QChart();
        QChart *chartDelaySpread = new QChart();
        QChart *chartRiceFactor = new QChart();
        chartPWR->addSeries(seriePWR);
        chartPWR->addSeries(seriePWRLine);
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
        seriePWRLine->attachAxis(axisXPWR);
        axisXPWR->setTitleText("Log(d)");
        QValueAxis *axisYPWR = new QValueAxis();
        chartPWR->addAxis(axisYPWR,Qt::AlignLeft);
        seriePWR->attachAxis(axisYPWR);
        seriePWRLine->attachAxis(axisYPWR);
        axisYPWR->setTitleText("Power (dBm)");
        chartPWR->legend()->hide();
        QValueAxis *axisXSNR = new QValueAxis();
        chartSNR->addAxis(axisXSNR,Qt::AlignBottom);
        serieSNR->attachAxis(axisXSNR);
        axisXSNR->setTitleText("Log(d)");
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
        //chartPWR->setTitle("Power");
        chartPWR->setTitle("Power (n = " + QString::number(abs(reg.first)/10) + ")");
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
}

OneDPlot::~OneDPlot()
{
    delete ui;
}

QPair<qreal,qreal> OneDPlot::linearReg(const QList<qreal>& x, const QList<qreal>& y)
//Make the linear regression
{
    const auto n    = x.size();
    const auto s_x  = std::accumulate(x.begin(), x.end(), 0.0);
    const auto s_y  = std::accumulate(y.begin(), y.end(), 0.0);
    const auto s_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
    const auto s_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    const auto a    = (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
    const auto b    = (s_y - a * s_x) / n;
    return QPair<qreal,qreal>(a,b);
}


