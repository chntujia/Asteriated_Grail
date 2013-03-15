#ifndef BACKGROUNDENGINE_H
#define BACKGROUNDENGINE_H
#include <QObject>
#include "main.h"
#include "PlayerEntity.h"
#include "CardEntity.h"

//class PlayerEntity;

//伤害结构体
struct Harm
{
    int harmPoint;//伤害点数
    int type;//ATTACK攻击伤害，MAGIC法术伤害
};

//游戏后台类
class BackgroundEngine:public QObject
{
    Q_OBJECT
public:
    BackgroundEngine();
    void randomize(QList<int> *queue);
    //获取下一个玩家
    PlayerEntity* getNext(PlayerEntity* player){return player->getNext();}
    //获取前一个晚间
    PlayerEntity* getFront(PlayerEntity* player);
    //虚弱处理
    void weakProcess(PlayerEntity* player, int howMany=3);
    //魔弹处理
    void missileProcess(CardEntity* card,int src,int dst);
    //魔弹传递
    void missilePass(bool rightOrder,int dst,int src,bool *passed,int missilePoint);
    //摸牌函数
    void drawCards(int num, int harmed, PlayerEntity *dst, PlayerEntity *src=NULL);
    //摸牌置于盖牌中
    QList<CardEntity*> drwaCardsForCover(int num);
    //初始化
    void initial();
    //洗牌
    void shuffle(bool reShuffle);
    //清除本局数据
    void clearData();
    //中毒处理
    void posionProcess(PlayerEntity* player,CardEntity* card);
    //设置基本效果
    void effectApply(CardEntity* card,PlayerEntity* user,PlayerEntity* dst);
    //回合开始时检查是否有效果触发的函数
    void checkEffect(PlayerEntity* player);
    //设定当前玩家
    void setCurrentPlayer(class PlayerEntity* currentPlayer){this->currentPlayer = currentPlayer;}
    //获取当前玩家
    PlayerEntity* getCurrentPlayer(){return this->currentPlayer;}
    //回合开始阶段函数
    void turnBeginPhase(PlayerEntity* currentPlayer);
    //行动阶段函数
    void actionPhase();
    //增加指定类型行动次数,参数为attackLeft,magicLeft,specialLeft,attackOrMagicLeft,actionLeft之一
    void addActionNum(int kind);
    //void minusActionNum(int kind);
    //回合结束阶段函数
    void turnEndPhase();
    //重置剩余行动次数
    void resetActionNum(){
                        this->attackLeft = 0;
                        this->magicLeft = 0;
                        this->specialLeft = 0;
                        this->attackOrMagicLeft = 0;
                        this->actionLeft = 1;
                        }
    //某类型行动后调用,减少剩余行动次数.参数为ATTACK,MAGIC,SPECIAL之一
    void acted(int kind);
    //从手上使用卡牌时调用此函数.可能触发"展示某牌时"之类的信号
    void useCard(QList<CardEntity*> cards,PlayerEntity* user,PlayerEntity* dst = NULL,bool stay = false,int realCard=1);
    //从卡牌上获得基础伤害数据.目前只能从攻击牌上获得数据.
    Harm getHarmFromCard(CardEntity* card);//从卡牌上获得基础的伤害信息
    //根据ID查找玩家
    PlayerEntity* getPlayerByID(int ID);
    //以下函数判断当前玩家是否有权进行相应行动
    bool allowAttack();
    bool allowMagic();
    bool allowSpecial();
    //获取玩家总数
    int getPlayerNum(){return this->playerNum;}
    //设置玩家总数
    void setPlayerNum(int num){this->playerNum = num;}



    bool canAct();//判定是否可以行动,待完成
    void reDraw();//无法行动，重摸.待完成

    //以下为伤害时间轴函数
    void timeLine1(CardEntity* attackCard,PlayerEntity* src,PlayerEntity* dst,bool isActiveAttack);
    void timeLine2(CardEntity* harmCard,PlayerEntity* src,PlayerEntity* dst,bool isActiveAttack,int attackType,Harm harm);
    void timeLine3(Harm harm, PlayerEntity *src,PlayerEntity *dst,QString magicReason = "");
    void timeLine4(Harm harm, PlayerEntity *src,PlayerEntity *dst,QString magicReason = "");
    void timeLine5(Harm harm,PlayerEntity *src,PlayerEntity *dst,int cross);
    void timeLine6(Harm harm,PlayerEntity *src,PlayerEntity *dst);

    //法术牌处理函数
    void useMagicCard(int cardID,int srcID,int dstID = -1);
    //以下为调试用函数
    void showTest();
    PlayerEntity* setRole(int roleID, BackgroundEngine *engine, int id, int color);

public slots:
    //调试用槽
    void testSLOT(){qDebug()<<"test by backEngine";}
    //游戏开始槽
    void gameStart();
    //位次排列槽
    void seatPrearrange(int id, int isRed, QString name);
    void seatArrange();
    void role3Pick1();
    void role3Pick1Reply(int id,int roleID);
    //提供候选角色
    void BP();
    void roleRandom();
    void seatPostarrange();
    //游戏结束检查槽
    bool checkEnd();
    //弃牌加入弃牌堆槽
    void toDiscardPileSLOT(QList<CardEntity*> cards,bool show);

    void moveCardFrom(CardEntity* card);
    void moveCardToCover(CardEntity* card,int dstPlayerID);
    void moveCardFromCoverToDiscard(CardEntity* card,bool show);

signals:
    //以下各时间点的信号
    void timeLine1ProSIG(QList<void*> args);
    void timeLine1SIG(QList<void*> args);
    //时间轴2-未命中
    void timeLine2missedSIG(QList<void*> args);
    //时间轴2-命中
    void timeLine2hitSIG(QList<void*> args);
    void timeLine3SIG(QList<void*> args);
    void timeLine4SIG(QList<void*> args);
    void timeLine5SIG(QList<void*> args);
    void timeLine6SIG(QList<void*> args);
    void timeLine6DrawedSIG(QList<void*> args);
    //回合开始阶段信号
    void turnBeginPhaseSIG(QList<void*> args);
    //回合结束阶段信号
    void turnEndPhaseSIG(PlayerEntity*);
    //行动阶段信号
    void weakSIG(PlayerEntity*, bool*);
    void actionPhaseSIG(QList<void*> args);
    void additonalActionSIG(QList<void*> args);
    void tiaoXinPhaseSIG(PlayerEntity*, int*,bool*);
    //连接玩家对象信号-槽的信号
    void makePlayerConnectSIG(int);
    //发送通讯信息
    void sendMessageSIG(int playerID,QString content);
    //使用法术技能信号
    void skillMagic(QList<void*> args);
    void skillAttack(QList<void*> args);
    void skillSpecial(QList<void*> args);
    void shieldSIG(QList<void*> args);
    void showHandCards(QList<CardEntity*> cards,PlayerEntity* user);

    //手牌变化信号（巫女使用）
    void handCardsChange(PlayerEntity* dst);

    //以下信号暂无作用
    //询问应战动作信号
    void askForReply(QList<CardEntity*> hand,QString element,int userID);
    //询问治疗响应信号
    void askForHeal(Harm harm, PlayerEntity* src, PlayerEntity* dst, int *crossAvailable,QString magicReason = "");
    //询问行动信号
    void askForActSIG(QList<CardEntity*> hand,int currentID);

    //以下为各类行动结束信号
    void attackFinishSIG(QList<void*> args);
    void magicFinishSIG(QList<void*> args);
    void specialFinishSIG(QList<void*> args);
    //法术行动生效前
    void beforeMagicSIG(QList<void*> args);
    //士气下降前（红莲骑士热血沸腾状态）
    void beforeLoseMoralSIG(int harmed, int *howMany, PlayerEntity *dst);
    //士气下降信号
    void loseMoraleHeChengSIG(int harmed, int *howMany, PlayerEntity *dst);
    void loseMoraleSIG(int harmed,int* howMany,PlayerEntity* dst);
    //蝶舞锁士气信号
    void fixMoralHeChengSIG(int harmed, int *howMany, PlayerEntity *dst);
    void fixMoraleSIG(int harmed,int* howMany,PlayerEntity* dst);
    //真实士气下降信号（巫女红莲进入状态使用、灵魂长黄魂使用、魔枪幻影星辰【？】）
    void trueLoseMoraleSIG(int harmed, int* howMany, PlayerEntity* dst,PlayerEntity* src);
    void toInforDisplay(QString content);
    void usedShield(int userID);
private:
    //牌堆与弃牌堆
    QList<CardEntity*> pile,discardPile,discardPileCovered;
    //玩家们
    QList<PlayerEntity*> playerList;
    PlayerEntity *currentPlayer;
    //攻击行动、法术行动、特殊行动、攻击或法术行动、任意行动的剩余次数
    int attackLeft,magicLeft,specialLeft,attackOrMagicLeft,actionLeft;
    int playerNum;
    //游戏进行中标记
    bool playing;
    QList<int> red,blue;
    QString queue;
    QList<int> roles;
    QStringList nicknames;
};

#endif // BACKGROUNDENGINE_H
