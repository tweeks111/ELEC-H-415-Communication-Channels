QT       += core gui\
            charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    building.cpp \
    drawingscene.cpp \
    drawingview.cpp \
    legendscene.cpp \
    main.cpp \
    mainwindow.cpp \
    point.cpp \
    ray.cpp \
    raytracing.cpp \
    impulsewindow.cpp\
    receiverrect.cpp \
    settingwindow.cpp

HEADERS += \
    building.h \
    drawingscene.h \
    drawingview.h \
    legendscene.h \
    mainwindow.h \
    impulsewindow.h\
    point.h \
    ray.h \
    raytracing.h \
    receiverrect.h \
    settingwindow.h

FORMS += \
    mainwindow.ui \
    settingwindow.ui\
    impulsewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
