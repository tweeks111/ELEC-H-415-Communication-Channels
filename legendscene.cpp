#include "legendscene.h"
#include "receiverrect.h"


LegendScene::LegendScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->drawScale();
}

LegendScene::~LegendScene()
{

}

void LegendScene::drawScale()
{
    QGraphicsRectItem *backgroundScaleRect = new QGraphicsRectItem(0,0,100,20);
    backgroundScaleRect->setBrush(Qt::white);
    addItem(backgroundScaleRect);
    scaleRect= new QGraphicsRectItem(0,0,100,20);
    scaleRect->setPen(QPen(Qt::gray,2));
    grad = new QLinearGradient(scaleRect->rect().topLeft(),scaleRect->rect().topRight());
    grad->setColorAt(0,QColor(255,0,0,255));
    grad->setColorAt(0.25,QColor(255,255,0,255));
    grad->setColorAt(0.5,QColor(0,255,0,255));
    grad->setColorAt(0.75,QColor(0,255,255,255));
    grad->setColorAt(1,QColor(0,0,255,255));
    scaleRect->setBrush(*grad);
    addItem(scaleRect);
    this->minLabel = new QGraphicsTextItem();
    this->maxLabel = new QGraphicsTextItem();
    this->minLabel->setPos(-40,0);
    this->maxLabel->setPos(101,0);
    addItem(this->minLabel);
    addItem(this->maxLabel);
    this->changeLegend();
}

void LegendScene::changeLegend()
{
    qreal min=0;
    qreal max=0;
    if(ReceiverRect::rect_state == RectState::Power){
        min = ReceiverRect::powMin;
        max = ReceiverRect::powMax;
    }
    else if(ReceiverRect::rect_state == RectState::SNR){
        min = ReceiverRect::SNRMin;
        max = ReceiverRect::SNRMax;
    }
    else if(ReceiverRect::rect_state  == RectState::Rice){
        min = ReceiverRect::riceMin;
        max =ReceiverRect::riceMax;
    }
    else if(ReceiverRect::rect_state == RectState::DelaySpread){
        min = ReceiverRect::dsMin;
        max = ReceiverRect::dsMax;
    }
    this->minLabel->setPlainText(QString::number(min));
    this->maxLabel->setPlainText(QString::number(max));
}
