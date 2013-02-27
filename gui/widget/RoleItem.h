#ifndef ROLEITEM_H
#define ROLEITEM_H

#include <QGraphicsObject>
#include "data/Player.h"
#include <QPushButton>
#include <QPixmap>

class RoleItem : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    RoleItem(int roleID);
    ~RoleItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    int getRoleID();
    bool isSelected();
    void setSelected(bool yes);
signals:
    void roleSelected(int);
    void roleUnselected(int);
private:
    int roleID;
    int width;
    int height;
    bool selected;
    QPixmap pixmap;

};

#endif // ROLEITEM_H
