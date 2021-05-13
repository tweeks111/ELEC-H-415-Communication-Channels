#include "wall.h"

#include <QPen>

Wall::Wall(const QLineF wall_line, QGraphicsItem *parent)
    :QGraphicsLineItem(wall_line, parent)
{
    QPen pen(Qt::blue);
    pen.setWidth(2);
    this->setPen(pen);
    this->px_per_m          = 2;
    this->grid_spacing_m    = 5;
    this->wall_label = new QGraphicsSimpleTextItem();
}

Wall::~Wall()
{

}

void Wall::setLine(QLineF line)
{
    QGraphicsLineItem::setLine(line);
    this->wall_label->setText(QString::number(line.length()*this->px_per_m));
    float x = (line.x2()+line.x1())/2;
    float y = (line.y2()+line.y1())/2;
    this->wall_label->setPos(x, y);
}


