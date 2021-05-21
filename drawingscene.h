#ifndef DRAWINGSCENE_H
#define DRAWINGSCENE_H

#include <QGraphicsScene>
#include "point.h"
#include "building.h"
#include "raytracing.h"
#include "receiverrect.h"


namespace SceneState {
enum SceneState {
    Disabled,
    Building,
    TX,
    RX,
    Simulation
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
    bool pointIsAvailable(QPointF point);
    void clearBS();
    void clearBuilding();
    QList<Building*> building_list;
    Point* rx_item;
    Point* tx_item;
    int map_width;
    int map_height;
    int px_per_m;
    int grid_spacing_m;
    QList<Building*> getSceneBuildingList();
    Point getSceneTX();
    Point getSceneRX();
    QList<int> getMapDim();
    void draw(bool ray = true);
    void runSimulation();
    void updateMapSize(int, int);
    void setSettings(QMap<QString, qreal>);

public slots:
    void changeMap(int index);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
signals:
    int  updateBar(int value);
private:
    Building* temp_building;
    Building* temp_MS;//To remove

    QGraphicsRectItem* main_street;
    QGraphicsItemGroup* buildingsGroup;
    QList<QGraphicsLineItem*> grid;
    QGraphicsSimpleTextItem* startBuildLabel;
    QGraphicsSimpleTextItem* currentBuildLabel;
    QGraphicsItemGroup* raysGroup;
    QGraphicsSimpleTextItem* power_label;
    QList<ReceiverRect*> rectList;


    SceneState::SceneState scene_state;

    RayTracing* rayTracing;
    bool checkTxRxAreSet();
    QPointF snapToGrid(QPointF *event,int precision=1);
    bool isOnTheGrid(QGraphicsSceneMouseEvent *event);
    QPointF eventToTheGrid(QGraphicsSceneMouseEvent *event);

};

#endif // DRAWINGSCENE_H
