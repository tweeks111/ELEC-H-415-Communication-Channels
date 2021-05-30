/********************************************************************************
** Form generated from reading UI file 'impulsewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPULSEWINDOW_H
#define UI_IMPULSEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include "QtCharts"

QT_BEGIN_NAMESPACE

class Ui_ImpulseWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *Layout;
    QChartView *Physical;
    QChartView *TDL;
    QChartView *USTDL;

    void setupUi(QDialog *ImpulseWindow)
    {
        if (ImpulseWindow->objectName().isEmpty())
            ImpulseWindow->setObjectName(QString::fromUtf8("ImpulseWindow"));
        ImpulseWindow->resize(677, 638);
        ImpulseWindow->setMinimumSize(QSize(677, 0));
        horizontalLayout = new QHBoxLayout(ImpulseWindow);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        Layout = new QVBoxLayout();
        Layout->setObjectName(QString::fromUtf8("Layout"));
        Physical = new QChartView(ImpulseWindow);
        Physical->setObjectName(QString::fromUtf8("Physical"));
        Physical->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        Layout->addWidget(Physical);

        TDL = new QChartView(ImpulseWindow);
        TDL->setObjectName(QString::fromUtf8("TDL"));
        TDL->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        Layout->addWidget(TDL);

        USTDL = new QChartView(ImpulseWindow);
        USTDL->setObjectName(QString::fromUtf8("USTDL"));
        USTDL->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        Layout->addWidget(USTDL);


        horizontalLayout->addLayout(Layout);


        retranslateUi(ImpulseWindow);

        QMetaObject::connectSlotsByName(ImpulseWindow);
    } // setupUi

    void retranslateUi(QDialog *ImpulseWindow)
    {
        ImpulseWindow->setWindowTitle(QCoreApplication::translate("ImpulseWindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImpulseWindow: public Ui_ImpulseWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPULSEWINDOW_H
