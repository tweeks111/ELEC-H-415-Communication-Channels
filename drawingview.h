#ifndef DRAWINGVIEW_H
#define DRAWINGVIEW_H

#include <QGraphicsView>

class DrawingView : public QGraphicsView
{
public:
    DrawingView(QWidget *parent = nullptr);
    ~DrawingView();

public slots:

private:
};

#endif // DRAWINGVIEW_H
