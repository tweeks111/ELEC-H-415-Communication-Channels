/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "drawingview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *addBS;
    QAction *addRX;
    QAction *addBuilding;
    QAction *clearBS;
    QAction *clearBuilding;
    QAction *open;
    QAction *save;
    QAction *actionRun;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    DrawingView *graphicsView;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QSlider *widthSlider;
    QLabel *label_2;
    QLabel *label;
    QLabel *widthLabel;
    QPushButton *runBtn;
    QLabel *heightLabel;
    QSlider *heightSlider;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QMenu *menuFile;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(765, 616);
        addBS = new QAction(MainWindow);
        addBS->setObjectName(QString::fromUtf8("addBS"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/TX_icon"), QSize(), QIcon::Normal, QIcon::Off);
        addBS->setIcon(icon);
        addRX = new QAction(MainWindow);
        addRX->setObjectName(QString::fromUtf8("addRX"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/RX_icon"), QSize(), QIcon::Normal, QIcon::Off);
        addRX->setIcon(icon1);
        addBuilding = new QAction(MainWindow);
        addBuilding->setObjectName(QString::fromUtf8("addBuilding"));
        addBuilding->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("resources/rect_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        addBuilding->setIcon(icon2);
        clearBS = new QAction(MainWindow);
        clearBS->setObjectName(QString::fromUtf8("clearBS"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/clearTX_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearBS->setIcon(icon3);
        clearBuilding = new QAction(MainWindow);
        clearBuilding->setObjectName(QString::fromUtf8("clearBuilding"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/clearRect_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearBuilding->setIcon(icon4);
        open = new QAction(MainWindow);
        open->setObjectName(QString::fromUtf8("open"));
        save = new QAction(MainWindow);
        save->setObjectName(QString::fromUtf8("save"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/play-fill.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon5);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new DrawingView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout->addWidget(graphicsView);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMaximumSize(QSize(369, 16777215));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widthSlider = new QSlider(widget);
        widthSlider->setObjectName(QString::fromUtf8("widthSlider"));
        widthSlider->setMinimum(1);
        widthSlider->setMaximum(18);
        widthSlider->setSingleStep(1);
        widthSlider->setPageStep(1);
        widthSlider->setValue(10);
        widthSlider->setOrientation(Qt::Horizontal);
        widthSlider->setTickPosition(QSlider::TicksBelow);
        widthSlider->setTickInterval(1);

        gridLayout_2->addWidget(widthSlider, 1, 1, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        widthLabel = new QLabel(widget);
        widthLabel->setObjectName(QString::fromUtf8("widthLabel"));

        gridLayout_2->addWidget(widthLabel, 1, 2, 1, 1);

        runBtn = new QPushButton(widget);
        runBtn->setObjectName(QString::fromUtf8("runBtn"));

        gridLayout_2->addWidget(runBtn, 3, 1, 1, 1);

        heightLabel = new QLabel(widget);
        heightLabel->setObjectName(QString::fromUtf8("heightLabel"));

        gridLayout_2->addWidget(heightLabel, 0, 2, 1, 1);

        heightSlider = new QSlider(widget);
        heightSlider->setObjectName(QString::fromUtf8("heightSlider"));
        heightSlider->setMinimum(1);
        heightSlider->setMaximum(18);
        heightSlider->setSingleStep(1);
        heightSlider->setPageStep(1);
        heightSlider->setValue(10);
        heightSlider->setSliderPosition(10);
        heightSlider->setOrientation(Qt::Horizontal);
        heightSlider->setInvertedAppearance(false);
        heightSlider->setInvertedControls(false);
        heightSlider->setTickPosition(QSlider::TicksBelow);
        heightSlider->setTickInterval(1);

        gridLayout_2->addWidget(heightSlider, 0, 1, 1, 1);

        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        gridLayout_2->addWidget(progressBar, 2, 0, 1, 3);


        horizontalLayout->addWidget(widget, 0, Qt::AlignTop);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 765, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(open);
        menuFile->addAction(save);
        toolBar->addAction(addBS);
        toolBar->addAction(addRX);
        toolBar->addAction(clearBS);
        toolBar->addAction(addBuilding);
        toolBar->addAction(clearBuilding);
        toolBar->addSeparator();
        toolBar->addAction(actionRun);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        addBS->setText(QCoreApplication::translate("MainWindow", "Add base station", nullptr));
#if QT_CONFIG(tooltip)
        addBS->setToolTip(QCoreApplication::translate("MainWindow", "Add base station", nullptr));
#endif // QT_CONFIG(tooltip)
        addRX->setText(QCoreApplication::translate("MainWindow", "Add receiver", nullptr));
#if QT_CONFIG(tooltip)
        addRX->setToolTip(QCoreApplication::translate("MainWindow", "Add receiver", nullptr));
#endif // QT_CONFIG(tooltip)
        addBuilding->setText(QCoreApplication::translate("MainWindow", "Add Building", nullptr));
#if QT_CONFIG(tooltip)
        addBuilding->setToolTip(QCoreApplication::translate("MainWindow", "Add Building", nullptr));
#endif // QT_CONFIG(tooltip)
        clearBS->setText(QCoreApplication::translate("MainWindow", "Clear base station", nullptr));
#if QT_CONFIG(tooltip)
        clearBS->setToolTip(QCoreApplication::translate("MainWindow", "Clear base station", nullptr));
#endif // QT_CONFIG(tooltip)
        clearBuilding->setText(QCoreApplication::translate("MainWindow", "Clear building", nullptr));
#if QT_CONFIG(tooltip)
        clearBuilding->setToolTip(QCoreApplication::translate("MainWindow", "Clear building", nullptr));
#endif // QT_CONFIG(tooltip)
        open->setText(QCoreApplication::translate("MainWindow", "Open project...", nullptr));
        save->setText(QCoreApplication::translate("MainWindow", "Save project...", nullptr));
        actionRun->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
#if QT_CONFIG(tooltip)
        actionRun->setToolTip(QCoreApplication::translate("MainWindow", "Run ", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("MainWindow", "Width", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Height:", nullptr));
        widthLabel->setText(QCoreApplication::translate("MainWindow", "500m", nullptr));
        runBtn->setText(QCoreApplication::translate("MainWindow", "Heatmap", nullptr));
        heightLabel->setText(QCoreApplication::translate("MainWindow", "500m", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
