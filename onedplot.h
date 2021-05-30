#ifndef ONEDPLOT_H
#define ONEDPLOT_H

#include <QDialog>
#include "receiverrect.h"
#include "drawingscene.h"

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
};

#endif // ONEDPLOT_H
