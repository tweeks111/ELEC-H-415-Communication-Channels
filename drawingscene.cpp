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

    for (int x=0; x<=this->map_width*this->px_per_m; x+=this->px_per_m*this->grid_spacing_m)
       this->addLine(x,0,x,this->map_height*this->px_per_m, QPen(Qt::gray));

    for (int y=0; y<=this->map_height*this->px_per_m; y+=this->px_per_m*this->grid_spacing_m)
        this->addLine(0,y,this->map_width*this->px_per_m,y, QPen(Qt::gray));

    this->scene_state = SceneState::Disabled;
    this->temp_wall = nullptr;
    this->temp_building = nullptr;
    this->rx_item = nullptr;
    this->tx_item = nullptr;
    this->rayTracing = new RayTracing(&(this->building_list));
}

DrawingScene::~DrawingScene()
{

}

void DrawingScene::setSceneState(SceneState::SceneState state)
{
    this->scene_state=state;
}

void DrawingScene::createRX(){
    this->removeItem(this->rx_item);
    this->rx_item = new Point("RX");
    this->addItem(this->rx_item);
}

void DrawingScene::addBS()
{
    this->tx_item = new Point("TX");
    this->addItem(tx_item);
    this->TX_items.append(tx_item);
}

void DrawingScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->scene_state == SceneState::Wall){
        QLineF temp_wall_line(qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                              qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                              qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                              qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m));
        this->temp_wall = new Wall(temp_wall_line);
        this->addItem(temp_wall);
        this->scene_state = SceneState::Wall;
    }
    else if(this->scene_state == SceneState::Building){
        QRectF temp_building_rect(qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                             qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                             0,
                             0);
        this->temp_building = new Building(temp_building_rect);
        this->addItem(temp_building);
        this->scene_state = SceneState::Building;
    }
    else if(this->scene_state == SceneState::RX){
        this->scene_state = SceneState::Disabled;
        this->rx_item->setCenter(QPointF(qRound(2*event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2),
                                      qRound(2*event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2)));
    }
    else if(this->scene_state == SceneState::TX){
        this->scene_state = SceneState::Disabled;
        this->tx_item->setCenter(QPointF(qRound(2*event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2),
                                      qRound(2*event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2)));
    }
    draw();
}

void DrawingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->scene_state == SceneState::Wall){
        int x1 = this->temp_wall->line().x1();
        int y1 = this->temp_wall->line().y1();
        int x2 = qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m);
        int y2 = qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m);
        if(x1 != x2 || y1 != y2){
            QLineF temp_wall_line(x1,y1,x2,y2);
            Wall *new_wall = new Wall(temp_wall_line);
            this->wall_list.append(new_wall);
            this->addItem(new_wall);
        }
        this->removeItem(this->temp_wall);
        this->temp_wall = nullptr;
        qDebug() << this->wall_list.size();
    }
    else if(this->scene_state == SceneState::Building){
        int x1 = this->temp_building->rect().topLeft().x();
        int y1 = this->temp_building->rect().topLeft().y();
        int x2 = qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m);
        int y2 = qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m);
        if(x1 != x2 || y1 != y2){
            QRectF temp_building_rect(x1,y1,(x2-x1),(y2-y1));
            Building *new_building = new Building(temp_building_rect);
            this->building_list.append(new_building);
            this->addItem(new_building);
        }
        this->removeItem(this->temp_building);
        this->temp_building = nullptr;
        qDebug() << this->building_list.size();
    }
    draw();
}

void DrawingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->scene_state == SceneState::Wall && this->temp_wall){
        QLineF temp_wall_line(this->temp_wall->line().x1(),
                              this->temp_wall->line().y1(),
                              event->scenePos().x(),
                              event->scenePos().y());
        this->temp_wall->setLine(temp_wall_line);
    }
    else if(this->scene_state == SceneState::Building && this->temp_building){
        QRectF temp_building_rect(this->temp_building->rect().topLeft().x(),
                              this->temp_building->rect().topLeft().y(),
                              (event->scenePos().x()-this->temp_building->rect().topLeft().x()),
                              (event->scenePos().y()-this->temp_building->rect().topLeft().y()));
        this->temp_building->setRect(temp_building_rect);
    }
    else if(this->scene_state == SceneState::RX){
        this->rx_item->setCenter(QPointF(qRound(2*event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2),
                                      qRound(2*event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2)));
    }
    else if(this->scene_state == SceneState::TX){
        this->tx_item->setCenter(QPointF(qRound(2*event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2),
                                      qRound(2*event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m/2)));
    }
    //draw(); //Comment this for easier debugging
}


bool DrawingScene::checkTxRxValidity()
{
    if(this->rx_item && this->tx_item && this->rx_item->isSet && this->tx_item->isSet)
    {
        for(Building* building:this->building_list)
        {
            if(building->rect().contains(this->tx_item->center) || building->rect().contains(this->rx_item->center))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }

}

void DrawingScene::draw(){
    if(rayTracing->raysGroup){
        removeItem(rayTracing->raysGroup);
        destroyItemGroup(rayTracing->raysGroup);
        rayTracing->raysGroup = nullptr;
    }
    if(checkTxRxValidity()){
        rayTracing->raysGroup = new QGraphicsItemGroup();
        addItem(rayTracing->raysGroup);
        rayTracing->drawRays(&(this->tx_item->center),&(this->rx_item->center));
    }
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
