#ifndef ANIMATION_H
#define ANIMATION_H
#include "RoomView.h"
#include "CardItem.h"
#include <QPixmap>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsObject>
#include <QObject>
#include <QMutex>

class PictureContainer:public QGraphicsObject
{
public:
    PictureContainer();
    void setPixmap(QString picSrc){pixmap.load(picSrc);width = pixmap.width();height = pixmap.height();}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QPixmap pixmap;
    int width;
    int height;
};

class Animation:QObject
{
    Q_OBJECT
public:
    Animation();
    void setRoomScene(RoomScene *view);
    void cardFlash(int cardID,int x,int y);
    QList<QGraphicsObject*>* getTempItems();
    QParallelAnimationGroup* itemFlash(QGraphicsObject* item,int x,int y);
    QList<QGraphicsObject*> tempItems;
    QThread *animationT;
public slots:
    void unlock();
private:
    RoomScene *view;
    QPropertyAnimation *posAni,*scaleAni,*opacityAni;
    QMutex lock;
    bool busy;
};

extern Animation *animation;
#endif // ANIMATION_H
