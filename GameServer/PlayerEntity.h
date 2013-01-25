#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H
#include "CardEntity.h"
#include <QObject>
#include "main.h"

#define NONCHARACTER 0
#define BLADEMASTER  1
#define BERSERKER    2
#define BOWLADY      3

class BackgroundEngine;

//玩家类
class PlayerEntity:public QObject
{
    Q_OBJECT
public:
    PlayerEntity(BackgroundEngine* engine,int id,int color);
    //连接信号槽
    void makeConnection(BackgroundEngine* engine);
    //增加手牌操作
    void addHandCards(QList<CardEntity*> newCard,int harmed,bool fromPile=true);
    //暴牌处理
    void cardsOverLoad(int harmed);
    //移除手牌操作
    void removeHandCards(QList<CardEntity*> oldCard,bool show,bool toDiscardPile = true);
    //给牌操作
    void giveHandCards(QList<CardEntity*> oldCard,PlayerEntity* to);
    //设置手牌上限
    void setHandCardsMax(int howMany);
    //设置治疗(未完成)
    void setCrossNum(int howMany, int atMost=-1);
    void setGem(int howMany);
    void setCrystal(int howMany);
    //设置当前回合是否为该玩家回合
    void setYourTurn(bool yes);
    void setSeatNum(int num){this->seatNum = num;}
    int getSeatNum(){return this->seatNum;}
    //设置横置
    void setTap(bool tap){this->tap = tap;}
    void setToken(int id,int howMany){token[id]=howMany<=tokenMax[id]?howMany:tokenMax[id];}
    //设置下一个玩家
    void setNext(PlayerEntity* nextPlayer){this->nextPlayer = nextPlayer;}
    //在该玩家前增加基础效果
    void addBasicEffect(CardEntity* effectCard){this->basicEffect << effectCard;}
    //设定手牌上限是否锁定
    void setHandCardsMaxFixed(bool fixed){this->handCardsMaxFixed = fixed;}
    //移除基础效果
    bool removeBasicEffect(CardEntity* effect,int toWho=-1,int toWhere=DISCARDPILE);
    int getID();
    QString getName();
    int getHandCardMax();
    int getHandCardNum();
    int getCrossNum();
    int getCrossMax();
    int getGem();
    int getCrystal();
    int getEnergy();
    int getColor();
    int getToken(int id){return token[id];}
    //获取该玩家面前的基础效果牌
    QList<CardEntity*> getBasicEffect(){return this->basicEffect;}
    //返回玩家星数
    int getStar(){return this->star;}
    PlayerEntity* getNext(){return this->nextPlayer;}
    QList<CardEntity*> getHandCards(){return this->handCards;}
    int getRoleID(){return characterID;}
    bool tapped(){return this->tap;}
    bool isHandCardsMaxFixed(){return this->handCardsMaxFixed;}
    bool getYourturn();
    //void setInfo(int roleID);
    //QString getInfo();
    //static void loseMorale(int num,int isRed);

signals:
    //暴牌信号
    void overLoadCardsSIG();
    //明弃牌信号
    void discardWithFaceUpSIG();
    //暴牌询问信号(其实没什么用)
    void askForOverLoadSIG(QList<CardEntity*> hand,int overNum);
    //士气下降信号
    void loseMoraleSIG(int harmed,int* howMany,PlayerEntity* dst);
    //检查结束信号
    void checkEndSIG();
    //发送通讯信息
    void sendMessageSIG(int,QString);
    //移动卡牌到弃牌区(连接到BackgroundEngine)
    void toDiscardPileSIG(QList<CardEntity*> cards,bool show);
    void showHandCards(QList<CardEntity*>,PlayerEntity*);

protected:
    int id;//玩家id
    int characterID;
    QString name;
    int handCardsMax;


    int crossNum;
    int crossMax;
    int gem;
    int crystal;
    int energyMax;
    int color;
    float star;
    bool tap;//横置状态
    bool handCardsMaxFixed;//是否锁定手牌上限
    bool yourTurn;
    int seatNum;
    int token[3];
    int tokenMax[3];
    PlayerEntity* nextPlayer;//下家
    //QList<Status*> status;
    //QString info;
    QList<CardEntity*> handCards;//手牌
    QList<CardEntity*> basicEffect;//基础效果牌
    QList<CardEntity*> exclusiveEffect;//专属效果
    //游戏引擎，方便调用engine的各项功能
    BackgroundEngine* engine;

};

#endif // PLAYERENTITY_H
