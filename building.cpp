#include "building.h"

#include <QPen>

Building::Building(const QRectF building_rect, QGraphicsItem *parent)
    :QGraphicsRectItem(building_rect,parent)
{
    QPen pen(Qt::darkGray);
    pen.setWidth(3);
    this->setBrush(QBrush(Qt::gray));
    this->setPen(pen);
    this->px_per_m          = 2;
    this->grid_spacing_m    = 5;
    this->building_label = new QGraphicsSimpleTextItem();
    this->corners.push_back(QPointF(building_rect.topLeft()));this->corners.push_back(QPointF(building_rect.topRight()));this->corners.push_back(QPointF(building_rect.bottomLeft()));this->corners.push_back(QPointF(building_rect.bottomRight()));
    this->walls.push_back(QLineF(building_rect.topLeft(),building_rect.topRight()));this->walls.push_back(QLineF(building_rect.topRight(),building_rect.bottomLeft()));this->walls.push_back(QLineF(building_rect.bottomLeft(),building_rect.bottomRight()));this->walls.push_back(QLineF(building_rect.bottomRight(),building_rect.topLeft()));
}

Building::~Building()
{

}

void Building::setRect(QRectF rect)
{
    QGraphicsRectItem::setRect(rect);
    this->building_label->setText(QString::number(rect.height()*this->px_per_m));
    this->building_label->setPos(rect.center());

}

QList<QPointF*> Building::getCorners()
{
    QList<QPointF*> cornersPointers;
    for(QPointF corner:this->corners){
        cornersPointers.push_back(&corner);
    }
    return cornersPointers;
}

QList<QLineF*> Building::getWalls()
{
    QList<QLineF*> wallsPointers;
    for(QLineF wall:this->walls){
        wallsPointers.push_back(&wall);
    }
    return wallsPointers;
}

bool Building::isBlockingPath(QLineF* line)
{
    for(QLineF wall:this->walls)
    {
        QPointF intersectionPoint;
        if(line->intersects(wall,&intersectionPoint)==QLineF::BoundedIntersection && !this->corners.contains(intersectionPoint))
        {
            return true;
        }
    }
    return false;
}

