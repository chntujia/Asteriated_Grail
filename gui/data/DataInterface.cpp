#include "data/DataInterface.h"
#include <QTextStream>
#include <QFile>
#include "widget/GUI.h"

DataInterface *dataInterface=NULL;
DataInterface::DataInterface(QObject *parent) :
    QObject(parent)
{
    initCardDB();
    initRoleList();
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

void DataInterface::initRoleList()
{
    roleList<<NULL;
    roleList<<tr("[剑圣]");
    roleList<<tr("[狂战士]");
    roleList<<tr("[弓之女神]");
    roleList<<tr("[封印师]");
    roleList<<tr("[暗杀者]");
    roleList<<tr("[圣女]");
    roleList<<tr("[天使]");
    roleList<<tr("[魔导师]");
    roleList<<tr("[魔剑]");
    roleList<<tr("[圣枪]");
    roleList<<tr("[元素师]");
    roleList<<tr("[冒险家]");
    roleList<<tr("[死灵法师]");
    roleList<<tr("[仲裁者]");
    roleList<<tr("[神官]");
    roleList<<tr("[祈祷师]");
    roleList<<tr("[贤者]");
    roleList<<tr("[灵符师]");
    roleList<<tr("[剑帝]");
    roleList<<tr("[格斗家]");
    roleList<<tr("[勇者]");
    roleList<<tr("[灵魂术士]");
    roleList<<tr("[巫女]");
    roleList<<tr("[蝶舞者]");
    roleList<<tr("[]");
    roleList<<tr("[魔弓]");
    roleList<<tr("[]");
    roleList<<tr("[红莲骑士]");
    roleList<<tr("[魔枪]");
}

void DataInterface::initPlayerList(QString msg,QStringList nicknames)
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
        playerList<<new Player(pID,isRed,nicknames[i]);

    }

    pID=msg[0].digitValue();
    firstPlayerID=pID;

    for(i=0;i<myPos;i++)
    {
        pID=msg[i].digitValue();
        isRed=msg[i+playerMax].digitValue();
        playerList<<new Player(pID,isRed,nicknames[i]);

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

void DataInterface::addCoverCard(Card* card)
{
    coverCards << card;
    gui->getCoverArea()->addCardItem(card);
}

void DataInterface::removeHandCard(Card *card)
{
    handcards.removeOne(card);
    gui->getHandArea()->removeCardItem(card);
}

void DataInterface::removeCoverCard(Card *card)
{
    coverCards.removeOne(card);
    gui->getCoverArea()->removeCardItem(card);
}
Player* DataInterface::getPlayerById(int id)
{
    for(int i = 0;i < playerList.length();i++)
    {
        if(id == playerList[i]->getID())
            return playerList[i];
    }
    return NULL;
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

QList<Card*> DataInterface::getCoverCards()
{
    return coverCards;
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
