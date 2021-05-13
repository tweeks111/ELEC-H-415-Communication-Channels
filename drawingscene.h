#ifndef DRAWINGSCENE_H
#define DRAWINGSCENE_H

#include <QGraphicsScene>
#include "wall.h"
#include "point.h"
#include "building.h"

namespace SceneState {
enum SceneState {
    Disabled,
    Wall,
    Building,
    TX,
    RX
};
}


class DrawingScene : public QGraphicsScene
{
    Q_OBJECT

public:
    DrawingScene(QObject *parent = nullptr);
    ~DrawingScene();

    void setSceneState(SceneState::SceneState state);
    void createRX();
    void addBS();

public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
private:
    int map_width;
    int map_height;
    int px_per_m;
    int grid_spacing_m;

    QList<Wall*> wall_list;
    QList<Building*> building_list;
    QList<QGraphicsEllipseItem*> TX_items;
    Wall *temp_wall;
    Building* temp_building;
    Point* rx_item;
    Point* tx_item;
    SceneState::SceneState scene_state;
};

#endif // DRAWINGSCENE_H
