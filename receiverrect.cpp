#include "receiverrect.h"

RectState::RectState ReceiverRect::rect_state;
const int ReceiverRect::powMax;
const int ReceiverRect::powMin;
const int ReceiverRect::SNRMax;
const int ReceiverRect::SNRMin;
const int ReceiverRect::riceMax;
const int ReceiverRect::riceMin;
qreal ReceiverRect::dsMax;
constexpr qreal ReceiverRect::dsMin;

ReceiverRect::ReceiverRect(int x,int y, int w, int h, QGraphicsItem* parent)
    :QGraphicsRectItem(x,y,w,h,parent)
{
    this->x = x-w/2;
    this->y = y-h/2;
    //setAcceptHoverEvents(true);


}


//void ReceiverRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    mouseOver=true;
////    QPen pen(Qt::cyan);
////    pen.setWidth(2);
////    setPen(pen);
////    QGraphicsRectItem::hoverEnterEvent(event);
//}

//void ReceiverRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
//{
//    mouseOver=false;
////    QPen pen(rectColor);
////    pen.setWidth(0);
////    setPen(pen);
////    QGraphicsRectItem::hoverLeaveEvent(event);
//}



void ReceiverRect::debit()
{
    //Débit plafonné à 433 Mb/s
    if(power>-51){
        debitBin=433;
    }
    //Loi de relation entre le débit binaire et la puissance
    else{
        double debitBinLog=power*(log10(433)-log10(54))/(-51+82)+4.123868;
        debitBin=(int) pow(10.0,debitBinLog);
        //Débit négatif impossible
        if(debitBin<0){
            debitBin=0;
        }
    }
}

void ReceiverRect::colorRect()
{
    int R=0;int G=0;int B=0;
    qreal value = 0;qreal min =0;qreal max=0;

    if(rect_state == RectState::Power){
        value = this->power;
        min = ReceiverRect::powMin;
        max = ReceiverRect::powMax;
    }
    else if(rect_state == RectState::SNR){
        value = this->SNR;
        min = ReceiverRect::SNRMin;
        max = ReceiverRect::SNRMax;
    }
    else if(rect_state == RectState::Rice){
        value = this->rice;
        min = ReceiverRect::riceMin;
        max =ReceiverRect::riceMax;
    }
    else if(rect_state == RectState::DelaySpread){
        value = this->delayspread;
        max = ReceiverRect::dsMin;
        min = ReceiverRect::dsMax;
    }

    qreal ratio = (value-min)/(max-min);
    if (ratio >1){
        ratio =1;
    }
    else if(ratio<0){
        ratio=0;
    }

    if(ratio<0.25){
        B=255;
        G=4*ratio*255;
    }
    else if(ratio<0.5){
        G=255;
        B=4*(0.5-ratio)*255;
    }
    else if(ratio<0.75){
        // Green -> Yellow
        G=255;
        R=4*(ratio-0.5)*255;
    }
    else{
        // Yellow -> Red
        R=255;
        G=4*(1-ratio)*255;
    }
    this->rectColor = QColor(R,G,B,255);
    QPen pen(rectColor);
    pen.setWidth(0);
    this->setPen(pen);
    QBrush brush(this->rectColor);
    this->setBrush(brush);
}

void ReceiverRect::changeState(RectState::RectState state){
    ReceiverRect::rect_state = state;
}


void ReceiverRect::setPower(qreal power){
    this->power = power;
}

void ReceiverRect::setSNR(qreal SNR){
    this->SNR = SNR;
}
