#include "PlayerItem.h"
#include "data/DataInterface.h"
#include <QPainter>
#include <QPushButton.h>
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

//ÖÐ¶¾0ÐéÈõ1Ê¥¶Ü2
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
    case 4:
        painter->drawPixmap(0,0,QPixmap("resource/wei.png"));
        break;
    case 5:
        painter->drawPixmap(0,0,QPixmap("resource/xun.png"));
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
    card=QPixmap("resource/card.png");
    overflow=QPixmap("resource/overflow.png");
    heal=QPixmap("resource/heal.png");
    remain=QPixmap("resource/remain.png");
}
QRectF PlayerItem::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
void PlayerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(selected)
        painter->drawPixmap(-5,-3,QPixmap("resource/playerSelected.png"));
    painter->drawPixmap(0, 0, frame);
    painter->drawPixmap(2,6,player->getFaceSource());

    QFont font;
    font.setBold(1);
    painter->setFont(font);

    painter->drawText(0.3*width,0.11*height,QString::number(player->getID()));

    int num=player->getHandCardNum();
    int max=player->getHandCardMax();
    QString entry=QString::number(num)+'/'+QString::number(max);
    painter->drawText(width*0.42,height*0.24,entry);
    int i,offset;
    for(i=0;num<=max && i<max;i++)
        if(i<num)
            painter->drawPixmap(width*0.62+card.width()*i,height*0.16, card);
        else
            painter->drawPixmap(width*0.62+card.width()*i,height*0.16, remain);
    for(i=0;num>max && i<max;i++)
        painter->drawPixmap(width*0.42+card.width()*i,height*0.24, overflow);

    num=player->getCrossNum();
    max=player->getCrossMax();
    entry=QString::number(num)+'/'+QString::number(max);
    painter->drawText(width*0.42,height*0.37,entry);
    for(i=0;i<max;i++)
        if(i<num)
            painter->drawPixmap(width*0.62+card.width()*i,height*0.26, heal);
        else
            painter->drawPixmap(width*0.62+card.width()*i,height*0.26, remain);
    for(i=0;i<num-max;i++)
        painter->drawPixmap(width*0.62+card.width()*i,height*0.3, heal);


    for(i=0;i<player->getGem();i++)
        painter->drawPixmap(width*0.74+i*gem.width(),height*0.37,gem);

    offset=width*0.74+i*gem.width();
    for(i=0;i<player->getCrystal();i++)
        painter->drawPixmap(i*crystal.width()+offset,height*0.37,crystal);

    if(player->getTokenMax(0)>0)
    {
        painter->drawPixmap(0,0,QPixmap("resource/token1.png"));
        entry=player->getTokenName(0)+"£º";
        entry+=QString::number(player->getToken(0))+'/'+QString::number(player->getTokenMax(0));
        painter->drawText(width*0.55,height*0.73,entry);
    }
    if(player->getTokenMax(1)>0)
    {
        painter->drawPixmap(0,0,QPixmap("resource/token2.png"));
        entry=player->getTokenName(1)+"£º";
        entry+=QString::number(player->getToken(1))+'/'+QString::number(player->getTokenMax(1));
        painter->drawText(width*0.55,height*0.82,entry);
    }
    if(player->getTokenMax(2)>0)
    {
        painter->drawPixmap(0,0,QPixmap("resource/token3.png"));
        entry=player->getTokenName(2)+"£º";
        entry+=QString::number(player->getToken(2))+'/'+QString::number(player->getTokenMax(2));
        painter->drawText(width*0.55,height*0.92,entry);
    }
    if(player->getTap())
        painter->drawPixmap(0,0,QPixmap(player->getTapSource()));
    if(player->getSpecial(0))
        painter->drawPixmap(115,20,QPixmap("resource/shufu.png"));
    if(player->getSpecial(1))
        painter->drawPixmap(115,35,QPixmap("resource/tiaoxin.png"));
    if(player->getSpecial(2))
        painter->drawPixmap(115,50,QPixmap("resource/lianjie.png"));
    if(player->getSpecial(3))
        painter->drawPixmap(115,65,QPixmap("resource/tongsheng.png"));


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


