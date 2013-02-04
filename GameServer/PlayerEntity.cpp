#include "PlayerEntity.h"
#include <QTextStream>
#include <QFile>
#include "BackgroundEngine.h"
#include "Server.h"

//移除基础效果卡
bool PlayerEntity::removeBasicEffect(CardEntity* effect,int toWho,int toWhere)
{
    if(!basicEffect.removeOne(effect))
        return false;
    QList<CardEntity*> cards;
    cards << effect;
    coder.moveCardNotice(1,cards,this->getID(),EFFECT,toWho,toWhere);
    effect->setOwner(toWho);
    effect->setPlace(toWhere);
    effect->setSrcUser(toWho);
    if(toWhere==DISCARDPILE)
        emit toDiscardPileSIG(cards,true);
    return true;
}

void PlayerEntity::makeConnection(BackgroundEngine *engine)
{

    connect(this,SIGNAL(checkEndSIG()),engine,SLOT(checkEnd()));
    connect(this,SIGNAL(sendMessageSIG(int,QString)),engine,SIGNAL(sendMessageSIG(int,QString)));
    connect(this,SIGNAL(toDiscardPileSIG(QList<CardEntity*>,bool)),engine,SLOT(toDiscardPileSLOT(QList<CardEntity*>,bool)));
    connect(this,SIGNAL(loseMoraleSIG(int,int*,PlayerEntity*)),engine,SIGNAL(loseMoraleSIG(int,int*,PlayerEntity*)));
    connect(this,SIGNAL(showHandCards(QList<CardEntity*>,PlayerEntity*)),engine,SIGNAL(showHandCards(QList<CardEntity*>,PlayerEntity*)));
}

PlayerEntity::PlayerEntity(BackgroundEngine *engine,int ID, int isRed)
{
    //setInfo(0);
    crossNum=0;
    crossMax=3;
    gem=0;
    crystal=0;
    handCardsMax=6;
    token[0]=tokenMax[0]=0;
    token[1]=tokenMax[1]=0;
    token[2]=tokenMax[2]=0;
    this->setTap(false);
    this->setHandCardsMaxFixed(false);

    this->id = ID;
    this->color = isRed;
    this->makeConnection(engine);
    this->engine = engine;

    //以下属性在子类中修改
    this->crossMax = 2;
    this->energyMax = 3;
    this->star = 3;
    this->characterID = NONCHARACTER;
}
/*
void PlayerEntity::setInfo(int roleID)
{
    QFile fp("roleDB.txt");
    QTextStream in(&fp);
    QTextStream out(stdout);
    if (!fp.open(QIODevice::ReadOnly)) {
        out << "Cannot open file for reading. ";
    }
    QString roleEntry;
    while (!in.atEnd())
    {
        roleEntry=in.readLine();
        if(roleEntry[0].digitValue()==roleID)
        {
            info=roleEntry;
            break;
        }
    }
    fp.close();
}
QString PlayerEntity::getInfo()
{
    return info;
}
*/


void PlayerEntity::setHandCardsMax(int howMany)
{
    if(handCardsMaxFixed)
        return;
    handCardsMax=howMany;
    if(this->handCards.size() > handCardsMax)
        this->cardsOverLoad(0);
}

void PlayerEntity::handCardRange(int howMany)
{
    handCardsRange+=howMany;
    if(handCardsMaxFixed)
        return;
    else
        handCardsMax = 6 + handCardsRange;
    if(handCardsMax<handCardsMin)
        handCardsMax = handCardsMin;
    if(this->handCards.size() > handCardsMax)
        this->cardsOverLoad(0);
}

void PlayerEntity::setCrossNum(int howMany, int atMost)
{
    if(atMost==-1)
        crossNum=howMany<=crossMax?howMany:crossMax;
    else
        crossNum=howMany<=atMost?howMany:atMost;
}

void PlayerEntity::setGem(int howMany)
{
    if(howMany+crystal<=energyMax)
        gem=howMany;
    else
        gem=energyMax-crystal;
}

void PlayerEntity::setCrystal(int howMany)
{
    if(howMany+gem<=energyMax)
        crystal=howMany;
    else
        crystal=energyMax-gem;
}

void PlayerEntity::setYourTurn(bool yes)
{
    yourTurn=yes;
}

bool PlayerEntity::getYourturn()
{
    return yourTurn;
}

int PlayerEntity::getID()
{
    return id;
}

QString PlayerEntity::getName()
{
    return name;
}

int PlayerEntity::getHandCardMax()
{
    return handCardsMax;
}

int PlayerEntity::getHandCardNum()
{
    return this->handCards.size();
}

int PlayerEntity::getCrossNum()
{
    return crossNum;
}
int PlayerEntity::getCrossMax()
{
    return crossMax;
}

int PlayerEntity::getGem()
{
    return gem;
}

int PlayerEntity::getCrystal()
{
    return crystal;
}

int PlayerEntity::getEnergy()
{
    return gem+crystal;
}

int PlayerEntity::getColor()
{
    return color;
}
/*
static void PlayerEntity::loseMorale(int num,int isRed)
{
    if(isRed)
    {
        PlayerEntity::redTeam.morale -= num;
    }
    else
    {
        PlayerEntity::blueTeam.morale -= num;
    }
}*/

//移除手牌
//对于弃牌,此函数会修改卡牌的位置等参数并从玩家手中移除，否则只完成从玩家手中移除的动作，不设置卡牌位置
//此函数只会发送移牌指令，不会发送弃牌通告等其他通告，请特别注意
void PlayerEntity::removeHandCards(QList<CardEntity*> oldCard, bool show,bool toDiscardPile)
{
    if(toDiscardPile)
    {
        //弃牌
        coder.moveCardNotice(oldCard.size(),oldCard,this->getID(),HAND,-1,show?DISCARDPILE:DISCARDPILECOVERED);
        for(int i = 0;i < oldCard.size();i++)
        {
            oldCard.at(i)->setOwner(OWNERLESS);
            oldCard.at(i)->setSrcUser(-1);
            this->handCards.removeOne(oldCard.at(i));
            if(show)
                oldCard.at(i)->setPlace(DISCARDPILE);
            else
                oldCard.at(i)->setPlace(DISCARDPILECOVERED);
        }
        emit this->toDiscardPileSIG(oldCard,show);
        if(show)
            emit discardWithFaceUpSIG();
    }
    else
    {
        //非弃牌(基础效果牌)
        for(int i = 0;i < oldCard.size();i++)
        {
            this->handCards.removeOne(oldCard.at(i));
        }
    }
    QList<int> args;
    args<<this->getID();
    emit handCardsChange(args);
    if(show)
        emit showHandCards(oldCard,this);
}

//暴牌
void PlayerEntity::cardsOverLoad(int harmed)
{
    int overNum = this->handCards.size() - this->handCardsMax;
    coder.askForDiscard(this->getID(),overNum,"n");

    QList<CardEntity*> cardChosen;
    cardChosen = messageBuffer::readCardID(overNum);
    this->removeHandCards(cardChosen,false);
    coder.discardNotice(id,overNum,"n",cardChosen);
    emit loseMoraleSIG(harmed,&overNum,this);//应该要根据harmed参数分辨是否是伤害/哪种伤害造成的士气下降
    teamArea.setMorale(this->color,teamArea.getMorale(this->color) - overNum);

    coder.moraleNotice(this->color,teamArea.getMorale(this->color));

    emit checkEndSIG();

}
//增加手牌，可能发生暴牌
void PlayerEntity::addHandCards(QList<CardEntity*> newCard,int harmed, bool fromPile)
{
    if(fromPile)
        coder.moveCardNotice(newCard.size(),newCard,-1,PILE,this->getID(),HAND);

    for(int i = 0;i < newCard.size();i++)
    {
        handCards<<newCard.at(i);
        newCard.at(i)->setPlace(HAND);
        newCard.at(i)->setOwner(this->id);    
    }
    //qDebug("%d,%d",handCards.size(),this->getHandCardMax());
    if(handCards.size() > this->getHandCardMax())
        cardsOverLoad(harmed);
}

void PlayerEntity::giveHandCards(QList<CardEntity*> cards,PlayerEntity* to)
{
    int toID=to->getID();
    int howMany=cards.size();
    PlayerEntity* toPlayer;
    for(int i = 0;i < howMany;i++)
    {
        this->handCards.removeOne(cards.at(i));
        toPlayer=engine->getPlayerByID(toID);
        toPlayer->handCards.append(cards.at(i));
        cards.at(i)->setSrcUser(id);
        cards.at(i)->setOwner(toID);
        //qDebug("%d",cards.at(i)->getID());
    }
    coder.moveCardNotice(howMany,cards,id,HAND,toID,HAND);
    QList<int> args;
    args<<this->getID();
    emit handCardsChange(args);
    coder.getCardNotice(howMany,cards,toID,false);
    if(to->getHandCardNum()>to->getHandCardMax())
        to->cardsOverLoad(0);
}



