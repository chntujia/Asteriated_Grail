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
        roleName=tr("[剑圣]");
        break;
    case 2:
        roleName=tr("[狂战士]");
        break;
    case 3:
        roleName=tr("[弓之女神]");
        break;
    case 4:
        roleName=tr("[封印师]");
        break;
    case 5:
        roleName=tr("[暗杀者]");
        tapSource="resource/tap/QianXing.png";
        break;
    case 6:
        roleName=tr("[圣女]");
        tapSource="resource/tap/LianMin.png";
        break;
    case 7:
        roleName=tr("[天使]");
        break;
    case 8:
        roleName=tr("[魔导师]");
        break;
    case 9:
        roleName=tr("[魔剑]");
        tapSource="resource/tap/AnYing.png";
        break;    
    case 10:
        roleName=tr("[圣枪]");
        crossMax=3;
        break;
    case 11:
        roleName=tr("[元素师]");
        addToken(0,new Token(tr("元素"),3,0));
        break;
    case 12:
        roleName=tr("[冒险家]");
        break;
    case 13:
        roleName=tr("[死灵法师]");
        crossMax = 5;
        break;
    case 14:
        roleName=tr("[仲裁者]");
        tapSource="resource/tap/ShenPan.png";
        addToken(0,new Token(tr("审判"),4,0));
        crystal=2;
        break;
    case 15:
        roleName=tr("[神官]");
        crossMax=6;
        break;
    case 16:
        roleName=tr("[祈祷师]");
        tapSource="resource/tap/QiDao.png";
        addToken(0,new Token(tr("符文"),3,0));
        break;
    case 17:
        roleName=tr("[贤者]");
        energyMax=4;
        break;
    case 18:
        roleName=tr("[灵符师]");
        addToken(2,new Token(tr("妖力"),2,2));
        break;
    case 19:
        roleName=tr("[剑帝]");
        addToken(0,new Token(tr("剑气"),5,0));
        addToken(2,new Token(tr("剑魂"),3,2));
        break;
    case 20:
        roleName=tr("[格斗家]");
        tapSource="resource/tap/BaiShi.png";
        addToken(0,new Token(tr("斗气"),6,0));
        break;
    case 21:
        roleName=tr("[勇者]");
        tapSource="resource/tap/JingPiLiJie.png";
        crystal=2;
        addToken(0,new Token(tr("怒气"),4,0));
        addToken(1,new Token(tr("知性"),4,1));
        break;
    case 22:
        roleName=tr("[灵魂术士]");
        addToken(0,new Token(tr("黄魂"),6,0));
        addToken(1,new Token(tr("蓝魂"),6,1));
        break;
    case 23:
        roleName=tr("[巫女]");
        tapSource="resource/tap/LiuXue.png";
        break;
    case 24:
        roleName=("[蝶舞者]");
        tapSource="resource/tap/DiaoLing.png";
        addToken(0,new Token(tr("蛹"),99,0));
        addToken(2,new Token(tr("茧"),8,2));
        break;
    case 26:
        roleName=("[魔弓]");
        addToken(2,new Token(tr("充能"),8,2));
        break;
    case 28:
        roleName=tr("[红莲骑士]");
        tapSource="resource/tap/ReXueFeiTeng.png";
        addToken(0,new Token(tr("血印"),2,0));
        crossMax=4;
        break;
    case 29:
        roleName=tr("[魔枪]");
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

