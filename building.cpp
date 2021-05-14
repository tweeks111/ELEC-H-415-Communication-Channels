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
    this->corners.push_back(this->rect().topLeft());this->corners.push_back(this->rect().topRight());this->corners.push_back(this->rect().bottomLeft());this->corners.push_back(this->rect().bottomRight());
    this->walls.push_back(QLineF(this->rect().topLeft(),this->rect().topRight()));this->walls.push_back(QLineF(this->rect().topRight(),this->rect().bottomRight()));this->walls.push_back(QLineF(this->rect().bottomRight(),this->rect().bottomLeft()));this->walls.push_back(QLineF(this->rect().bottomLeft(),this->rect().topLeft()));
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

QList<QPointF>* Building::getCorners()
{
    return &(this->corners);
}

QList<QLineF> *Building::getWalls()
{
    return &(this->walls);
}


bool Building::isBlockingPath(QLineF* line)
{
    for(QLineF wall:this->walls)
    {
        QPointF intersectionPoint;
        if(line->intersects(wall,&intersectionPoint)==QLineF::BoundedIntersection)
        {
            if((intersectionPoint != line->p1() && intersectionPoint != line->p2()) || this->isContainingPoint(line->center()))
            {
                return true;
            }
        }
    }
    return false;
}

bool Building::isContainingPoint(QPointF point)
{
    return this->rect().toRect().contains(point.toPoint(),true);
}

bool Building::isContainingBuilding(Building *building)
{
    return this->rect().toRect().contains(building->rect().toRect());
}



