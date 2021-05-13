#ifndef DRAWINGSCENE_H
#define DRAWINGSCENE_H

#include <QGraphicsScene>
#include "wall.h"


namespace SceneState {
enum SceneState {
    Disabled,
    Wall,
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
    Wall *temp_wall;
    SceneState::SceneState scene_state;
};

#endif // DRAWINGSCENE_H
