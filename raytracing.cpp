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

    this->GtxMax = this->Gtx(pi/2);
    this->Ptx = this->EIRPmax/this->GtxMax;
    this->heMax = this->he(pi/2);
    this->beta = (2*pi*frequency/c);
}


qreal RayTracing::Gtx(qreal theta)
{
    return 16/(3*pi)*pow(sin(theta),3);
}

qreal RayTracing::he(qreal theta)
{
    return -(c/(frequency*pi)) * cos((pi/2)*cos(theta)) / pow(sin(theta),2);
}

qreal RayTracing::coefReflWall(qreal theta)//perpendicular coef
{
    if(theta < 0){theta = - theta;}
    if(theta < pi/2){theta = pi/2 - theta;}
    else if(theta > pi/2){theta = theta - pi/2;}
    qreal temp = sqrt(1 - (1/relPermittivity)*pow(sin(theta),2))*sqrt(relPermittivity);
    return (cos(theta)-temp)/(cos(theta)+temp);
}

qreal RayTracing::coefReflGround(qreal theta)//parra
{
    qreal temp = sqrt(1 - (1/relPermittivity)*pow(sin(theta),2))/sqrt(relPermittivity);
    return (cos(theta)-temp)/(cos(theta)+temp);
}

std::complex<qreal> RayTracing::coefDiff(qreal Dr)
{
    qreal nu = sqrt(2*beta*Dr/pi);
    return std::polar(pow(10,(-6.9 -20*log10(sqrt(pow(nu - 0.1,2)+1)+nu-0.1))/10),-(pi/4)-(pi/2)*pow(nu,2));
}

void RayTracing::drawRays(QPointF* tx, QPointF* rx, QList<Building*>* building_list)
{
    this->transmitter = tx;
    this->receiver = rx;
    this->building_list = building_list;
    this->raysList.clear();
    this->los_tension_mod = 0;
    this->nlos_tension_mod = 0;
    this->tension = 0;
    this->delay_max = 0;
    this->delay_min = INFINITE;
    this->delay_spread = 0;
    this->received_power = 0;
    this->received_power_dbm = 0;
    this->rice_factor = 0;
    if(checkTxRxValidity())
    {
        this->counterRefl = 0;
        this->counterDiff = 0;
        this->counterReflMax = 0;
        this->counterDiffMax = 0;
        makeDirectAndGroundReflection();
        if(this->mainStreet->contains(*(this->receiver))){
            makeWallReflection();
            //qDebug() << "Main Street Computation";
            //qDebug() << "Reflexions " << (this->counterRefl) << "/" << (this->counterReflMax);
        }
        else
        {
            if(this->raysList.isEmpty()){
                makeDiffraction();
                //qDebug() << "Second Street Computation NLOS";
                //qDebug() << "Diffraction " << (this->counterDiff) << "/" << (this->counterDiffMax);
            }
            else {
            //qDebug() << "Second Street Computation LOS";
            }
        }     
    }
    if(this->los_tension_mod != 0){
        this->rice_factor = this->los_tension_mod / this->nlos_tension_mod;
    }
    this->rice_factor = 10*log10(this->rice_factor);
    this->received_power = (1/(2*Ra)) * pow(abs(this->tension),2);
    this->received_power_dbm = 10*log10(this->received_power/0.01);
}

qreal RayTracing::SNR()
{
    qreal power_dbw = this->received_power_dbm - 30;
    qreal SNR = power_dbw - 10*log10(noise_figure) - 10*log10(boltzman*BW*temp);
    return SNR;
}

void RayTracing::setSettings(QMap<QString, qreal> dict)
{
    this->h = dict["height"];
    this->noise_figure = dict["noisefig"];
    this->BW = dict["BW"];
    this->temp = dict["temp"];
}

void RayTracing::makeDirectAndGroundReflection()
{
    QLineF directLine = QLineF(*(transmitter),*(receiver));
    if(!lineIsBlocked(&directLine)){
        std::complex<qreal> E_direct = std::polar(sqrt(60*Ptx*GtxMax)/(directLine.length()/ *(this->px_per_m)),-beta*(directLine.length()/ *(this->px_per_m)));
        std::complex<qreal> T_direct = E_direct*heMax;
        qreal d_ground = sqrt(pow(directLine.length()/ *(this->px_per_m),2)+pow(2*h,2));
        qreal theta_ground = pi/2 - atan((2*h)/directLine.length() * *(this->px_per_m));
        qreal coef = coefReflGround(theta_ground);
        std::complex<qreal> E_ground = std::polar(coef*sqrt(60*Ptx*Gtx(theta_ground))/(d_ground),-beta*(d_ground));
        std::complex<qreal> T_ground = E_ground * he(theta_ground) * sin(theta_ground);
        this->tension = T_direct + T_ground;
        this->los_tension_mod = norm(T_direct);
        this->nlos_tension_mod = norm(T_ground);


        QPen rayPen(QColor(106, 224, 27));
        rayPen.setWidth(2);
        Ray* directRay = new Ray(directLine);
        directRay->coef*=1;
        directRay->setPen(rayPen);
        this->raysList.push_back(directRay);
    }
    else{
        //kill directLine
    }
}

void RayTracing::makeWallReflection(QList<QPointF> mirrorPoints,QList<QLineF*> walls, qint16 n_reflection){
    QPen rayPen;
    if(n_reflection == 1){
        rayPen = QPen(QColor(224, 221, 27));
    }
    else if(n_reflection == 2){
        rayPen = QPen(QColor(224, 152, 27));
    }
    else if(n_reflection >= 3 ){
        rayPen = QPen(QColor(224, 27, 27));
    }
    rayPen.setWidth(2);
    for(Building* building:*this->building_list){
        for(QLineF wall:*(building->getWalls())){
            if((walls.isEmpty() || wall!=*(walls.last())))
            {
                this->counterReflMax ++;
                if(wallIsValid(wall)){
                    this->counterRefl ++;
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
                    QList<Ray*> rays = QList<Ray*>();
                    qreal coef = 1;
                    qreal total_length = 0;
                    QPointF lastIntersectionPoint = *(receiver);
                    int n_mirror = tempMirrorPoints.size();
                    for(qint16 i = 0; i<=n_reflection; i++){
                        QLineF lineLIPtoIP;
                        if(i != n_reflection){//Ray reflecting on a wall
                            QLineF* currentWall = tempWalls.value(tempWalls.size()-(i+1));
                            QLineF lineLIPtoMP(lastIntersectionPoint,tempMirrorPoints.value(n_mirror-(i+1)));
                            QPointF intersectionPoint;
                            if(currentWall->intersects(lineLIPtoMP,&intersectionPoint)==QLineF::BoundedIntersection && intersectionPoint != lastIntersectionPoint){//Is there a intersection on the wall ?
                                lineLIPtoIP = QLineF(lastIntersectionPoint,intersectionPoint);
                                if(!lineIsBlocked(&lineLIPtoIP)){//Is there no other wall blocking the ray ?
                                    Ray* ray = new Ray(lineLIPtoIP);
                                    qreal angleD = lineLIPtoIP.angleTo(*currentWall);
                                    qreal incidenceAngle = (pi/180)*angleD;
                                    coef *= coefReflWall(incidenceAngle);
                                    total_length += lineLIPtoIP.length();
                                    rays.push_back(ray);
                                    lastIntersectionPoint = intersectionPoint;
                                }else{break;}
                            }else{break;}
                        }
                        else{//Last ray, going from last mirror point to TX
                            lineLIPtoIP = QLineF(lastIntersectionPoint,*(transmitter));
                            if(!lineIsBlocked(&lineLIPtoIP)){//Is there no other wall blocking the ray ?
                                Ray* ray = new Ray(lineLIPtoIP);
                                total_length += lineLIPtoIP.length();
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
                        std::complex<qreal> E_refl = std::polar(coef*sqrt(60* GtxMax * Ptx )/ (total_length/ *(this->px_per_m)), (-this->beta*(total_length/(*(this->px_per_m)))));
                        std::complex<qreal> T_refl = E_refl * heMax;
                        this->tension += T_refl;
                        this->nlos_tension_mod += norm(T_refl);
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
            this->counterDiffMax ++;
            if(cornerIsValid(corner))
            {
                this->counterDiff ++;
                QLineF lineTXtoEP(*(transmitter),corner);
                QLineF lineEPtoRX(corner,*(receiver));

                QPointF point1 = makeNormalPoint(lineTXtoEP);
                QPointF point2 = makeNormalPoint(lineEPtoRX);

                QPointF checkPoint1 = corner+2*point1;
                QPointF checkPoint2 = corner+2*point2;
                qreal angle = lineTXtoEP.angleTo(lineEPtoRX);
                bool diff = false;
                if(checkPoint1 == checkPoint2 || (checkPoint1.y() == checkPoint2.y() && (checkPoint1.x() == corner.x() || checkPoint2.x() == corner.x())) || (checkPoint1.x() == checkPoint2.x() && (checkPoint1.y() == corner.y() || checkPoint2.y() == corner.y()))){
                    if(building->rect().toRect().contains(checkPoint1.toPoint())){
                        if(angle<=90){
                            diff = true;
                        }
                    } else {
                        if(angle>=270){
                            diff = true;
                        }
                    }
                 }
                if(diff && !lineIsBlocked(&lineTXtoEP) && !lineIsBlocked(&lineEPtoRX))
                {
                    Ray* rayTXtoDP = new Ray(lineTXtoEP);
                    Ray* rayDPtoRX = new Ray(lineEPtoRX);
                    QLineF direct = QLineF(*(this->transmitter),*(this->receiver));
                    qreal startAngle = direct.angleTo(lineTXtoEP)*(pi/180);
                    qreal h = lineTXtoEP.length()/ *(this->px_per_m) * abs(sin(startAngle));
                    qreal Dr = (pow(h,2)/2) * ( 1/(lineTXtoEP.length()/ *(this->px_per_m)) + 1/(lineEPtoRX.length()/ *(this->px_per_m)) );
                    std::complex<qreal> F = coefDiff(Dr);
                    qreal total_length = (lineTXtoEP.length() + lineEPtoRX.length())/ *(this->px_per_m);
                    std::complex<qreal> E_diff = std::polar(sqrt(abs(F))*sqrt(60* GtxMax * Ptx )/ total_length, (-this->beta*total_length) + arg(F));
                    std::complex<qreal> T_diff = E_diff * heMax;
                    this->tension += T_diff;
                    this->nlos_tension_mod += norm(T_diff);
                    rayTXtoDP->setPen(rayPen);
                    rayDPtoRX->setPen(rayPen);
                    this->raysList.push_back(rayTXtoDP);
                    this->raysList.push_back(rayDPtoRX);
                }
            }
        }
    }
}

void RayTracing::delayCheck(qreal length){
    qreal delay = length / c;
    if(delay < this->delay_min){
        this->delay_min = delay;
    }
    if(delay > this->delay_max){
        this->delay_max = delay;
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
            if(building->isContainingPoint(*(this->transmitter))){
                this->mainStreet = nullptr;
                return;
            }
            for(QLineF wall:*(building->getWalls())){
                if(wallIsValid(wall)){
                    QPointF intersectionPointh;
                    if(wall.intersects(h_line,&intersectionPointh)==QLineF::BoundedIntersection){//horizontal
                        if(intersectionPointh.x() == this->transmitter->x())
                        {
                            if(building->isContainingPoint(intersectionPointh+QPointF(2,0)) && intersectionPointh.x() < h_up.x()){
                                h_up.setX(intersectionPointh.x());
                            }
                            if(building->isContainingPoint(intersectionPointh+QPointF(-2,0)) && intersectionPointh.x() > h_down.x()){
                                h_down.setX(intersectionPointh.x());
                            }
                        }
                        if(intersectionPointh.x() > this->transmitter->x() && intersectionPointh.x() < h_up.x()){//up inter
                            if(building->isContainingPoint(intersectionPointh + QPointF(2,0))){
                                h_up.setX(intersectionPointh.x());
                            }
                        }
                        else if(intersectionPointh.x() < this->transmitter->x() && intersectionPointh.x() > h_down.x()){//down inter
                            if(building->isContainingPoint(intersectionPointh + QPointF(-2,0))){
                                h_down.setX(intersectionPointh.x());
                            }
                        }
                    }
                    QPointF intersectionPointv;
                    if(wall.intersects(v_line,&intersectionPointv)==QLineF::BoundedIntersection){//vertical
                        if(intersectionPointv.y() == this->transmitter->y())
                        {
                            if(building->isContainingPoint(intersectionPointv+QPointF(0,2)) && intersectionPointv.y() < v_up.y()){
                                v_up.setY(intersectionPointv.y());
                            }
                            if(building->isContainingPoint(intersectionPointv+QPointF(0,-2)) && intersectionPointv.y() > v_down.y()){
                                v_down.setY(intersectionPointv.y());
                            }
                        }
                        if(intersectionPointv.y() > this->transmitter->y() && intersectionPointv.y() < v_up.y()){//up inter
                            if(building->isContainingPoint(intersectionPointv + QPointF(0,2))){
                                v_up.setY(intersectionPointv.y());
                            }
                        }
                        else if(intersectionPointv.y() < this->transmitter->y() && intersectionPointv.y() > v_down.y()){//down inter
                            if(building->isContainingPoint(intersectionPointv + QPointF(0,-2))){
                                v_down.setY(intersectionPointv.y());
                            }
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

bool RayTracing::wallIsValid(QLineF wall)
{
    return !((wall.dx() == 0 && (wall.x1() == 0 || wall.x1() == *(this->map_width)**(this->px_per_m))) || (wall.dy() == 0 && (wall.y1() == 0 || wall.y1() == *(this->map_height)**(this->px_per_m))));

}

bool RayTracing::cornerIsValid(QPointF corner)
{
    return !((corner.x() == 0 || corner.x() == *(this->map_width)**(this->px_per_m)) || (corner.y() == 0 || corner.y() == *(this->map_height)**(this->px_per_m)));

}

bool RayTracing::checkTxRxValidity()
{
    if(this->transmitter && this->receiver)
    {
        for(Building* building:*(this->building_list))
        {
            if(building->isContainingPoint(*(this->transmitter)) || building->isContainingPoint(*(this->receiver)))
            {return false;}
        }
        return true;
    }
    else{return false;}
}

QPointF RayTracing::makeNormalPoint(QLineF line)
{
    QPointF point = QPointF((line.normalVector().dx())/abs(line.normalVector().dx()),(line.normalVector().dy())/abs(line.normalVector().dy()));
    if(point.x() != point.x()){
        point.setX(0);
    }
    if(point.y() != point.y()){
        point.setY(0);
    }
    return point;
}


 //TODO Fixed main street again (max heigth broken some times)

