#include "drawingscene.h"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

DrawingScene::DrawingScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->map_width         = 500;
    this->map_height        = 500;
    this->px_per_m          = 2;
    this->grid_spacing_m    = 5;

    this->scene_state = SceneState::Disabled;
    ReceiverRect::changeState(RectState::Power);

    this->temp_building = nullptr;
    this->temp_MS = nullptr;
    this->rx_item = nullptr;
    this->tx_item = nullptr;
    this->main_street = nullptr;

    this->buildingsGroup = nullptr;
    this->raysGroup = nullptr;

    this->rayTracing = new RayTracing(&(this->map_width),&(this->map_width),&(this->px_per_m),&(this->grid_spacing_m));

    this->updateMapSize(this->map_width, this->map_height);

    this->startBuildLabel = nullptr;
    this->currentBuildLabel = new QGraphicsSimpleTextItem();this->currentBuildLabel->setScale(1.5);
    this->power_label = new QGraphicsSimpleTextItem();this->power_label->setScale(1.5);
    this->addItem(currentBuildLabel);
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
    bool ray = true;
    if(this->scene_state == SceneState::Building){
        QRectF temp_building_rect(snapToGrid(&eventPoint),snapToGrid(&eventPoint));
        this->temp_building = new Building(temp_building_rect);
        this->scene_state = SceneState::Building;
        this->startBuildLabel = new QGraphicsSimpleTextItem();this->startBuildLabel->setScale(1.5);
        this->startBuildLabel->setText("("+QString::number(qRound(snapToGrid(&eventPoint).x()/this->px_per_m))+","+QString::number(qRound(snapToGrid(&eventPoint).y()/this->px_per_m))+")");
        this->startBuildLabel->setPos(eventPoint+QPointF(12,-20));
        this->addItem(this->startBuildLabel);
        ray = false;
    }
    else if(this->scene_state == SceneState::RX && pointIsAvailable(eventPoint)){
        this->scene_state = SceneState::Disabled;
        this->rx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::TX && pointIsAvailable(eventPoint)){
        this->scene_state = SceneState::Disabled;
        this->tx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::Simulation){
        ray=false;
    }
    draw(ray);
}

void DrawingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);
    bool ray = true;
    if(this->scene_state == SceneState::Building && this->temp_building){
        QPointF topleft = this->temp_building->rect().topLeft();
        QPointF bottomRight = snapToGrid(&eventPoint);
        if(topleft != bottomRight){
            QRectF temp_building_rect(topleft,bottomRight);
            Building *new_building = new Building(temp_building_rect);
            this->building_list.append(new_building);
        }
        this->removeItem(this->startBuildLabel);
        this->startBuildLabel = nullptr;
        this->temp_building = nullptr;
    }
    else if(this->scene_state == SceneState::Simulation)
    {
        ray = false;
    }
    else {
        this->scene_state = SceneState::Disabled;
        ray = false;
    }
    draw(ray);
}

void DrawingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF eventPoint = eventToTheGrid(event);
    bool ray = true;

    this->currentBuildLabel->setText("("+QString::number(snapToGrid(&eventPoint,2).x()/this->px_per_m)+","+QString::number(snapToGrid(&eventPoint,2).y()/this->px_per_m)+")");
    this->currentBuildLabel->setPos(eventPoint+QPointF(12,-20));

    if(this->scene_state == SceneState::Building){
        ray = false;
        if(this->temp_building){
            QRectF temp_building_rect(this->temp_building->rect().topLeft(),snapToGrid(&eventPoint));
            this->temp_building->setRect(temp_building_rect);
        }
        this->currentBuildLabel->setText("("+QString::number(snapToGrid(&eventPoint).x()/this->px_per_m)+","+QString::number(snapToGrid(&eventPoint).y()/this->px_per_m)+")");
        this->currentBuildLabel->setPos(eventPoint+QPointF(12,-20));

    }
    else if(this->scene_state == SceneState::RX && pointIsAvailable(eventPoint)){
        this->rx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::TX && pointIsAvailable(eventPoint)){
        this->tx_item->setCenter(snapToGrid(&eventPoint,2));
    }
    else if(this->scene_state == SceneState::Disabled){
        ray = false;
    }
    else if(this->scene_state == SceneState::Simulation){
        ray = false;
    }
    draw(ray); //Comment this for easier debugging (click for update)
}

void DrawingScene::draw(bool ray)
{
    if(this->scene_state != SceneState::Simulation){
        //MainStreet
        if(this->main_street){this->removeItem(this->main_street);}
        this->main_street = nullptr;
        if(this->tx_item && this->tx_item->isSet)
        {
            //qDebug() << this->tx_item->center;
            this->rayTracing->findMainStreetQRectF(&(this->tx_item->center), &(this->building_list));
            if(this->rayTracing->mainStreet)
            {
                this->main_street = new QGraphicsRectItem(this->rayTracing->mainStreet->toRect());
                this->main_street->setBrush(Qt::blue);
                this->main_street->setOpacity(0.15);
                this->addItem(this->main_street);
            }
        }
    }
    //Buildings
    if(this->buildingsGroup){this->removeItem(this->buildingsGroup);}
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

    if(ray){
        //Rays
        if(this->raysGroup){this->removeItem(this->raysGroup);}
        this->raysGroup = new QGraphicsItemGroup();
        if(this->tx_item && this->tx_item->isSet && this->rx_item && this->rx_item->isSet)
        {
            rayTracing->drawRays(&(this->tx_item->center), &(this->rx_item->center), &(this->building_list));
            for(Ray* ray:this->rayTracing->raysList)
            {
                this->raysGroup->addToGroup(ray);
            }

        }
        this->addItem(this->raysGroup);
    }

    //BS
    if(this->tx_item){this->removeItem(this->tx_item);this->addItem(this->tx_item);}
    if(this->rx_item){
        this->removeItem(this->rx_item);
        this->addItem(this->rx_item);
        this->removeItem(this->power_label);
        this->power_label->setText(QString::number(qRound(this->rayTracing->received_power_dbm))+" dBm");
        this->power_label->setPos(this->rx_item->pos()+QPointF(10,0));
        this->addItem(this->power_label);
    }

    //Labels
    if(this->startBuildLabel){this->removeItem(this->startBuildLabel);this->addItem(this->startBuildLabel);}
    if(this->currentBuildLabel){this->removeItem(this->currentBuildLabel);this->addItem(this->currentBuildLabel);}

}

void DrawingScene::runSimulation()
{
    this->scene_state = SceneState::Simulation;
    this->rectList.clear();
    this->removeItem(this->main_street);
    int x0 =  0;//this->main_street->rect().x()/this->px_per_m;
    int y0 =  0;//this->main_street->rect().y()/this->px_per_m;
    int w  = this->map_width; //this->main_street->rect().width()/this->px_per_m;
    int h  = this->map_height; //this->main_street->rect().height()/this->px_per_m;

    float x_tx = this->tx_item->center.x()/this->px_per_m;
    float y_tx = this->tx_item->center.y()/this->px_per_m;

    int counter = 0;
    for(int i=x0; i<x0+w; i++){
        for(int j=y0; j<y0+h; j++){
            float x_m = (float)(i+0.5);
            float y_m = (float)(j+0.5);
            if(pointIsAvailable(QPointF(x_m*this->px_per_m, y_m*this->px_per_m))){
                QPointF *RX = new QPointF(x_m*this->px_per_m, y_m*this->px_per_m);
                if(sqrt(pow(x_m-x_tx,2)+pow(y_m-y_tx,2)) > 10){
                    this->rayTracing->drawRays(&this->tx_item->center, RX, &this->building_list);
                    qreal power = this->rayTracing->received_power_dbm;
                    ReceiverRect *rect = new ReceiverRect(i*this->px_per_m, j*this->px_per_m, this->px_per_m, this->px_per_m);
                    rect->power = power;
                    rect->SNR = this->rayTracing->SNR();
                    rect->colorRect();

                    this->rectList.append(rect);
                    this->addItem(rect);
                    update();
                }
            }
            counter += 1;
            emit updateBar(counter);
        }
    }
    emit simulationFinished();

}

void DrawingScene::updateMapSize(int width, int height)
{
    this->map_width  = width;
    this->map_height = height;

    if(this->tx_item || this->rx_item) this->clearBS();
    if(this->buildingsGroup) this->clearBuilding();

    foreach(QGraphicsLineItem *line, this->grid){
        this->removeItem(line);
    }
    this->grid.clear();

    QPen pen = QPen(Qt::gray);
    for (int x=0; x<=this->map_width*this->px_per_m; x+=this->px_per_m*this->grid_spacing_m)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(x,0,x,this->map_height*this->px_per_m);
        line->setPen(pen);
        this->grid.append(line);
        this->addItem(line);
    }

    for (int y=0; y<=this->map_height*this->px_per_m; y+=this->px_per_m*this->grid_spacing_m)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0,y,this->map_width*this->px_per_m,y);
        line->setPen(pen);
        this->grid.append(line);
        this->addItem(line);
    }

    this->setSceneRect(QRectF(0,0,this->map_width*this->px_per_m,this->map_height*this->px_per_m));
   // this->draw(false);
}

void DrawingScene::setSettings(QMap<QString, qreal> dict)
{
    this->rayTracing->setSettings(dict);
    if(this->scene_state == SceneState::Simulation){
        foreach(ReceiverRect* rect, this->rectList){
            // TODO: update map when changing settings ? How ?
        }
    }
}

void DrawingScene::changeMap(int index)
{
    switch(index){
        case 0:
            ReceiverRect::changeState(RectState::Power);
            break;
        case 1:
            ReceiverRect::changeState(RectState::SNR);
            break;
        case 2:
            ReceiverRect::changeState(RectState::Rice);
            break;
        case 3:
            ReceiverRect::changeState(RectState::DelaySpread);
            break;
    }
    if(this->scene_state==SceneState::Simulation){
        foreach(ReceiverRect *rect, this->rectList){
            rect->colorRect();
        }
    }
}

void DrawingScene::clearSimulation()
{
    foreach(ReceiverRect* rect, this->rectList){
        this->removeItem(rect);
    }
    this->rectList.clear();
    this->scene_state = SceneState::Disabled;
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

bool DrawingScene::pointIsAvailable(QPointF point)
{
    for(Building* building:this->building_list)
    {
        if(building->isContainingPoint(point) || building->isContainingPoint(point))
        {return false;}
    }
    return true;
}
void DrawingScene::createRX()
{
    if(this->rx_item){this->removeItem(this->rx_item);}
    this->rx_item = new Point("RX");
    this->addItem(this->rx_item);
    draw();
}

void DrawingScene::addBS()
{
    if(this->tx_item){this->removeItem(this->tx_item);}
    this->tx_item = new Point("TX");
    this->addItem(this->tx_item);
    draw();
}

void DrawingScene::clearBS()
{
    if(this->rx_item){this->removeItem(this->rx_item);}
    if(this->tx_item){this->removeItem(this->tx_item);}
    this->tx_item = nullptr;
    this->rx_item = nullptr;
    draw();
}

void DrawingScene::clearBuilding()
{
    this->building_list.clear();
    draw();
}

Point DrawingScene::getSceneTX()
{
    return this->tx_item;
}

Point DrawingScene::getSceneRX()
{
    return this->rx_item;
}

QList<int> DrawingScene::getMapDim()
{
    QList<int> list;
    list.append(this->map_width);
    list.append(this->map_height);
    return list;
}

QList<Building *> DrawingScene::getSceneBuildingList()
{
    return this->building_list;
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

/*
 * TODO: load map dim when loading project
 * TODO: add rerun of simulation
 * TODO: correct bug when resizing when in placement state
 * TODO: correct bug with diffraction corner
 *
 */
