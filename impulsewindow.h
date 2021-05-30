#ifndef IMPULSEWINDOW_H
#define IMPULSEWINDOW_H

#include <QDialog>
#include <QValueAxis>
#include "raytracing.h"

namespace Ui {
class ImpulseWindow;
}

class ImpulseWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ImpulseWindow(RayTracing *raytracing,QWidget *parent = nullptr);
    ~ImpulseWindow();

private:
    Ui::ImpulseWindow *ui;
    QList<QPair<qreal,std::complex<qreal>>> *rayData;
    void makePhysical();
    void makeTDL();
    void makeUSTDL();
    qreal sinc(qreal x);
    qreal minDelay;
    qreal maxDelay;
    qreal totalPower;
    qreal deltaT;
    qreal BW;
    qreal Ra;

};

#endif // IMPULSEWINDOW_H
