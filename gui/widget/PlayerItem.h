#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QGraphicsObject>
#include "data/Player.h"
class StatusItem : public QGraphicsObject
{
    friend class PlayerItem;
public:
    StatusItem(Status*status);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    Status*status;
};

class PlayerItem : public QGraphicsObject
{
    Q_OBJECT
public:
    PlayerItem(Player* player);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    Player *getPlayer();
    void setSelected(bool yes);
    bool isSelected();
    void adjustStatusItems();
public slots:
    void addStatusItem(Status* status);
    void removeStatusItem(Status* status);
signals:
    void playerSelected(int);
    void playerUnselected(int);
private:
    Player* player;
    QPixmap frame;
    QPixmap face;
    QPixmap gem;
    QPixmap crystal;
    int width;
    int height;
    bool selected;
    QList<StatusItem*> statusItems;
};

#endif // PLAYERITEM_H
