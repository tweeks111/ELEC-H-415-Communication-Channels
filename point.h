#ifndef POINT_H
#define POINT_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>

class Point: public QGraphicsEllipseItem{
public:
    Point(QGraphicsItem *parent=nullptr);
    Point(QString point_type);
    Point(QPointF point, QColor pointColor, QGraphicsItem *parent=nullptr);
    ~Point();
    bool isSet = false;
    QPointF* getCenter();
    void setCenter(QPointF);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
private:
    QColor pointColor;
    QPointF center;
};

#endif // POINT_H
