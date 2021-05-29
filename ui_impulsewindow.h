/********************************************************************************
** Form generated from reading UI file 'impulsewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPULSEWINDOW_H
#define UI_IMPULSEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QtCharts"

QT_BEGIN_NAMESPACE

class Ui_ImpulseWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *Layout;
    QChartView *Physical;
    QChartView *TDL;
    QChartView *USTDL;

    void setupUi(QDialog *ImpulseWindow)
    {
        if (ImpulseWindow->objectName().isEmpty())
            ImpulseWindow->setObjectName(QString::fromUtf8("ImpulseWindow"));
        ImpulseWindow->resize(528, 538);
        verticalLayoutWidget = new QWidget(ImpulseWindow);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 501, 511));
        Layout = new QVBoxLayout(verticalLayoutWidget);
        Layout->setObjectName(QString::fromUtf8("Layout"));
        Layout->setContentsMargins(0, 0, 0, 0);
        Physical = new QChartView(verticalLayoutWidget);
        Physical->setObjectName(QString::fromUtf8("Physical"));

        Layout->addWidget(Physical);

        TDL = new QChartView(verticalLayoutWidget);
        TDL->setObjectName(QString::fromUtf8("TDL"));

        Layout->addWidget(TDL);

        USTDL = new QChartView(verticalLayoutWidget);
        USTDL->setObjectName(QString::fromUtf8("USTDL"));

        Layout->addWidget(USTDL);


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
