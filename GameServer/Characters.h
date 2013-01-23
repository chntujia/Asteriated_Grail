#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "PlayerEntity.h"
class BackgroundEntity;
//¿ñÕ½Ê¿
class Berserker :public PlayerEntity
{
    Q_OBJECT
public :
    Berserker(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //¿ñ»¯
    void wild(QList<void*> args);
    //ËºÁÑ
    void tear(QList<void*> args);
    //ÑªÓ°¿ñµ¶
    void wildBlade(QList<void*> args);
    void wildBladeAsk(QList<void*> args);
    //ÑªÐÈÅØÏø
    void roar(QList<void*> args);
signals:
private:
    bool wildBladeUsed;
};

//¹­Ö®Å®Éñ
class BowLady : public PlayerEntity
{
    Q_OBJECT
public :
    BowLady(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //ÉÁµç¼ý
    void thunderArrow(QList<void*> args);
    //¹á´©Éä»÷
    void transfixtion(QList<void*> args);
    //ÉÁ¹âÏÝÚå
    void trap(QList<void*> args);
    //¾«×¼Éä»÷
    void accurateShoot(QList<void*> args);
    //¾Ñ»÷
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
    //±ùËªµ»ÑÔ
    void prayerOfFrost(QList<void*> args);
    //ÖÎÁÆÊõ
    void cure(QList<void*> args);
    //ÖÎÓúÖ®¹â
    void healingLight(QList<void*> args);
    //Á¯Ãõ
    void mercy(QList<void*> args);
    //Ê¥ÁÆ
    void layOnHands(QList<void*> args);
private:
    bool layOnHandsUsed;
};

//½£Ê¥
class JianSheng :public PlayerEntity
{
    Q_OBJECT
public :
    JianSheng(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //ÁÒ·ç¼¼
    void LieFengJi1(QList<void*> args);
    void LieFengJi2(QList<void*> args);
    //¼²·ç¼¼
    void JiFengJi(QList<void*> args);
    //Á¬Ðø¼¼
    void LianXuJi1(QList<void*> args);
    void LianXuJi2(QList<void*> args);
    //½£Ó°
    void JianYing1(QList<void*> args);
    void JianYing2(QList<void*> args);
    //Ê¥½£
    void ShengJian(QList<void*> args);
    void skillReset(QList<void*> args);

private:
    int attackCount;
    bool LieFengJiUsed,LianXuJiUsed,JianYingUsed;
};

//·âÓ¡
class FengYin : public PlayerEntity
{
    Q_OBJECT
public :
    FengYin(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //·¨Êõ¼¤µ´
    void FaShuJiDang1(QList<void*> args);
    void FaShuJiDang2(QList<void*> args);
    //·âÓ¡·¨Êõ
    void FengYinFaShu1(QList<void*> args);
    void FengYinFaShu2(QList<CardEntity *> cards, PlayerEntity *user);
    //ÎåÏµÊø¸¿
    void WuXiShuFu1(QList<void*> args);
    void WuXiShuFu2(PlayerEntity*,bool*);
    //·âÓ¡ÆÆËé
    void FengYinPoSui(QList<void*> args);

private:
    int shuFuID;
};

//Ä§½£
class MoJian :public PlayerEntity
{
    Q_OBJECT
public :
    MoJian(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //ºÚ°µÕð²ü
    void HeiAnZhenChan1(QList<void*> args);
    void HeiAnZhenChan2(QList<void*> args);
    //ÐÞÂÞÁ¬Õ¶
    void XiuLuoLianZhan1(QList<void*> args);
    void XiuLuoLianZhan2(QList<void*> args);
    //°µÓ°Á÷ÐÇ
    void AnYingLiuXing(QList<void*> args);
    //°µÓ°Äý¾Û
    void AnYingNingJu(QList<void*> args);
    //°µÓ°Ö®Á¦
    void AnYingZhiLi(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool HeiAnZhenChanUsed,XiuLuoLianZhanUsed;
};
//Ã°ÏÕ
class MaoXian : public PlayerEntity
{
    Q_OBJECT
public :
    MaoXian(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //ÆÛÕ©
    void QiZha(QList<void*> args);
    //ÍµÌì»»ÈÕ
    void TouTianHuanRi(QList<void*> args);
    //ÌØÊâ¼Ó¹¤
    void TeShuJiaGong(QList<void*> args);
    //Ã°ÏÕÕßÌìÌÃ
    void MaoXianZheTianTang(QList<void*> args);
signals:
private:
    \
};
//Ê¥Ç¹
class ShengQiang: public PlayerEntity
{
    Q_OBJECT
public:
    ShengQiang(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void HuiYao(QList<void*> args);
    void ChengJie(QList<void*> args);
    void ShengJi(QList<void*> args);
    void TianQiang(QList<void*> args);
    void DiQiang(QList<void*> args);
    void ShengGuangQiYu(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool ShengGuangQiYuUsed;
};

#endif // CHARACTERS_H
