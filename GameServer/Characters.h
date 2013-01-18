#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "PlayerEntity.h"
class BackgroundEntity;
//狂战士
class Berserker :public PlayerEntity
{
    Q_OBJECT
public :
    Berserker(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //狂化
    void wild(QList<void*> args);
    //撕裂
    void tear(QList<void*> args);
    //血影狂刀
    void wildBlade(QList<void*> args);
    void wildBladeAsk(QList<void*> args);
    //血腥咆哮
    void roar(QList<void*> args);
signals:
private:
    bool wildBladeUsed;
};

//弓之女神
class BowLady : public PlayerEntity
{
    Q_OBJECT
public :
    BowLady(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //闪电箭
    void thunderArrow(QList<void*> args);
    //贯穿射击
    void transfixtion(QList<void*> args);
    //闪光陷阱
    void trap(QList<void*> args);
    //精准射击
    void accurateShoot(QList<void*> args);
    //狙击
    void snipe(QList<void*> args);
signals:
private:

};

class MoDao : public PlayerEntity
{
    Q_OBJECT
public :
    MoDao(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void MoDanRongHe(QList<void*> args);
    void MoBaoChongJi(QList<void*> args);
    void HuiMeiFengBao(QList<void*> args);
signals:
private:
    \
};

class AnSha : public PlayerEntity
{
    Q_OBJECT
public :
    AnSha(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void ShuiYing(QList<void*> args);
    void QianXing(QList<void*> args);
    void FanShi(QList<void*> args);
    void AttackCheck(QList<void*> args);
signals:
private:
    \
};

class TianShi : public PlayerEntity
{
    Q_OBJECT
public :
    TianShi(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void FengZhiJieJin(QList<void*> args);
    void TianShiZhuFu(QList<void*> args);
    void TianShiZhiQiang(QList<void*> args);
    void TianShiZhiGe(QList<void*> args);
    void ShenZhiBiHu(int harmed,int*howMany,PlayerEntity*dst);
    void TianShiJiBan(int userID);
signals:
private:
    \
};
class Saintness : public PlayerEntity
{
    Q_OBJECT
public:
    Saintness(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    //冰霜祷言
    void prayerOfFrost(QList<void*> args);
    //治疗术
    void cure(QList<void*> args);
    //治愈之光
    void healingLight(QList<void*> args);
    //怜悯
    void mercy(QList<void*> args);
    //圣疗
    void layOnHands(QList<void*> args);
private:
    bool layOnHandsUsed;
};

//剑圣
class JianSheng :public PlayerEntity
{
    Q_OBJECT
public :
    JianSheng(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //烈风技
    void LieFengJi1(QList<void*> args);
    void LieFengJi2(QList<void*> args);
    //疾风技
    void JiFengJi(QList<void*> args);
    //连续技
    void LianXuJi1(QList<void*> args);
    void LianXuJi2(QList<void*> args);
    //剑影
    void JianYing1(QList<void*> args);
    void JianYing2(QList<void*> args);
    //圣剑
    void ShengJian(QList<void*> args);
    void skillReset(QList<void*> args);

private:
    int attackCount;
    bool LieFengJiUsed,LianXuJiUsed,JianYingUsed;
};

//封印
class FengYin : public PlayerEntity
{
    Q_OBJECT
public :
    FengYin(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //法术激荡
    void FaShuJiDang1(QList<void*> args);
    void FaShuJiDang2(QList<void*> args);
    //封印法术
    void FengYinFaShu1(QList<void*> args);
    void FengYinFaShu2(QList<CardEntity *> cards, PlayerEntity *user);
    //五系束缚
    void WuXiShuFu1(QList<void*> args);
    void WuXiShuFu2(PlayerEntity*,bool*);
    //封印破碎
    void FengYinPoSui(QList<void*> args);

private:
    int shuFuID;
};

//魔剑
class MoJian :public PlayerEntity
{
    Q_OBJECT
public :
    MoJian(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //黑暗震颤
    void HeiAnZhenChan1(QList<void*> args);
    void HeiAnZhenChan2(QList<void*> args);
    //修罗连斩
    void XiuLuoLianZhan1(QList<void*> args);
    void XiuLuoLianZhan2(QList<void*> args);
    //暗影流星
    void AnYingLiuXing(QList<void*> args);
    //暗影凝聚
    void AnYingNingJu(QList<void*> args);
    //暗影之力
    void AnYingZhiLi(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool HeiAnZhenChanUsed,XiuLuoLianZhanUsed;
};
//冒险
class MaoXian : public PlayerEntity
{
    Q_OBJECT
public :
    MaoXian(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //欺诈
    void QiZha(QList<void*> args);
    //偷天换日
    void TouTianHuanRi(QList<void*> args);
    //特殊加工
    void TeShuJiaGong(QList<void*> args);
    //冒险者天堂
    void MaoXianZheTianTang(QList<void*> args);
signals:
private:
    \
};
#endif // CHARACTERS_H
