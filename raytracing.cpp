#include "raytracing.h"

RayTracing::RayTracing(int* map_width, int* map_height, int* px_per_m, int* grid_spacing_m)
{
    this->map_width         = map_width;
    this->map_height        = map_height;
    this->px_per_m          = px_per_m;
    this->grid_spacing_m    = grid_spacing_m;
    this->raysList = QList<Ray*>();
    this->mainStreet = nullptr;
    this->transmitter = nullptr;
    this->receiver = nullptr;
    this->building_list = nullptr;
}


QPointF RayTracing::mirrorPointMaker(QLineF* wall, QPointF* initialPoint)
{
    float a = (wall->p2().y()-wall->p1().y())/wall->length();
    float b = (wall->p1().x()-wall->p2().x())/wall->length();
    float c = -a*wall->p1().x()-b*wall->p1().y();
    float dist = a*initialPoint->x()+b*initialPoint->y()+c;
    return QPointF(initialPoint->x()-2*a*dist,initialPoint->y()-2*b*dist);
}

bool RayTracing::lineIsBlocked(QLineF* line)
{
    for(Building* building:*this->building_list){if(building->isBlockingPath(line)){return true;}}
    return false;
}

std::complex<qreal> RayTracing::computeCoef(Ray *ray, QLineF* wall)
{
    qreal angle  = ray->line().angleTo(wall->normalVector());
    if(angle>270){angle=360-angle;}
    else if(angle>180){angle=angle-180;}
    else if(angle>90){angle=180-angle;}
    qreal incAngleRad=angle*pi/180;    // incAngle is in degrees => rad
    qreal tranAngle = asin(sqrt(1/relPermittivity)*sin(incAngleRad));
    qreal s = thickness/(100*cos(tranAngle));  // thickness is in cm => /100
    std::complex<qreal> R = ((Z*cos(incAngleRad)-Z0*cos(tranAngle))/(Z*cos(incAngleRad)+Z0*cos(tranAngle)));
    std::complex<qreal> a = -gammam*s;   //(0,-betam*s);
    std::complex<qreal> b(0,beta0*2*s*sin(incAngleRad)*sin(tranAngle));
    std::complex<qreal> Tm = ((1.0-pow(R,2.0))*exp(a))/(1.0-pow(R,2.0)*exp(2.0*a)*exp(b));
    return Tm;
}

void RayTracing::drawRays(QPointF* tx, QPointF* rx, QList<Building*>* building_list)
{
    this->transmitter = tx;
    this->receiver = rx;
    this->building_list = building_list;
    this->raysList.clear();
    qreal power = 0;
    std::complex<qreal> En=0;
    if(checkTxRxValidity())
    {
        this->counter = 1;
        this->counterMax = 1;
        makeDirectAndGroundReflection();
        if(this->mainStreet->contains(*(this->transmitter))){
            makeWallReflection();
            qDebug() << "Main Street Computation";
        }
        else
        {
            if(this->raysList.isEmpty()){
                makeDiffraction();
                qDebug() << "Main Street Computation NLOS";
            }
            qDebug() << "Second Street Computation LOS";
        }
        qDebug() << "Computations" << (this->counter) << "/" << (this->counterMax);
    }
}

void RayTracing::makeDirectAndGroundReflection()
{

    QLineF directLine = QLineF(*(transmitter),*(receiver));
    if(!lineIsBlocked(&directLine)){
        QPen rayPen(QColor(106, 224, 27));
        rayPen.setWidth(2);
        Ray* directRay = new Ray(directLine);
        directRay->coef*=1; //TODO !
        directRay->setPen(rayPen);
        this->raysList.push_back(directRay);
    }
    else{
        //kill directLine
    }
}

void RayTracing::makeWallReflection(QList<QPointF> mirrorPoints,QList<QLineF*> walls, qint16 n_reflection){
    for(Building* building:*this->building_list){
        for(QLineF wall:*(building->getWalls())){
            if((walls.isEmpty() || wall!=*(walls.last())))
            {
                this->counterMax ++;
                if(wallIsValid(&wall)){
                    this->counter ++;
                    //Compute the new mirror pt based on the last one (tx pos for the 1st reflection)
                    QList<QLineF*> tempWalls = walls;
                    tempWalls.push_back(&wall);
                    QList<QPointF> tempMirrorPoints = mirrorPoints;
                    QPointF mirrorPoint;
                    if(tempMirrorPoints.isEmpty()){
                        mirrorPoint = mirrorPointMaker(&wall, transmitter);
                    }
                    else{
                        mirrorPoint = mirrorPointMaker(&wall, &(tempMirrorPoints.last()));
                    }
                    tempMirrorPoints.push_back(mirrorPoint);

                    //Compute the n_reflection for the current list of mirrorPoints and walls
                    QList<Ray*> rays;
                    QPointF lastIntersectionPoint = *(receiver);
                    int n_mirror = tempMirrorPoints.size();
                    for(qint16 i = 0; i<=n_reflection; i++){
                        QLineF lineLIPtoIP;
                        if(i != n_reflection){//Ray reflecting on a wall
                            QLineF* currentWall = tempWalls.value(tempWalls.size()-(i+1));
                            QLineF lineLIPtoMP(lastIntersectionPoint,tempMirrorPoints.value(n_mirror-(i+1)));
                            QPointF intersectionPoint;
                            if(currentWall->intersects(lineLIPtoMP,&intersectionPoint)==QLineF::BoundedIntersection){//Is there a intersection on the wall ?
                                lineLIPtoIP = QLineF(lastIntersectionPoint,intersectionPoint);
                                if(!lineIsBlocked(&lineLIPtoIP)){//Is there no other wall blocking the ray ?
                                    Ray* ray = new Ray(lineLIPtoIP);
                                    ray->coef*=computeCoef(ray,&wall);
                                    //Also do RX coef
                                    rays.push_back(ray);
                                    lastIntersectionPoint = intersectionPoint;
                                }else{break;}
                            }else{break;}
                        }
                        else{//Last ray, going from last mirror point to TX
                            lineLIPtoIP = QLineF(lastIntersectionPoint,*(transmitter));
                            if(!lineIsBlocked(&lineLIPtoIP)){//Is there no other wall blocking the ray ?
                                Ray* ray = new Ray(lineLIPtoIP);
                                ray->coef*=computeCoef(ray,&wall);
                                rays.push_back(ray);
                            }else{break;}
                        }
                    }
                    if(rays.size() == n_reflection +1){//Valide ray path (no rays intersects the walls)
                        QPen rayPen;
                        if(rays.size() == 2){
                            rayPen = QPen(QColor(224, 221, 27));
                        }
                        else if(rays.size() == 3){
                            rayPen = QPen(QColor(224, 152, 27));
                        }
                        else if(rays.size() >= 4){
                            rayPen = QPen(QColor(224, 27, 27));
                        }
                        rayPen.setWidth(2);
                        //std::complex<qreal> En = EnCalcultor(rays);
                        //qreal power = (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
                        for(Ray* ray:rays){
                            ray->setPen(rayPen);
                            this->raysList.push_back(ray);
                        }
                    }
                    //More reflections iterations
                    if(n_reflection < maxReflection){
                        makeWallReflection(tempMirrorPoints,tempWalls,n_reflection+1);
                    }
                }
            }
        }
    }
}

void RayTracing::makeDiffraction()
{
    QPen rayPen(QColor(155, 0, 233)); //print the direct ray "- - -"
    rayPen.setWidth(2);
    for(Building* building:*building_list){
        for(QPointF corner:*(building->getCorners())){
            this->counterMax ++;
            if(cornerIsValid(&corner))
            {
                this->counter ++;
                QLineF lineTXtoEP(*(transmitter),corner);
                QLineF lineEPtoRX(corner,*(receiver));
                if(!lineIsBlocked(&lineTXtoEP) && !lineIsBlocked(&lineEPtoRX) && (lineTXtoEP.angleTo(lineEPtoRX)<=90 || lineTXtoEP.angleTo(lineEPtoRX)>=270)){
                    Ray* rayTXtoDP = new Ray(lineTXtoEP);
                    Ray* rayDPtoRX = new Ray(lineEPtoRX);
                    rayTXtoDP->setPen(rayPen);
                    rayDPtoRX->setPen(rayPen);
                    this->raysList.push_back(rayTXtoDP);
                    this->raysList.push_back(rayDPtoRX);
                }
            }
        }
    }
}

void RayTracing::findMainStreetQRectF(QPointF* tx, QList<Building*>* building_list)
{
    this->transmitter = tx;
    this->building_list = building_list;
    if(this->transmitter)
    {
        QPointF v_up = QPointF(this->transmitter->x(),*(this->map_height)**(this->px_per_m));
        QPointF v_down = QPointF(this->transmitter->x(),0);
        QPointF h_up = QPointF(*(this->map_width)**(this->px_per_m),this->transmitter->y());
        QPointF h_down = QPointF(0,this->transmitter->y());
        QLineF v_line = QLineF(v_up,v_down);
        QLineF h_line = QLineF(h_up,h_down);
        for(Building* building:*(this->building_list)){
            for(QLineF wall:*(building->getWalls())){
                if(wallIsValid(&wall)){
                    QPointF intersectionPointh;
                    if(wall.intersects(h_line,&intersectionPointh)==QLineF::BoundedIntersection){//horizontal
                        if(intersectionPointh.x() >= this->transmitter->x() && intersectionPointh.x() < h_up.x()){
                            h_up.setX(intersectionPointh.x());
                        }
                        else if(intersectionPointh.x() <= this->transmitter->x() && intersectionPointh.x() > h_down.x()){
                            h_down.setX(intersectionPointh.x());
                        }
                    }
                    QPointF intersectionPointv;
                    if(wall.intersects(v_line,&intersectionPointv)==QLineF::BoundedIntersection){//vertical
                        if(intersectionPointv.y() >= this->transmitter->y() && intersectionPointv.y() < v_up.y()){
                            v_up.setY(intersectionPointv.y());
                        }
                        else if(intersectionPointv.y() <= this->transmitter->y() && intersectionPointv.y() > v_down.y()){
                            v_down.setY(intersectionPointv.y());
                        }
                    }
                }
            }
        }
        this->mainStreet = new QRectF(QPointF(h_down.x(),v_down.y()),QPointF(h_up.x(),v_up.y()));
    }
    else
    {
        this->mainStreet = nullptr;
    }
}

bool RayTracing::wallIsValid(QLineF* wall)
{
    return !((wall->dx() == 0 && (wall->x1() == 0 || wall->x1() == *(this->map_width))) || (wall->dy() == 0 && (wall->y1() == 0 || wall->y1() == *(this->map_height))));

}

bool RayTracing::cornerIsValid(QPointF* corner)
{
    return !((corner->x() == 0 || corner->x() == *(this->map_width)) || (corner->y() == 0 || corner->y() == *(this->map_height)));

}

bool RayTracing::checkTxRxValidity()
{
    if(this->transmitter && this->receiver)
    {
        for(Building* building:*(this->building_list))
        {
            if(building->isContainingPoint(*(this->transmitter)) || building->isContainingPoint(*(this->transmitter)))
            {return false;}
            if(building->isContainingPoint(*(this->transmitter)) || building->isContainingPoint(*(this->transmitter)))
            {return false;}
        }
        return true;
    }
    else{return false;}
}




