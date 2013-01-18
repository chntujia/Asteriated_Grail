#ifndef CARDENTITY_H
#define CARDENTITY_H

#include <QList>
#include <QStringList>
#include <QObject>

#define SHIELDCARD 0
#define POISONCARD 1
#define WEAKCARD 2
#define MISSILECARD 3
#define LIGHTCARD 4

#define NOTEXIST           0
#define PILE               1
#define DISCARDPILE        2
#define DISCARDPILECOVERED 3
#define HAND               4
#define EFFECT             5
#define COVERED            6

#define OWNERLESS            -1

extern QList<int> shieldCardID;
extern QList<int> weakCardID;
extern QList<int> poisonCardID;
extern QList<int> missileCardID;

//卡牌类
class CardEntity:public QObject
{
    Q_OBJECT
public:
    //构造,输入参数是包含卡牌信息的一列String
    CardEntity(QStringList *cardEntry);

    int getID();
    //获取卡牌独有技信息
    int getHasSpeciality();
    //获取攻击/法术类别
    QString getType();
    //获取卡牌属性
    QString getElement();
    //获取卡牌技、血、咏、圣、幻类别
    QString getProperty();
    //获取卡牌名称，注意是中文
    QString getName();
    //获取卡牌图片地址
    QString getSource();
    //获取卡牌独有技列表
    QStringList getSpecialityList();
    //将法术牌名称转为int
    int getMagicName(){return this->magicName;}
    void setMagicName(int name){this->magicName = name;}
    //获取使用者
    int getSrcUser(){return this->srcUser;}
    //获取卡牌当前所处位置（手牌、盖牌、效果区、牌堆、明弃牌堆、暗弃牌堆）
    int getPlace(){return this->place;}
    //获取卡牌当前处于谁的区域（包括手牌、盖牌、效果区等）（A对B使用中毒，中毒的srcUser是A，owner是B）
    int getOwner(){return this->owner;}

    void setPlace(int newPlace){this->place = newPlace;}
    void setOwner(int newOwner){this->owner = newOwner;}
    void setSrcUser(int srcID){this->srcUser = srcID;}
    //核查卡牌是否是基本法术牌
    int checkBasicMagic(QString cardName);
protected:
    int id;
    int hasSpeceiality;
    QString type;
    QString element;
    QString property;
    QString name;
    QStringList specialityList;
    QString source;
    int srcUser;
    int owner;
    int place;
    int magicName;
};

#endif // CARDENTITY_H
