#include "BPArea.h"
#include <QPainter>
#include <QFont>
#include "data/DataInterface.h"
#include "widget/GUI.h"

static QRectF BPAreaRect(-100, 0, 1000, 1000);
static const QPointF RolePos[]=
{QPointF(200,200),QPointF(240,200),QPointF(280,200),QPointF(320,200),
 QPointF(360,200),QPointF(400,200),QPointF(440,200),QPointF(480,200),
 QPointF(520,200),QPointF(560,200),QPointF(600,200),QPointF(640,200),
 QPointF(680,200),QPointF(720,200),QPointF(760,200),QPointF(800,200)
};
BPArea::BPArea():least(1),most(1)
{
    height=1000;
    width=1000;
    setVisible(false);
    currentRed = 0;
    currentBlue = 0;
    currentColor = 1;
}

void BPArea::BPStart(int num, QList<int> roles)
{
    gui->reset();
    left = roles;
    for(int i=0;i<num;i++)
    {
        roleItems << new RoleItem(roles[i]);
        connect(roleItems[i],SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
        connect(roleItems[i],SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        roleItems[i]->setParentItem(this);
        roleItems[i]->setPos(RolePos[i]);
        roleItems[i]->setZValue(0.1*i);
    }
    setVisible(true);
    reset();
}

void BPArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font;
    font.setPixelSize(20);
    painter->setFont(font);
    painter->setPen(QColor(Qt::white));
    painter->drawText(QRectF(0, 0, width, height*0.8),Qt::AlignCenter,msg);
}

QRectF BPArea::boundingRect() const
{
    return BPAreaRect;
}

void BPArea::enableRoleItem(int roleID)
{
    RoleItem* role = getRoleByID(roleID);
    role->setEnabled(1);
    role->setOpacity(1);
}

void BPArea::disableRoleItem(int roleID)
{
    RoleItem* role = getRoleByID(roleID);
    role->setEnabled(0);
    role->setOpacity(0.6);
    role->setY(220);
}

void BPArea::onRoleSelected(int id)
{
    if(selectedRoles.count()>=most)
    {
        int firstRole=selectedRoles.takeFirst();
        RoleItem* tmp = getRoleByID(firstRole);
        tmp->setSelected(0);
        tmp->setY(tmp->y()+20);
    }
    selectedRoles<<id;
    if(selectedRoles.count()<least)
        return;
    emit roleReady();
}

void BPArea::onRoleUnselected(int id)
{
    selectedRoles.removeOne(id);
    if(selectedRoles.count()<least)
    {
        gui->getDecisionArea()->disable(0);
        emit roleUnready();
    }
}

void BPArea::setMsg(QString msg)
{
    this->msg=msg;
    setVisible(1);
    update();
}

QList<int> BPArea::getSelectedRoles()
{
    return selectedRoles;
}

RoleItem *BPArea::getRoleByID(int ID)
{
    foreach(RoleItem* ptr, roleItems)
    {
        if(ptr->getRoleID() ==ID)
            return ptr;
    }
}

void BPArea::choose(int roleID)
{
    RoleItem* choice = getRoleByID(roleID);
    QString queue = dataInterface->getQueue();
    int max = dataInterface->getPlayerMax();
    if(currentColor==1)
    {
        while(queue[currentRed+max].digitValue()!=1)
            currentRed++;
        choice->setX((currentRed+5)*60);
        choice->setY(450);
        choice->setEnabled(0);
        choice->setOpacity(1);
        currentRed ++;
        currentColor = 0;
    }
    else
    {
        while(queue[currentBlue+max].digitValue()!=0)
            currentBlue++;
        choice->setX((currentBlue+5)*60);
        choice->setY(500);
        choice->setEnabled(0);
        choice->setOpacity(1);
        currentBlue ++;
        currentColor = 1;
    }
}

void BPArea::reset()
{
    selectedRoles.clear();
    setMsg("");
    disablaAll();
}


bool BPArea::checkOver()
{
    int max = dataInterface->getPlayerMax();
    if(currentRed==max&&currentBlue==max-1)
        return true;
    if(currentRed==max-1||currentBlue==max)
        return true;
    return false;
}

void BPArea::remove(int roleID)
{
    left.removeOne(roleID);
}

void BPArea::disablaAll()
{
    foreach(int ptr, left)
    {
        getRoleByID(ptr)->setEnabled(0);
    }
}

void BPArea::enableAll()
{
    foreach(int ptr, left)
    {
        getRoleByID(ptr)->setEnabled(1);
    }
}
