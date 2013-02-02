#ifndef TEAMAREA_H
#define TEAMAREA_H

#include <QGraphicsObject>
#include "data/Team.h"
class TeamArea : public QGraphicsObject
{
    Q_OBJECT
public:
    TeamArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addRoundBy1(){round=round>=99?0:round+1;}

private:
    Team* red,*blue;
    QPixmap background;
    QPixmap Rgrail,Bgrail,Pcrystal,Pgem,Rdice[6],Bdice[6],Num[10];
    int width;
    int height;
    int avg;
    int round;
};

#endif // TEAMAREA_H
