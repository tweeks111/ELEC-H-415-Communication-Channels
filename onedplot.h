#ifndef ONEDPLOT_H
#define ONEDPLOT_H

#include <QDialog>
#include "receiverrect.h"
#include "drawingscene.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace Ui {
class OneDPlot;
}

class OneDPlot : public QDialog
{
    Q_OBJECT

public:
    explicit OneDPlot(DrawingScene *drawingscene, QWidget *parent = nullptr);
    ~OneDPlot();

private:
    Ui::OneDPlot *ui;
    QPair<qreal,qreal> linearReg(const QList<qreal>& x, const QList<qreal>& y);
};

#endif // ONEDPLOT_H
