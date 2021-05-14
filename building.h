#ifndef BUILDING_H
#define BUILDING_H

#include <QGraphicsRectItem>
#include <QLabel>
class Building : public QGraphicsRectItem
{
public:
    Building(const QRectF building_rect, QGraphicsItem *parent=nullptr);
    ~Building();
    void setRect(QRectF);
    QList<QPointF>* getCorners();
    QList<QLineF>* getWalls();
    bool isBlockingPath(QLineF* line);
    bool isContainingPoint(QPointF point);
    bool isContainingBuilding(Building* building);
private:
    QGraphicsSimpleTextItem* building_label;
    int px_per_m;
    int grid_spacing_m;
    QList<QPointF> corners;
    QList<QLineF> walls;

};


#endif // BUILDING_H
