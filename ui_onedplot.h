/********************************************************************************
** Form generated from reading UI file 'onedplot.ui'
**
** Created by: Qt User Interface Compiler version 6.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONEDPLOT_H
#define UI_ONEDPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include "QtCharts"

QT_BEGIN_NAMESPACE

class Ui_OneDPlot
{
public:
    QGridLayout *gridLayout_2;
    QChartView *RiceFactor;
    QChartView *PWR;
    QChartView *DelaySpread;
    QChartView *SNR;

    void setupUi(QDialog *OneDPlot)
    {
        if (OneDPlot->objectName().isEmpty())
            OneDPlot->setObjectName(QString::fromUtf8("OneDPlot"));
        OneDPlot->setWindowModality(Qt::NonModal);
        OneDPlot->resize(631, 509);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OneDPlot->sizePolicy().hasHeightForWidth());
        OneDPlot->setSizePolicy(sizePolicy);
        OneDPlot->setLayoutDirection(Qt::LeftToRight);
        OneDPlot->setAutoFillBackground(false);
        gridLayout_2 = new QGridLayout(OneDPlot);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        RiceFactor = new QChartView(OneDPlot);
        RiceFactor->setObjectName(QString::fromUtf8("RiceFactor"));

        gridLayout_2->addWidget(RiceFactor, 3, 0, 1, 1);

        PWR = new QChartView(OneDPlot);
        PWR->setObjectName(QString::fromUtf8("PWR"));

        gridLayout_2->addWidget(PWR, 5, 1, 1, 1);

        DelaySpread = new QChartView(OneDPlot);
        DelaySpread->setObjectName(QString::fromUtf8("DelaySpread"));
        DelaySpread->setAutoFillBackground(true);
        DelaySpread->setResizeAnchor(QGraphicsView::AnchorViewCenter);

        gridLayout_2->addWidget(DelaySpread, 3, 1, 1, 1);

        SNR = new QChartView(OneDPlot);
        SNR->setObjectName(QString::fromUtf8("SNR"));
        SNR->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        gridLayout_2->addWidget(SNR, 5, 0, 1, 1);


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
