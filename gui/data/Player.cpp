#include "data/Player.h"
#include <QTextStream>
#include <QFile>
Player::Player(int ID, int isRed, QString nickname):id(ID),handCardsMax(6),crossNum(0),crossMax(2),energyMax(3),color(isRed),tap(0)
{
    roleID=0;
    gem=0;
    crystal=0;
    handCardsNum=0;
    coverCardsMax = 0;
    coverCardsNum = 0;
    this->nickname=nickname;
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
        roleName=tr("[½£Ê¥]");
        break;
    case 2:
        roleName=tr("[¿ñÕ½Ê¿]");
        break;
    case 3:
        roleName=tr("[¹­Ö®Å®Éñ]");
        break;
    case 4:
        roleName=tr("[·âÓ¡Ê¦]");
        break;
    case 5:
        roleName=tr("[°µÉ±Õß]");
        tapSource="resource/tap/QianXing.png";
        break;
    case 6:
        roleName=tr("[Ê¥Å®]");
        tapSource="resource/tap/LianMin.png";
        break;
    case 7:
        roleName=tr("[ÌìÊ¹]");
        break;
    case 8:
        roleName=tr("[Ä§µ¼Ê¦]");
        break;
    case 9:
        roleName=tr("[Ä§½£]");
        tapSource="resource/tap/AnYing.png";
        break;    
    case 10:
        roleName=tr("[Ê¥Ç¹]");
        crossMax=3;
        break;
    case 11:
        roleName=tr("[ÔªËØÊ¦]");
        addToken(0,new Token(tr("ÔªËØ"),3,0));
        break;
    case 12:
        roleName=tr("[Ã°ÏÕ¼Ò]");
        break;
    case 13:
        roleName=tr("[ËÀÁé·¨Ê¦]");
        crossMax = 5;
        break;
    case 14:
        roleName=tr("[ÖÙ²ÃÕß]");
        tapSource="resource/tap/ShenPan.png";
        crystal=2;
        addToken(0,new Token(tr("ÉóÅÐ"),4,0));
        break;
    case 15:
        roleName=tr("[Éñ¹Ù]");
        crossMax=6;
        break;
    case 16:
        roleName=tr("[Æíµ»Ê¦]");
        tapSource="resource/tap/QiDao.png";
        addToken(0,new Token(tr("·ûÎÄ"),3,0));
        break;
    case 17:
        roleName=tr("[ÏÍÕß]");
        energyMax=4;
        break;
    case 18:
        roleName = tr("[Áé·ûÊ¦]");
        addToken(2,new Token(tr("ÑýÁ¦"),2,2));
        break;
    case 19:
        roleName=tr("[½£µÛ]");
        addToken(0,new Token(tr("½£Æø"),5,0));
        addToken(2,new Token(tr("½£»ê"),3,2));
        break;
    case 20:
        roleName=tr("[¸ñ¶·¼Ò]");
        tapSource="resource/tap/BaiShi.png";
        addToken(0,new Token(tr("¶·Æø"),6,0));
        break;
    case 21:
        roleName=tr("[ÓÂÕß]");
        tapSource="resource/tap/JingPiLiJie.png";
        crystal=2;
        addToken(0,new Token(tr("Å­Æø"),4,0));
        addToken(1,new Token(tr("ÖªÐÔ"),4,1));
        break;
    case 22:
        roleName=tr("[Áé»êÊõÊ¿]");
        addToken(0,new Token(tr("»Æ»ê"),6,0));
        addToken(1,new Token(tr("À¶»ê"),6,1));
        break;
    case 23:
        roleName=tr("[Î×Å®]");
        tapSource="resource/tap/LiuXue.png";
        break;
    case 24:
        roleName=tr("[µûÎèÕß]");
        tapSource="resource/tap/DiaoLing.png";
        addToken(0,new Token(tr("Ó¼"),99,0));
        addToken(2,new Token(tr("¼ë"),8,2));
        break;
    case 26:
        roleName=tr("[Ä§¹­]");
        addToken(2,new Token(tr("³äÄÜ"),8,2));
        break;
    case 28:
        roleName=tr("[ºìÁ«ÆïÊ¿]");
        tapSource="resource/tap/ReXueFeiTeng.png";
        addToken(0,new Token(tr("ÑªÓ¡"),2,0));
        crossMax=4;
        break;
    case 29:
        roleName=tr("[Ä§Ç¹]");
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

