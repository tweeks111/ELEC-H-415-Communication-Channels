#ifndef LEGENDSCENE_H
#define LEGENDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>


class LegendScene : public QGraphicsScene
{
public:
    LegendScene(QObject *parent = nullptr);
    ~LegendScene();
    void drawScale();
    void changeLegend();
private:
    QLinearGradient* grad;
    QGraphicsRectItem* scaleRect;
    QGraphicsTextItem* minLabel;
    QGraphicsTextItem* maxLabel;

    int powMax=-40;
    int powMin=-150;
    int SNRMax=30;
    int SNRMin=-100;
    float riceMax=0;
    float riceMin=-20;
    float dsMax=1000;
    float dsMin=0;
};

#endif // LEGENDSCENE_H
