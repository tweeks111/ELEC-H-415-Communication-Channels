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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "drawingview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *addBS;
    QAction *addRX;
    QAction *clearBS;
    QAction *addBuilding;
    QAction *clearBuilding;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    DrawingView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(802, 622);

        addBS = new QAction(MainWindow);
        addBS->setObjectName(QString::fromUtf8("addBS"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/TX_icon"), QSize(), QIcon::Normal, QIcon::Off);
        addBS->setIcon(icon);

        clearBS = new QAction(MainWindow);
        clearBS->setObjectName(QString::fromUtf8("clearBS"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/clearTX_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearBS->setIcon(icon1);

        addRX = new QAction(MainWindow);
        addRX->setObjectName(QString::fromUtf8("addRX"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/RX_icon"), QSize(), QIcon::Normal, QIcon::Off);
        addRX->setIcon(icon2);

        addBuilding = new QAction(MainWindow);
        addBuilding->setObjectName(QString::fromUtf8("addBuilding"));
        addBuilding->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/rect_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        addBuilding->setIcon(icon3);

        clearBuilding = new QAction(MainWindow);
        clearBuilding->setObjectName(QString::fromUtf8("clear building"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/clearRect_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearBuilding->setIcon(icon4);

        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        graphicsView = new DrawingView(widget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout_2->addWidget(graphicsView, 0, 0, 1, 1);


        horizontalLayout->addWidget(widget);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 802, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        toolBar->addAction(addBS);
        toolBar->addAction(clearBS);
        toolBar->addAction(addRX);
        toolBar->addAction(addBuilding);
        toolBar->addAction(clearBuilding);

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

        clearBS->setText(QCoreApplication::translate("MainWindow", "Clear base station", nullptr));
#if QT_CONFIG(tooltip)
        clearBS->setToolTip(QCoreApplication::translate("MainWindow", "Clear base station", nullptr));
#endif // QT_CONFIG(tooltip)

        addRX->setText(QCoreApplication::translate("MainWindow", "Add receiver", nullptr));
#if QT_CONFIG(tooltip)
        addRX->setToolTip(QCoreApplication::translate("MainWindow", "Add receiver", nullptr));
#endif // QT_CONFIG(tooltip)

        addBuilding->setText(QCoreApplication::translate("MainWindow", "Add building", nullptr));
#if QT_CONFIG(tooltip)
        addBuilding->setToolTip(QCoreApplication::translate("MainWindow", "Add building", nullptr));
#endif // QT_CONFIG(tooltip)

        clearBuilding->setText(QCoreApplication::translate("MainWindow", "Clear building", nullptr));
#if QT_CONFIG(tooltip)
        clearBuilding->setToolTip(QCoreApplication::translate("MainWindow", "Clear building", nullptr));
#endif // QT_CONFIG(tooltip)


        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
