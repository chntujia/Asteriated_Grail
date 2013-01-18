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

private:
    Team* red,*blue;
    QPixmap background;
    QPixmap Rgrail,Bgrail,Pcrystal,Pgem,Rdice[6],Bdice[6];
    int width;
    int height;
    int avg;

};

#endif // TEAMAREA_H
