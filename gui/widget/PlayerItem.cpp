#include "PlayerItem.h"
#include "data/DataInterface.h"
#include <QPainter>
StatusItem::StatusItem(Status* status)
{
    this->status=status;
    Card*card=status->card;
    QString tip=card->getName()+'-'+card->getProperty();
    for(int i=0;i<card->getHasSpeciality();i++)
        tip+='\n'+card->getSpecialityList().at(i);
    setToolTip(tip);
}

QRectF StatusItem::boundingRect() const
{
    return QRectF(0, 0, 15, 15);
}

//中毒0虚弱1圣盾2
void StatusItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(status->type)
    {
    case 0:
        painter->drawPixmap(0,0,QPixmap("resource/du.png"));
        break;
    case 1:
        painter->drawPixmap(0,0,QPixmap("resource/xu.png"));
        break;
    case 2:
        painter->drawPixmap(0,0,QPixmap("resource/dun.png"));
        break;
    case 3:
        painter->drawPixmap(0,0,QPixmap("resource/feng.png"));
        break;
    }

}

PlayerItem::PlayerItem(Player* player):selected(0)
{
    this->player=player;
    connect(player,SIGNAL(addStatusSIG(Status*)),this,SLOT(addStatusItem(Status*)));
    connect(player,SIGNAL(removeStatusSIG(Status*)),this,SLOT(removeStatusItem(Status*)));
    if(player->getColor())
        frame=QPixmap("resource/playerFrameRed.png");
    else
        frame=QPixmap("resource/playerFrameBlue.png");
    this->width=frame.width();
    this->height=frame.height();
    gem=QPixmap("resource/Egem.png");
    crystal=QPixmap("resource/Ecrystal.png");
}
QRectF PlayerItem::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
void PlayerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, frame);
    painter->drawPixmap(16,20,player->getFaceSource());

    QFont font;
    font.setBold(1);
    painter->setFont(font);

    painter->drawText(0.15*width,0.1*height,QString::number(player->getID()));

    QString entry=tr("手牌：");
    entry+=QString::number(player->getHandCardNum())+'/'+QString::number(player->getHandCardMax());
    painter->drawText(width*0.12,height*0.73,entry);

    entry=tr("治疗：");
    entry+=QString::number(player->getCrossNum())+'/'+QString::number(player->getCrossMax());
    painter->drawText(width*0.12,height*0.82,entry);
    int i,offset;
    painter->drawText(width*0.12,height*0.92,tr("能量："));
    for(i=0;i<player->getGem();i++)
        painter->drawPixmap(width*0.35+i*9,height*0.86,gem.scaled(9,13));
    offset=width*0.35+i*9;
    for(i=0;i<player->getCrystal();i++)
        painter->drawPixmap(i*9+offset,height*0.86,crystal.scaled(9,13));
    if(player->getTap())
        painter->drawPixmap(0,0,QPixmap(player->getTapSource()));
    if(player->getSpecial(0))
        painter->drawPixmap(115,20,QPixmap("resource/shufu.png"));
    if(selected)
        painter->drawPixmap(-5,-5,QPixmap("resource/playerSelected.png"));

}
void PlayerItem::addStatusItem(Status *status)
{
    StatusItem *statusItem=new StatusItem(status);
    statusItem->setParentItem(this);
    statusItem->setY(85);
    statusItems<<statusItem;
    adjustStatusItems();
}

void PlayerItem::removeStatusItem(Status *status)
{
    int i;
    for(i=0;i<statusItems.count();i++)
        if(statusItems[i]->status==status)
        {
            delete statusItems[i];
            statusItems.removeAt(i);
        }
    adjustStatusItems();
}

void PlayerItem::adjustStatusItems()
{
    int i;
    for(i=0;i<statusItems.count();i++)
    {
        statusItems[i]->setX(i*15);
        statusItems[i]->setY(height-8);
    }
}

Player* PlayerItem::getPlayer()
{
    return player;
}
void PlayerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->selected)
    {
        selected=1;
        update();
        emit playerSelected(player->getID());
    }
    else
    {
        selected=0;
        update();
        emit playerUnselected(player->getID());
    }
}
void PlayerItem::setSelected(bool yes)
{
    selected=yes;
    update();
}
bool PlayerItem::isSelected()
{
    return selected;
}
