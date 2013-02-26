#include "PlayerArea.h"
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "data/Player.h"
#include <QPainter>
static QRectF PlayerAreaRect(0, 0, 1000, 700);
static const QPointF PlayerPos[]={QPointF(10,500),QPointF(740,260),QPointF(545,60),QPointF(380,60),QPointF(210,60),QPointF(10,260)};
PlayerArea::PlayerArea():least(1),most(1)
{
    int i;
    //根据座次建立
    QList<Player*> playerList=dataInterface->getPlayerList();
    for(i=0;i<playerList.count();i++)
    {
        playerItems<<new PlayerItem(playerList[i]);
        connect(playerItems[i],SIGNAL(playerSelected(int)),this,SLOT(onPlayerSelected(int)));
        connect(playerItems[i],SIGNAL(playerUnselected(int)),this,SLOT(onPlayerUnselected(int)));
        playerItems[i]->setParentItem(this);
        playerItems[i]->setPos(PlayerPos[i]);
        playerList[i]->setPos(i);
    }
    //根据ID排序
    sortPlayerItems();
    dataInterface->sortPlayers();
    //设置队友
    int myID=dataInterface->getID();
    playerList=dataInterface->getPlayerList();
    Player* player=playerList[myID];
    int myColor=player->getColor();
    for(i=0;i<playerList.count();i++)
        if(playerList[i]->getColor()!=myColor)
            enemy<<i;
    currentPlayerID=dataInterface->getFirstPlayerID();

}
void PlayerArea::reset()
{
    disableAll();
    foreach(PlayerItem* ptr,playerItems)
        ptr->setSelected(0);
    selectedPlayers.clear();
}

QRectF PlayerArea::boundingRect() const
{
    return PlayerAreaRect;
}

void PlayerArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    PlayerItem* currentPlayerItem;
    currentPlayerItem=playerItems[currentPlayerID];
    painter->drawPixmap(currentPlayerItem->x()-5,currentPlayerItem->y()-5,QPixmap("resource/yourTurn.png"));
}

void PlayerArea::sortPlayerItems()
{
    int i,j;
    int max=dataInterface->getPlayerMax();
    for(i=0;i<max;i++)
        for(j=0;j<max;j++)
        {
            if(playerItems[j]->getPlayer()->getID()==i)
            {
                playerItems.swap(i,j);
                break;
            }
        }
}
void PlayerArea::enablePlayerItem(int id)
{
    playerItems[id]->setEnabled(1);
    playerItems[id]->setOpacity(1);
}

void PlayerArea::disablePlayerItem(int id)
{
    playerItems[id]->setEnabled(0);
    playerItems[id]->setOpacity(0.6);
}

void PlayerArea::enableAll()
{
    foreach(PlayerItem*ptr,playerItems)
    {
        ptr->setEnabled(1);
        ptr->setOpacity(1);
    }
}

void PlayerArea::disableAll()
{
    foreach(PlayerItem*ptr,playerItems)
    {
        ptr->setEnabled(0);
        ptr->setOpacity(0.6);
    }
}

void PlayerArea::enableMate(bool includeYou)
{
    int i;
    for(i=0;i<playerItems.count();i++)
        if(!enemy.contains(i))
            enablePlayerItem(i);
    if(!includeYou)
        disablePlayerItem(dataInterface->getID());
}
void PlayerArea::enableEnemy()
{
    int i;
    for(i=0;i<playerItems.count();i++)
        if(enemy.contains(i))
            enablePlayerItem(i);
}

void PlayerArea::onPlayerSelected(int id)
{
    if(selectedPlayers.count()>=most)
    {
        int firstPlayerID=selectedPlayers.takeFirst()->getID();
        playerItems[firstPlayerID]->setSelected(0);
    }
    selectedPlayers<<playerItems[id]->getPlayer();
    if(selectedPlayers.count()<least)
        return;
    emit playerReady();
}

void PlayerArea::onPlayerUnselected(int id)
{
    selectedPlayers.removeOne(playerItems[id]->getPlayer());
    if(selectedPlayers.count()<least)
    {
        gui->getDecisionArea()->disable(0);
        emit playerUnready();
    }
}
QList<Player*> PlayerArea::getSelectedPlayers()
{
    return selectedPlayers;
}
