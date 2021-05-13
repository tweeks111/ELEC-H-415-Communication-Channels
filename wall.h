#ifndef WALL_H
#define WALL_H

#include <QGraphicsLineItem>
#include <QLabel>
#include "complex"

class Wall : public QGraphicsLineItem
{
public:
    Wall(const QLineF wall_line, QGraphicsItem *parent=nullptr);
    ~Wall();
    void setLine(QLineF);

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QGraphicsSimpleTextItem* wall_label;
    int px_per_m;
    int grid_spacing_m;

};

#endif // WALL_H
