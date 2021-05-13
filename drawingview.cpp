#include "drawingview.h"

DrawingView::DrawingView(QWidget *parent)
    : QGraphicsView(parent)
{
    // Enable mouse tracking
    setMouseTracking(true);
}

DrawingView::~DrawingView()
{

}

void DrawingView::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
}

void DrawingView::dragEnterEvent(QDragEnterEvent *e)
{
    Q_UNUSED(e);
}

void DrawingView::dragMoveEvent(QDragMoveEvent *e)
{
    Q_UNUSED(e);
}

void DrawingView::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e);
}

