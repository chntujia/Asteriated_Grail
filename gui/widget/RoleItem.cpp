#include "RoleItem.h"
#include <QPainter>

RoleItem::RoleItem(int roleID)
{
    this->roleID=roleID;
    pixmap=QPixmap("role/"+QString::number(roleID)+".jpg");

    this->width=pixmap.width();
    this->height=pixmap.height();
    this->selected=0;
    setVisible(1);
}

RoleItem::~RoleItem()
{

}

QRectF RoleItem::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
void RoleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pixmap);
}
int RoleItem::getRoleID()
{
    return this->roleID;
}
bool RoleItem::isSelected()
{
    return this->selected;
}
void RoleItem::setSelected(bool status)
{
    this->selected=status;
    update();
}

void RoleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->selected)
    {
        selected=1;
        setY(y()-20);
        emit roleSelected(roleID);
    }
    else
    {
        selected=0;
        setY(y()+20);
        emit roleUnselected(roleID);
    }
}

