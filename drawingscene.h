#ifndef DRAWINGSCENE_H
#define DRAWINGSCENE_H

#include <QGraphicsScene>

class DrawingScene : public QGraphicsScene
{
public:
    DrawingScene(QObject *parent = nullptr);
    ~DrawingScene();

public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    //void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
private:
    int map_width;
    int map_height;
    int px_per_m;
    int grid_spacing_m;
};

#endif // DRAWINGSCENE_H
