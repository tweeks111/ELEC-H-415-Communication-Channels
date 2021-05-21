#ifndef RECEIVERRECT_H
#define RECEIVERRECT_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>
#include <complex>

namespace RectState {
enum RectState {
    Power,
    SNR,
    Rice,
    DelaySpread
};
}




class ReceiverRect : public QGraphicsRectItem
{


    public:
        ReceiverRect(int x,int y, int w, int h, QGraphicsItem* parent=nullptr);
        qreal power;
        qreal SNR;
        qreal rice;
        qreal delayspread;

        bool mouseOver=false;
        QColor rectColor;
        int debitBin;
        void debit ();
        void setPower(qreal power);
        void setSNR(qreal SNR);
        void colorRect();
        static void changeState(RectState::RectState state);

    protected:
//        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        int powMax=-40;
        int powMin=-150;
        int SNRMax=30;
        int SNRMin=-100;
        int riceMax=-40;
        int riceMin=-150;
        int dsMax=-40;
        int dsMin=-150;
        static RectState::RectState rect_state;




};

#endif // RECEIVERRECT_H
