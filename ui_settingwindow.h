/********************************************************************************
** Form generated from reading UI file 'settingwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWINDOW_H
#define UI_SETTINGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QSlider *tempSlider;
    QLabel *BWLabel;
    QSlider *BWSlider;
    QLabel *label;
    QLabel *label_3;
    QLabel *tempLabel;
    QLabel *noiseLabel;
    QSlider *noiseSlider;
    QLabel *eqheightLabel;
    QLabel *label_7;
    QLabel *label_5;
    QSlider *eqheightSlider;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingWindow)
    {
        if (SettingWindow->objectName().isEmpty())
            SettingWindow->setObjectName(QString::fromUtf8("SettingWindow"));
        SettingWindow->resize(383, 283);
        verticalLayout = new QVBoxLayout(SettingWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tempSlider = new QSlider(SettingWindow);
        tempSlider->setObjectName(QString::fromUtf8("tempSlider"));
        tempSlider->setMaximum(60);
        tempSlider->setPageStep(1);
        tempSlider->setValue(40);
        tempSlider->setOrientation(Qt::Horizontal);
        tempSlider->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(tempSlider, 2, 1, 1, 1);

        BWLabel = new QLabel(SettingWindow);
        BWLabel->setObjectName(QString::fromUtf8("BWLabel"));

        gridLayout->addWidget(BWLabel, 0, 2, 1, 1);

        BWSlider = new QSlider(SettingWindow);
        BWSlider->setObjectName(QString::fromUtf8("BWSlider"));
        BWSlider->setMaximum(20);
        BWSlider->setValue(10);
        BWSlider->setOrientation(Qt::Horizontal);
        BWSlider->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(BWSlider, 0, 1, 1, 1);

        label = new QLabel(SettingWindow);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(SettingWindow);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        tempLabel = new QLabel(SettingWindow);
        tempLabel->setObjectName(QString::fromUtf8("tempLabel"));

        gridLayout->addWidget(tempLabel, 2, 2, 1, 1);

        noiseLabel = new QLabel(SettingWindow);
        noiseLabel->setObjectName(QString::fromUtf8("noiseLabel"));

        gridLayout->addWidget(noiseLabel, 3, 2, 1, 1);

        noiseSlider = new QSlider(SettingWindow);
        noiseSlider->setObjectName(QString::fromUtf8("noiseSlider"));
        noiseSlider->setMaximum(20);
        noiseSlider->setValue(10);
        noiseSlider->setOrientation(Qt::Horizontal);
        noiseSlider->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(noiseSlider, 3, 1, 1, 1);

        eqheightLabel = new QLabel(SettingWindow);
        eqheightLabel->setObjectName(QString::fromUtf8("eqheightLabel"));

        gridLayout->addWidget(eqheightLabel, 1, 2, 1, 1);

        label_7 = new QLabel(SettingWindow);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        label_5 = new QLabel(SettingWindow);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        eqheightSlider = new QSlider(SettingWindow);
        eqheightSlider->setObjectName(QString::fromUtf8("eqheightSlider"));
        eqheightSlider->setMaximum(20);
        eqheightSlider->setValue(10);
        eqheightSlider->setOrientation(Qt::Horizontal);
        eqheightSlider->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(eqheightSlider, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(SettingWindow);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SettingWindow);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SettingWindow, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SettingWindow, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SettingWindow);
    } // setupUi

    void retranslateUi(QDialog *SettingWindow)
    {
        SettingWindow->setWindowTitle(QCoreApplication::translate("SettingWindow", "Dialog", nullptr));
        BWLabel->setText(QCoreApplication::translate("SettingWindow", "100 MHz", nullptr));
        label->setText(QCoreApplication::translate("SettingWindow", "Bandwidth", nullptr));
        label_3->setText(QCoreApplication::translate("SettingWindow", "Temperature", nullptr));
        tempLabel->setText(QCoreApplication::translate("SettingWindow", "20 \302\260C", nullptr));
        noiseLabel->setText(QCoreApplication::translate("SettingWindow", "10 dB", nullptr));
        eqheightLabel->setText(QCoreApplication::translate("SettingWindow", "2 m", nullptr));
        label_7->setText(QCoreApplication::translate("SettingWindow", "Antenna height", nullptr));
        label_5->setText(QCoreApplication::translate("SettingWindow", "Noise Figure", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingWindow: public Ui_SettingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWINDOW_H
