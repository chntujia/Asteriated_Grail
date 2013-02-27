#include "data/Player.h"
#include <QTextStream>
#include <QFile>
Player::Player(int ID, int isRed):id(ID),handCardsMax(6),crossNum(0),crossMax(2),energyMax(3),color(isRed),tap(0)
{
    roleID=0;
    gem=0;
    crystal=0;
    handCardsNum=0;
    coverCardsMax = 0;
    coverCardsNum = 0;
    for(int i=0;i<5;i++ )
        specials[i]=false;
}
void Player::setRole(int roleID)
{
    if(this->roleID!=0)
        return;
    this->roleID=roleID;
    faceSource="role/"+QString::number(roleID)+".png";
    switch(roleID)
    {
    case 1:
        name=tr("[½£Ê¥]");
        break;
    case 2:
        name=tr("[¿ñÕ½Ê¿]");
        break;
    case 3:
        name=tr("[¹­Ö®Å®Éñ]");
        break;
    case 4:
        name=tr("[·âÓ¡Ê¦]");
        break;
    case 5:
        name=tr("[°µÉ±Õß]");
        tapSource="resource/tap/QianXing.png";
        break;
    case 6:
        name=tr("[Ê¥Å®]");
        tapSource="resource/tap/LianMin.png";
        break;
    case 7:
        name=tr("[ÌìÊ¹]");
        break;
    case 8:
        name=tr("[Ä§µ¼Ê¦]");
        break;
    case 9:
        name=tr("[Ä§½£]");
        tapSource="resource/tap/AnYing.png";
        break;    
    case 10:
        name=tr("[Ê¥Ç¹]");
        crossMax=3;
        break;
    case 11:
        name=tr("[ÔªËØÊ¦]");
        addToken(0,new Token(tr("ÔªËØ"),3,0));
        break;
    case 12:
        name=tr("[Ã°ÏÕ¼Ò]");
        break;
    case 13:
        name=tr("[ËÀÁé·¨Ê¦]");
        crossMax = 5;
        break;
    case 14:
        name=tr("[ÖÙ²ÃÕß]");
        tapSource="resource/tap/ShenPan.png";
        crystal=2;
        addToken(0,new Token(tr("ÉóÅÐ"),4,0));
        break;
    case 15:
        name=tr("[Éñ¹Ù]");
        crossMax=6;
        break;
    case 16:
        name=tr("[Æíµ»Ê¦]");
        tapSource="resource/tap/QiDao.png";
        addToken(0,new Token(tr("·ûÎÄ"),3,0));
        break;
    case 17:
        name=tr("[ÏÍÕß]");
        energyMax=4;
        break;
    case 18:
        name = tr("[Áé·ûÊ¦]");
        addToken(2,new Token(tr("ÑýÁ¦"),2,2));
        break;
    case 19:
        name=tr("[½£µÛ]");
        addToken(0,new Token(tr("½£Æø"),5,0));
        addToken(2,new Token(tr("½£»ê"),3,2));
        break;
    case 20:
        name=tr("[¸ñ¶·¼Ò]");
        tapSource="resource/tap/BaiShi.png";
        addToken(0,new Token(tr("¶·Æø"),6,0));
        break;
    case 21:
        name=tr("[ÓÂÕß]");
        tapSource="resource/tap/JingPiLiJie.png";
        crystal=2;
        addToken(0,new Token(tr("Å­Æø"),4,0));
        addToken(1,new Token(tr("ÖªÐÔ"),4,1));
        break;
    case 22:
        name=tr("[Áé»êÊõÊ¿]");
        addToken(0,new Token(tr("»Æ»ê"),6,0));
        addToken(1,new Token(tr("À¶»ê"),6,1));
        break;
    case 23:
        name=tr("[Î×Å®]");
        tapSource="resource/tap/LiuXue.png";
        break;
    case 24:
        name=("[µûÎèÕß]");
        tapSource="resource/tap/DiaoLing.png";
        addToken(0,new Token(tr("Ó¼"),99,0));
        addToken(2,new Token(tr("¼ë"),8,2));
        break;
    case 26:
        name=("[Ä§¹­]");
        addToken(2,new Token(tr("³äÄÜ"),8,2));
        break;
    case 28:
        name=tr("[ºìÁ«ÆïÊ¿]");
        tapSource="resource/tap/ReXueFeiTeng.png";
        addToken(0,new Token(tr("ÑªÓ¡"),2,0));
        crossMax=4;
        break;
    case 29:
        name=tr("[Ä§Ç¹]");
        tapSource="resource/tap/HuanYing.png";
        break;
    }
}

void Player::addBasicStatus(int type,Card* card)
{
    if(type==3)
    {
        if(card->getElement()=="wind")
            type=31;
        else if(card->getElement()=="water")
            type=32;
        else if(card->getElement()=="fire")
            type=33;
        else if(card->getElement()=="earth")
            type=34;
        else if(card->getElement()=="thunder")
            type=35;
    }
    BasicStatus*status=new BasicStatus(type,card);
    BasicStatusList<<status;
    emit addBasicStatusSIG(status);
}

void Player::removeBasicStatus(Card* card)
{
    int i;
    for(i=0;i<BasicStatusList.count();i++)
        if(BasicStatusList.at(i)->card==card)
        {
            emit removeBasicStatusSIG(BasicStatusList.at(i));
            BasicStatusList.removeAt(i);
        }
}

bool Player::checkBasicStatus(int type)
{
    foreach(BasicStatus* ptr,BasicStatusList)
        if(ptr->type==type)
            return 1;
    return 0;
}

bool Player::checkBasicStatus(QString status)
{
    foreach(BasicStatus* ptr,BasicStatusList)
        if(ptr->getCard()->getSpecialityList().contains(status))
            return 1;
    return 0;
}

void Player::setHandCardsMax(int howMany)
{
    handCardsMax=howMany;
}

void Player::setCrossNum(int howMany)
{
    crossNum=howMany;
}

void Player::setGem(int howMany)
{
    gem=howMany;
}

void Player::setCrystal(int howMany)
{
    crystal=howMany;
}

void Player::setEnergyMax(int howMany)
{
    energyMax=howMany;
}

void Player::setSpecial(int type,bool flag)
{
    specials[type]=flag;
    if(flag)
        emit addSpecialStatusSIG(type);
    else
        emit removeSpecialStatusSIG(type);
}

int Player::getID()
{
    return id;
}

QString Player::getName()
{
    return name;
}

int Player::getHandCardMax()
{
    return handCardsMax;
}

int Player::getHandCardNum()
{
    return handCardsNum;
}

int Player::getCrossNum()
{
    return crossNum;
}
int Player::getCrossMax()
{
    return crossMax;
}

int Player::getGem()
{
    return gem;
}

int Player::getCrystal()
{
    return crystal;
}

int Player::getEnergy()
{
    return gem+crystal;
}

int Player::getColor()
{
    return color;
}

int Player::getEnergyMax()
{
    return energyMax;
}

