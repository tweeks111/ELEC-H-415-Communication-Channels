#ifndef BUILDING_H
#define BUILDING_H

#include <QGraphicsRectItem>
#include <QLabel>
class Building : public QGraphicsRectItem
{
public:
    Building(const QRectF building_rect, QGraphicsItem *parent=nullptr);
    Building(const QRectF building_rect,int color, QGraphicsItem *parent=nullptr);
    ~Building();
    void setRect(QRectF);
    QList<QPointF>* getCorners();
    QList<QLineF>* getWalls();
    bool isBlockingPath(QLineF* line);
    bool isContainingPoint(QPointF point);
    bool isContainingBuilding(Building* building);
    QGraphicsSimpleTextItem* building_label;
private:
    int px_per_m;
    int grid_spacing_m;
    QList<QPointF> corners;
    QList<QLineF> walls;
    qreal maxScale = 2;
    void updateLabel(QRectF rect);

};


#endif // BUILDING_H
