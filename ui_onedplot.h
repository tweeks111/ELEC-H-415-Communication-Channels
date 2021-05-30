/********************************************************************************
** Form generated from reading UI file 'onedplot.ui'
**
** Created by: Qt User Interface Compiler version 6.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONEDPLOT_H
#define UI_ONEDPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>
#include "QtCharts"

QT_BEGIN_NAMESPACE

class Ui_OneDPlot
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QChartView *PWR;
    QChartView *SNR;
    QChartView *DelaySpread;
    QChartView *RiceFactor;

    void setupUi(QDialog *OneDPlot)
    {
        if (OneDPlot->objectName().isEmpty())
            OneDPlot->setObjectName(QString::fromUtf8("OneDPlot"));
        OneDPlot->resize(544, 510);
        gridLayoutWidget = new QWidget(OneDPlot);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 521, 491));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        PWR = new QChartView(gridLayoutWidget);
        PWR->setObjectName(QString::fromUtf8("PWR"));

        gridLayout->addWidget(PWR, 0, 0, 1, 1);

        SNR = new QChartView(gridLayoutWidget);
        SNR->setObjectName(QString::fromUtf8("SNR"));

        gridLayout->addWidget(SNR, 0, 1, 1, 1);

        DelaySpread = new QChartView(gridLayoutWidget);
        DelaySpread->setObjectName(QString::fromUtf8("DelaySpread"));

        gridLayout->addWidget(DelaySpread, 1, 0, 1, 1);

        RiceFactor = new QChartView(gridLayoutWidget);
        RiceFactor->setObjectName(QString::fromUtf8("RiceFactor"));

        gridLayout->addWidget(RiceFactor, 1, 1, 1, 1);


        retranslateUi(OneDPlot);

        QMetaObject::connectSlotsByName(OneDPlot);
    } // setupUi

    void retranslateUi(QDialog *OneDPlot)
    {
        OneDPlot->setWindowTitle(QCoreApplication::translate("OneDPlot", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OneDPlot: public Ui_OneDPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONEDPLOT_H
