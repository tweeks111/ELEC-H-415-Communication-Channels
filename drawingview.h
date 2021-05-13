#ifndef DRAWINGVIEW_H
#define DRAWINGVIEW_H

#include <QGraphicsView>

class DrawingView : public QGraphicsView
{
    Q_OBJECT

public:
    DrawingView(QWidget *parent = nullptr);
    ~DrawingView();

protected:
    void resizeEvent(QResizeEvent *e) override;

    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dragLeaveEvent(QDragLeaveEvent *e) override;

public slots:

private:
};

#endif // DRAWINGVIEW_H
