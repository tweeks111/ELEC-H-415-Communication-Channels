#include "drawingscene.h"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

DrawingScene::DrawingScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->map_width         = 600;
    this->map_height        = 300;
    this->px_per_m          = 2;
    this->grid_spacing_m    = 5;

    this->setSceneRect(QRectF(0,0,this->map_width*this->px_per_m,this->map_height*this->px_per_m));

    this->scene_state = SceneState::Disabled;
    this->temp_building = nullptr;
    this->rx_item = nullptr;
    this->tx_item = nullptr;
    this->rayTracing = new RayTracing(&(this->building_list),&(this->map_width),&(this->map_height),&(this->px_per_m),&(this->grid_spacing_m));

    this->txGroup = new QGraphicsItemGroup();this->addItem(this->txGroup);
    this->rxGroup = new QGraphicsItemGroup();this->addItem(this->rxGroup);
    this->buildingsGroup = new QGraphicsItemGroup();this->addItem(this->buildingsGroup);
    this->raysGroup = new QGraphicsItemGroup();this->addItem(this->raysGroup);

    for (int x=0; x<=this->map_width*this->px_per_m; x+=this->px_per_m*this->grid_spacing_m)
    {
        this->addLine(x,0,x,this->map_height*this->px_per_m,QPen(Qt::gray));
    }

    for (int y=0; y<=this->map_height*this->px_per_m; y+=this->px_per_m*this->grid_spacing_m)
    {
        this->addLine(0,y,this->map_width*this->px_per_m,y,QPen(Qt::gray));
    }
}

DrawingScene::~DrawingScene()
{

}

void DrawingScene::setSceneState(SceneState::SceneState state)
{
    this->scene_state=state;
}

void DrawingScene::createRX(){
    this->removeItem(this->rxGroup);
    this->rxGroup->removeFromGroup(this->rx_item);
    this->rx_item = new Point("RX");
    this->rxGroup->addToGroup(this->rx_item);
    this->addItem(this->rxGroup);
}

void DrawingScene::addBS()
{
    this->tx_item = new Point("TX");
    this->txGroup->addToGroup(this->tx_item);
    this->TX_items.append(tx_item);
}



void DrawingScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);
    if(this->scene_state == SceneState::Building){
        QRectF temp_building_rect(snapToGrid(&eventPoint),snapToGrid(&eventPoint));
        this->temp_building = new Building(temp_building_rect);
        this->buildingsGroup->addToGroup(temp_building);
        this->scene_state = SceneState::Building;
    }
    else if(this->scene_state == SceneState::RX && pointIsAvailable(&eventPoint)){
        this->scene_state = SceneState::Disabled;
        this->rx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::TX && pointIsAvailable(&eventPoint)){
        this->scene_state = SceneState::Disabled;
        this->tx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    draw();
}

void DrawingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);

    this->removeItem(this->buildingsGroup);
    if(this->scene_state == SceneState::Building && this->temp_building){
        QPointF topleft = this->temp_building->rect().topLeft();
        QPointF bottomRight = snapToGrid(&eventPoint);
        if(topleft != bottomRight){
            QRectF temp_building_rect(topleft,bottomRight);
            Building *new_building = new Building(temp_building_rect);
            this->building_list.append(new_building);
            this->buildingsGroup->addToGroup(new_building);
        }
        this->buildingsGroup->removeFromGroup(this->temp_building);
        this->temp_building = nullptr;
    }
    else
    {
        this->scene_state = SceneState::Disabled;
    }
    this->addItem(this->buildingsGroup);
    draw();
}

void DrawingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);

    if(this->scene_state == SceneState::Building && this->temp_building){
        QRectF temp_building_rect(this->temp_building->rect().topLeft(),
                                  snapToGrid(&eventPoint));
        this->temp_building->setRect(temp_building_rect);
    }
    else if(this->scene_state == SceneState::RX && pointIsAvailable(&eventPoint)){
        this->rx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::TX && pointIsAvailable(&eventPoint)){
        this->tx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    draw(); //Comment this for easier debugging
}

void DrawingScene::draw()
{
    //this->removeItem(this->buildingsGroup);this->removeItem(this->txGroup);this->removeItem(this->rxGroup);
    this->removeItem(this->raysGroup);
    for(QGraphicsItem* rayItem:this->raysGroup->childItems()){this->raysGroup->removeFromGroup(rayItem);}
    if(checkTxRxAreSet()){rayTracing->drawRays(&(this->tx_item->center),&(this->rx_item->center));}
    for(Ray* ray:this->rayTracing->raysList){this->raysGroup->addToGroup(ray);}
    //this->addItem(this->buildingsGroup);this->addItem(this->txGroup);this->addItem(this->rxGroup);
    this->addItem(this->raysGroup);
}

bool DrawingScene::checkTxRxAreSet()
{
    return (this->rx_item && this->tx_item && this->rx_item->isSet && this->tx_item->isSet);
}

QPointF DrawingScene::snapToGrid(QPointF *event,int precision)
{
    return QPointF(qRound(precision*event->x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/precision),
                   qRound(precision*event->y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/precision));
}

bool DrawingScene::isOnTheGrid(QGraphicsSceneMouseEvent *event)
{
    return (this->sceneRect().contains(event->scenePos()));
}

QPointF DrawingScene::eventToTheGrid(QGraphicsSceneMouseEvent *event)
{
    QPointF eventGridPoint;
    if(isOnTheGrid(event)){
        eventGridPoint = QPointF(event->scenePos());
    }
    else
    {
        if(event->scenePos().x() <0){eventGridPoint = QPointF(0,event->scenePos().y());}
        else if(event->scenePos().x() > this->map_width*this->px_per_m){eventGridPoint = QPointF(this->map_width*this->px_per_m,event->scenePos().y());}
        if(event->scenePos().y() <0){eventGridPoint = QPointF(event->scenePos().x(),0);}
        else if(event->scenePos().y() > this->map_height*this->px_per_m){eventGridPoint = QPointF(event->scenePos().x(),this->map_height*this->px_per_m);}
    }
    return eventGridPoint;
}

bool DrawingScene::pointIsAvailable(QPointF *point)
{
    for(Building* building:this->building_list)
    {
        if(building->isContainingPoint(*point) || building->isContainingPoint(*point))
        {return false;}
    }
    return true;
}

void DrawingScene::clearBS()
{
    this->removeItem(this->txGroup);
    for(QGraphicsItem* pointItem:this->txGroup->childItems()){this->txGroup->removeFromGroup(pointItem);}
    this->TX_items.clear();
    this->tx_item = nullptr;
    this->addItem(this->txGroup);
    draw();
}

void DrawingScene::clearBuilding()
{
    this->removeItem(this->buildingsGroup);
    for(QGraphicsItem* building:this->buildingsGroup->childItems()){this->buildingsGroup->removeFromGroup(building);}
    this->building_list.clear();
    this->addItem(this->buildingsGroup);
    draw();
}

void DrawingScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void DrawingScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    Q_UNUSED(event);
}

void DrawingScene::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}
