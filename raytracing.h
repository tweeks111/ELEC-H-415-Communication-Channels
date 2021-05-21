#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <QWidget>
#include "building.h"
#include "ray.h"
#include "complex"
#include "point.h"
#include <QGraphicsItemGroup>

#ifndef pi
#define pi  3.14159
#endif

class RayTracing
{

public:
    RayTracing();
    RayTracing(int* map_width, int* map_height, int* px_per_m, int* grid_spacing_m);
    void drawRays(QPointF* tx, QPointF* rx, QList<Building*>* building_list);
    void findMainStreetQRectF(QPointF* tx, QList<Building*>* building_list);
    QList<Ray*> raysList;
    QRectF* mainStreet;
    qreal received_power_dbm;

    qreal SNR();
    void setSettings(QMap<QString, qreal>);

private:
    //Attributs
    int* map_width;
    int* map_height;
    int* px_per_m;
    int* grid_spacing_m;
    QList<Building*>* building_list;
    QPointF* transmitter;
    QPointF* receiver;

    int counterRefl;
    int counterDiff;
    int counterReflMax;
    int counterDiffMax;
    int maxReflection = 3;

    qreal const c = 299792458;
    const qreal boltzman = 1.379*1e-23;
    qreal BW = 200*1e6;


    qreal frequency=27e9;
    qreal relPermittivity = 5;
    qreal h = 2;
    qreal Ra = 71;//check
    qreal EIRPmax = 2;
    qreal noise_figure = 10;
    qreal temp = 293.15;


    qreal beta;
    qreal Ptx;
    qreal GtxMax;
    qreal heMax;
    std::complex<qreal> tension;
    qreal received_power;


    qreal he(qreal theta);
    qreal Gtx(qreal theta);

    qreal coefReflWall(qreal theta);
    qreal coefReflGround(qreal theta);
    std::complex<qreal> coefDiff(qreal Dr);
    std::complex<qreal> tension_comp(qreal distance, qreal coef = 1, qreal incidence_angle = pi/2, qreal dephasage=0);

    //Methods
    bool lineIsBlocked(QLineF* line);
    QPointF mirrorPointMaker(QLineF* wall, QPointF* initialPoint);
    void makeDirectAndGroundReflection();
    void makeWallReflection(QList<QPointF> = QList<QPointF>(), QList<QLineF*> = QList<QLineF*>(), qint16 n_reflection = 1);
    void makeDiffraction();
    bool wallIsValid(QLineF wall);
    bool cornerIsValid(QPointF corner);
    bool checkTxRxValidity();

    QPointF makeNormalPoint(QLineF line);
};

#endif // RAYTRACING_H
