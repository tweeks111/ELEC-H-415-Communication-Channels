#include "receiverrect.h"

ReceiverRect::ReceiverRect(int x,int y, int w, int h,qreal power, QGraphicsItem* parent):QGraphicsRectItem(x,y,w,h,parent)
{
    this->power=power;
    this->colorRect();
    QPen pen(rectColor);
    pen.setWidth(0);
    this->setPen(pen);
    //setAcceptHoverEvents(true);
}


void ReceiverRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mouseOver=true;
//    QPen pen(Qt::cyan);
//    pen.setWidth(2);
//    setPen(pen);
//    QGraphicsRectItem::hoverEnterEvent(event);
}

void ReceiverRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mouseOver=false;
//    QPen pen(rectColor);
//    pen.setWidth(0);
//    setPen(pen);
//    QGraphicsRectItem::hoverLeaveEvent(event);
}



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
    qreal ratioPower = (this->power-scaleMin)/(scaleMax-scaleMin);
    if (ratioPower >1){
        ratioPower =1;
    }
    else if(ratioPower<0){
        ratioPower=0;
    }

    if(ratioPower<0.25){
        B=255;
        G=4*ratioPower*255;
    }
    else if(ratioPower<0.5){
        G=255;
        B=4*(0.5-ratioPower)*255;
    }
    else if(ratioPower<0.75){
        // Green -> Yellow
        G=255;
        R=4*(ratioPower-0.5)*255;
    }
    else{
        // Yellow -> Red
        R=255;
        G=4*(1-ratioPower)*255;
    }
    this->rectColor = QColor(R,G,B,255);
    QBrush brush(this->rectColor);
    this->setBrush(brush);
}
