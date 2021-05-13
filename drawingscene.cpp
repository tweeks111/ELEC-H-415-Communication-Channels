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
    if(this->scene_state == SceneState::Wall){
        QLineF temp_wall_line(qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                              qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                              qRound(event->scenePos().x()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m),
                              qRound(event->scenePos().y()/(this->grid_spacing_m*this->px_per_m))*(this->grid_spacing_m*this->px_per_m));
        this->temp_wall = new Wall(temp_wall_line);
        this->addItem(temp_wall);
        this->scene_state = SceneState::Wall;
    }
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
