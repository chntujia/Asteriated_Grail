#include "data/DataInterface.h"
#include <QTextStream>
#include <QFile>
#include "widget/GUI.h"

DataInterface *dataInterface=NULL;
DataInterface::DataInterface(QObject *parent) :
    QObject(parent)
{
    initCardDB();
    /*
    playerMax=6;
    id=4;
    initPlayerList(QString("143025101001"));
    initTeam(15);
*/
}
void DataInterface::initCardDB()
{
    QFile fp("data/cardDB.txt");
    QTextStream in(&fp);
    QTextStream out(stdout);
    if (!fp.open(QIODevice::ReadOnly)) {
            out << "Cannot open file for reading. ";
    }
    cardDB.clear();
    QStringList cardEntry;
    while (!in.atEnd())
    {
        cardEntry=in.readLine().split("\t");
        cardDB<<new Card(&cardEntry);
    }
    fp.close();
}
void DataInterface::initPlayerList(QString msg)
{
    int i,isRed,pID,myPos;
    queue=msg;
    //find myPos
    for(i=0;i<playerMax;i++)
        if(msg[i].digitValue()==id)
            break;
    myPos=i;
    //设置座次，分队    
    for(;i<playerMax;i++)
    {
        pID=msg[i].digitValue();
        isRed=msg[i+playerMax].digitValue();
        playerList<<new Player(pID,isRed);

    }

    pID=msg[0].digitValue();
    firstPlayerID=pID;

    for(i=0;i<myPos;i++)
    {
        pID=msg[i].digitValue();
        isRed=msg[i+playerMax].digitValue();
        playerList<<new Player(pID,isRed);

    }
    myself=playerList[0];

}

void DataInterface::initTeam(int moraleMax)
{
    red=new Team(1,moraleMax);
    blue=new Team(0,moraleMax);
    if (myself->getColor()){
        myTeam=red;
        otherTeam=blue;
    }
    else{
        myTeam=blue;
        otherTeam=red;
    }
}

void DataInterface::sortPlayers()
{
    int i,j;
    for(i=0;i<playerMax;i++)
        for(j=0;j<playerMax;j++)
        {
            if(playerList[j]->getID()==i)
            {
                playerList.swap(i,j);
                break;
            }
        }
}
void DataInterface::setID(int id)
{
    this->id=id;
}
void DataInterface::setPlayerMax(int playerMax)
{
    this->playerMax=playerMax;
}

void DataInterface::setMyself(Player* player)
{
    myself=player;
}

void DataInterface::addHandCard(Card* card)
{
    handcards<<card;
    gui->getHandArea()->addCardItem(card);

}
void DataInterface::removeHandCard(Card *card)
{
    handcards.removeOne(card);
    gui->getHandArea()->removeCardItem(card);
}

Card* DataInterface::getCard(int id)
{
    return cardDB.at(id);
}
int DataInterface::getID()
{
    return id;
}
int DataInterface::getPlayerMax()
{
    return playerMax;
}

QList<Player*> DataInterface::getPlayerList()
{
    return playerList;
}

QList<Card*> DataInterface::getHandCards()
{
    return handcards;
}

Player* DataInterface::getMyself()
{
    return myself;
}

Team* DataInterface::getRedTeam()
{
    return red;
}
Team* DataInterface::getBlueTeam()
{
    return blue;
}
