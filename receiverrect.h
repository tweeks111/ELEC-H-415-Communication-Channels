#ifndef RECEIVERRECT_H
#define RECEIVERRECT_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>
#include <complex>

class ReceiverRect : public QGraphicsRectItem
{
    public:
        ReceiverRect(int x,int y, int w, int h,qreal power, QGraphicsItem* parent=nullptr);
        qreal power;
        bool mouseOver=false;
        QColor rectColor;
        int debitBin;
        void debit ();
    protected:

        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        int scaleMax=-20;
        int scaleMin=-55;
    private:
        void colorRect();

};

#endif // RECEIVERRECT_H
