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
        static RectState::RectState rect_state;
        static const int powMax=-40;
        static const int powMin=-150;
        static const int SNRMax=30;
        static const int SNRMin=-100;
        static const int riceMax=1;
        static const int riceMin=0;
        static constexpr qreal dsMax=1e-6;
        static constexpr qreal dsMin=1e-7;


    protected:
//        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);






};

#endif // RECEIVERRECT_H
