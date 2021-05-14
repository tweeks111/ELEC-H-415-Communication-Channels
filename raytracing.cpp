#include "raytracing.h"

RayTracing::RayTracing(QList<Building*>* building_list)
{
    this->building_list = building_list;
    this->raysGroup = nullptr;
    this->map_width         = 600;
    this->map_height        = 300;
    this->px_per_m          = 2;
    this->grid_spacing_m    = 5;
    this->raysList = QList<Ray*>();
}


QPointF RayTracing::mirrorPointMaker(QLineF* wall, QPointF* initialPoint)
{
    float a = (wall->p2().y()-wall->p1().y())/wall->length();
    float b = (wall->p1().x()-wall->p2().x())/wall->length();
    float c = -a*wall->p1().x()-b*wall->p1().y();
    float dist = a*initialPoint->x()+b*initialPoint->y()+c;
    QPointF mirrorPoint(initialPoint->x()-2*a*dist,initialPoint->y()-2*b*dist);
    return mirrorPoint;
}

bool RayTracing::lineIsBlocked(QLineF* line)
{
    for(Building* building:*this->building_list){
        if(building->isBlockingPath(line)){
            return true;
        }
    }
    return false;
}

std::complex<qreal> RayTracing::computeCoef(Ray *ray, QLineF* wall)
{
    qreal angle  = ray->line().angleTo(wall->normalVector());
    if(angle>270){
        angle=360-angle;
    }
    else if(angle>180){
        angle=angle-180;
    }
    else if(angle>90){
        angle=180-angle;
    }
    qreal incAngleRad=angle*pi/180;    // incAngle is in degrees => rad
    qreal tranAngle = asin(sqrt(1/relPermittivity)*sin(incAngleRad));
    qreal s = thickness/(100*cos(tranAngle));  // thickness is in cm => /100
    std::complex<qreal> R = ((Z*cos(incAngleRad)-Z0*cos(tranAngle))/(Z*cos(incAngleRad)+Z0*cos(tranAngle)));
    std::complex<qreal> a = -gammam*s;   //(0,-betam*s);
    std::complex<qreal> b(0,beta0*2*s*sin(incAngleRad)*sin(tranAngle));
    std::complex<qreal> Tm = ((1.0-pow(R,2.0))*exp(a))/(1.0-pow(R,2.0)*exp(2.0*a)*exp(b));
    return Tm;
}

void RayTracing::drawRays(QPointF* transmitter, QPointF* receiver)
{
    this->raysList.clear();
    this->transmitter = transmitter;
    this->receiver = receiver;
    qreal power = 0;
    std::complex<qreal> En=0;
    this->counter = 0;
    makeDirectAndGroundReflection();
    if(raysGroup->childItems().isEmpty()) //Add condition for main street and secondary street
    {
        makeDiffraction();
    }
    else
    {
        makeWallReflection();
    }
    qDebug() << "Counter" << (this->counter);
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
        raysGroup->addToGroup(directRay);
        this->raysList.push_back(directRay);
    }
    else{
        //kill directLine
    }
}

void RayTracing::makeWallReflection(QList<QPointF> mirrorPoints,QList<QLineF*> walls, qint16 n_reflection){
    for(Building* building:*this->building_list){
        for(QLineF wall:*(building->getWalls())){
            if(wallIsValid(&wall) && (walls.isEmpty() || wall!=*(walls.last()))){
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
                        raysGroup->addToGroup(ray);
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

void RayTracing::makeDiffraction()
{
    QPen rayPen(QColor(155, 0, 233)); //print the direct ray "- - -"
    rayPen.setWidth(2);
    bool thereIsDiffraction = false;
    for(Building* building:*building_list){
        for(QPointF corner:*(building->getCorners())){
            QLineF lineTXtoEP(*(transmitter),corner);
            QLineF lineEPtoRX(corner,*(receiver));
            if(!lineIsBlocked(&lineTXtoEP) && !lineIsBlocked(&lineEPtoRX) && (lineTXtoEP.angleTo(lineEPtoRX)<=90 || lineTXtoEP.angleTo(lineEPtoRX)>=270)){
                Ray* rayTXtoDP = new Ray(lineTXtoEP);
                Ray* rayDPtoRX = new Ray(lineEPtoRX);
                rayTXtoDP->setPen(rayPen);
                rayDPtoRX->setPen(rayPen);
                raysGroup->addToGroup(rayTXtoDP);
                raysGroup->addToGroup(rayDPtoRX);
                this->raysList.push_back(rayTXtoDP);
                this->raysList.push_back(rayDPtoRX);
                thereIsDiffraction = true;
            }
        }
    }
    if(thereIsDiffraction)
    {
        QPen rayPen(Qt::gray);
        rayPen.setStyle(Qt::DashLine);
        Ray* directRay = new Ray(QLineF(*(this->transmitter),*(this->receiver)));
        directRay->setPen(rayPen);
        this->raysGroup->addToGroup(directRay);
    }
}

bool RayTracing::wallIsValid(QLineF* wall)
{
    return !((wall->dx() == 0 && (wall->x1() == 0 || wall->x1() == this->map_width)) || (wall->dy() == 0 && (wall->y1() == 0 || wall->y1() == this->map_height)));

}






