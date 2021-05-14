#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <QWidget>
#include "building.h"
#include "ray.h"
#include "complex"
#include "point.h"
#include <QGraphicsItemGroup>

class RayTracing
{

public:
    RayTracing();
    RayTracing(int* map_width, int* map_height, int* px_per_m, int* grid_spacing_m);
    void drawRays(QPointF* tx, QPointF* rx, QList<Building*>* building_list);
    void findMainStreetQRectF(QPointF* tx, QList<Building*>* building_list);
    qreal power;
    QList<Ray*> raysList;
    QRectF* mainStreet;

private:
    //Attributs
    int* map_width;
    int* map_height;
    int* px_per_m;
    int* grid_spacing_m;
    QList<Building*>* building_list;
    QPointF* transmitter;
    QPointF* receiver;

    int counter;
    int counterMax;
    int maxReflection = 3;

    float pi = 3.14159;
    int thickness;
    float relPermittivity = 0; //TODO
    std::complex<qreal> Z; //TODO
    qreal Z0=376.730; //TODO
    std::complex<qreal> gammam; //TODO
    qreal alpham; //TODO
    qreal betam; //TODO
    qreal beta0; //TODO

    //Methods
    bool lineIsBlocked(QLineF* line);
    std::complex<qreal> computeCoef(Ray* ray,QLineF* wall);
    QPointF mirrorPointMaker(QLineF* wall, QPointF* initialPoint);
    void makeDirectAndGroundReflection();
    void makeWallReflection(QList<QPointF> = QList<QPointF>(), QList<QLineF*> = QList<QLineF*>(), qint16 n_reflection = 1);
    void makeDiffraction();
    bool wallIsValid(QLineF*);
    bool cornerIsValid(QPointF* corner);
    bool checkTxRxValidity();
};

#endif // RAYTRACING_H
