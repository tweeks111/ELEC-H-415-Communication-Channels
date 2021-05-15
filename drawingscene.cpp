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
    this->temp_MS = nullptr;
    this->rx_item = nullptr;
    this->tx_item = nullptr;
    this->main_street = nullptr;
    this->rayTracing = new RayTracing(&(this->map_width),&(this->map_height),&(this->px_per_m),&(this->grid_spacing_m));


    this->buildingsGroup = new QGraphicsItemGroup();this->addItem(this->buildingsGroup);
    this->raysGroup = new QGraphicsItemGroup();this->addItem(this->raysGroup);

    this->startBuildLabel = new QGraphicsSimpleTextItem();this->startBuildLabel->setScale(1.5);
    this->currentBuildLabel = new QGraphicsSimpleTextItem();this->currentBuildLabel->setScale(1.5);

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


void DrawingScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);
    if(this->scene_state == SceneState::Building){
        QRectF temp_building_rect(snapToGrid(&eventPoint),snapToGrid(&eventPoint));
        this->temp_building = new Building(temp_building_rect);
        this->buildingsGroup->addToGroup(temp_building);
        this->scene_state = SceneState::Building;

        this->startBuildLabel->setText("("+QString::number(qRound(snapToGrid(&eventPoint).x()/this->px_per_m))+","+QString::number(qRound(snapToGrid(&eventPoint).y()/this->px_per_m))+")");
        this->startBuildLabel->setPos(eventPoint+QPointF(12,-20));
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
    this->removeItem(this->buildingsGroup);
    QPointF eventPoint = eventToTheGrid(event);
    if(this->scene_state == SceneState::Building && this->temp_building){
        QPointF topleft = this->temp_building->rect().topLeft();
        QPointF bottomRight = snapToGrid(&eventPoint);
        if(topleft != bottomRight){
            QRectF temp_building_rect(topleft,bottomRight);
            Building *new_building = new Building(temp_building_rect);
            this->building_list.append(new_building);
            this->buildingsGroup->addToGroup(new_building);
        }
        this->removeItem(this->startBuildLabel);
        this->buildingsGroup->removeFromGroup(this->temp_building);
        this->addItem(this->buildingsGroup);
        this->temp_building = nullptr;
    }
    else {
        this->scene_state = SceneState::Disabled;
    }
    this->addItem(this->buildingsGroup);
    draw();
}

void DrawingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);  

    this->currentBuildLabel->setText("("+QString::number(qRound(snapToGrid(&eventPoint).x()/this->px_per_m))+","+QString::number(qRound(snapToGrid(&eventPoint).y()/this->px_per_m))+")");
    this->currentBuildLabel->setPos(eventPoint+QPointF(12,-20));

    if(this->scene_state == SceneState::Building && this->temp_building){
        QRectF temp_building_rect(this->temp_building->rect().topLeft(),snapToGrid(&eventPoint));
        this->temp_building->setRect(temp_building_rect);
    }
    else if(this->scene_state == SceneState::RX && pointIsAvailable(&eventPoint)){
        this->rx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::TX && pointIsAvailable(&eventPoint)){
        this->tx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    draw(); //Comment this for easier debugging (click for update)


}

void DrawingScene::draw()
{
    this->removeItem(this->main_street);
    this->main_street = nullptr;
    this->removeItem(this->raysGroup);
    for(QGraphicsItem* rayItem:this->raysGroup->childItems())
    {
        this->raysGroup->removeFromGroup(rayItem);
    }    
    if(this->tx_item && this->tx_item->isSet)
    {
        this->rayTracing->findMainStreetQRectF(&(this->tx_item->center), &(this->building_list));
        if(this->rayTracing->mainStreet)
        {
            this->main_street = new QGraphicsRectItem(this->rayTracing->mainStreet->toRect());
            this->main_street->setBrush(Qt::blue);
            this->main_street->setOpacity(0.15);
            this->addItem(this->main_street);
        }
    }
    this->removeItem(this->buildingsGroup);
    this->buildingsGroup = new QGraphicsItemGroup();
    if(this->temp_building)
    {
        this->buildingsGroup->addToGroup(this->temp_building);
    }
    for(Building* building:this->building_list)
    {
        this->buildingsGroup->addToGroup(building);
    }
    this->addItem(this->buildingsGroup);

    if(this->tx_item && this->tx_item->isSet && this->rx_item && this->rx_item->isSet)
    {
        rayTracing->drawRays(&(this->tx_item->center), &(this->rx_item->center), &(this->building_list));
        for(Ray* ray:this->rayTracing->raysList)
        {
            this->raysGroup->addToGroup(ray);
        }
        this->addItem(this->raysGroup);
    }

    if(this->startBuildLabel){this->removeItem(this->startBuildLabel);}
    this->addItem(this->startBuildLabel);
    if(this->currentBuildLabel){this->removeItem(this->currentBuildLabel);}
    this->addItem(this->currentBuildLabel);
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
    QPointF eventGridPoint = QPointF(event->scenePos());
    {
        if(event->scenePos().x() <0){eventGridPoint.setX(0);}
        else if(event->scenePos().x() > this->map_width*this->px_per_m){eventGridPoint.setX(this->map_width*this->px_per_m);}
        if(event->scenePos().y() <0){eventGridPoint.setY(0);;}
        else if(event->scenePos().y() > this->map_height*this->px_per_m){eventGridPoint.setY(this->map_height*this->px_per_m);}
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
void DrawingScene::createRX()
{
    this->removeItem(this->rx_item);
    this->rx_item = new Point("RX");
    this->addItem(this->rx_item);
}

void DrawingScene::addBS()
{
    this->removeItem(this->tx_item);
    this->tx_item = new Point("TX");
    this->addItem(this->tx_item);
}

void DrawingScene::clearBS()
{
    this->removeItem(this->tx_item);
    this->removeItem(this->rx_item);
    this->tx_item = nullptr;
    this->rx_item = nullptr;
    draw();
}

void DrawingScene::clearBuilding()
{
    this->removeItem(this->buildingsGroup);
    this->buildingsGroup = new QGraphicsItemGroup();
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
