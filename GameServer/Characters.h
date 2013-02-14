#ifndef CHARACTERS_H
#define CHARACTERS_H
#include "BackgroundEngine.h"
#include "PlayerEntity.h"


struct Harm;
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
    void snipe2(QList<void*> args);
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
    void layOnHands2(QList<void*> args);
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
    void JiFengJi1(QList<void*> args);
    void JiFengJi2(QList<void*> args);
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
    void HeiAnZhenChan3(QList<void *> args);
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
    bool HeiAnZhenChanUsed,XiuLuoLianZhanUsed,isHeiAnZhenChan;
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
    void TouTianHuanRi2(QList<void*> args);
    //特殊加工
    void TeShuJiaGong(QList<void*> args);
    void TeShuJiaGong2(QList<void*> args);
    //冒险者天堂
    void MaoXianZheTianTang(QList<void*> args);
signals:
private:
    
};


//元素
class YuanSu : public PlayerEntity
{
    Q_OBJECT
public :
    YuanSu(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //元素吸收
    void YuanSuXiShou(QList<void*> args);
    //元素法术
    void YuanSuFaShu(QList<void*> args);
    void YuanSuFaShu2(QList<void*> args);
    //元素点燃
    void YuanSuDianRan(QList<void*> args);
    void YuanSuDianRan2(QList<void*> args);
    //月光
    void YueGuang(QList<void*> args);

signals:
private:
    bool ignite;
    
};
//仲裁
class ZhongCai : public PlayerEntity
{
    Q_OBJECT
public:
    ZhongCai(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    //仪式中断
    void YiShiZhongDuan(QList<void*> args);
    //末日审判
    void MoRiShenPan(QList<void*> args);
    //审判浪潮
    void ShenPanLangChao(QList<void*> args);
    //仲裁仪式
    void ZhongCaiYiShi1(QList<void*> args);
    void ZhongCaiYiShi2(QList<void *> args);
    //判决天平
    void PanJueTianPing(QList<void*> args);
    //重置启动次数
    void skillReset(QList<void*> args);
signals:
private:
    bool startUsed;
};

//勇者
class YongZhe : public PlayerEntity
{
    Q_OBJECT
public:
    YongZhe(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    //怒吼
    void NuHou(QList<void*> args);
    void NuHou1(QList<void*> args);
    void NuHou2(QList<void*> args);
    //明镜止水
    void MingJingZhiShui(QList<void*> args);
    //禁断之力
    void JinDuanZhiLi1(QList<void*> args);
    void JinDuanZhiLi2(QList<void*> args);
    //死斗
    void SiDou(QList<void*> args);
    //挑衅
    void TiaoXin1(QList<void*> args);
    void TiaoXin2(PlayerEntity*,int*, bool *canGiveUp);
    void TiaoXin3(PlayerEntity*);
    //void TiaoXin4(QList<void *> args);
    //精疲力竭
    void JingPiLiJie1(QList<void*> args);
    void JingPiLiJie2(QList<void *> args);
    void JingPiLiJie3(QList<void *> args);
private:
    int tiaoXinID,fireNum;
    bool nuHouUsed,tiaoXinUsed,tiaoXinChuFa;
    bool jinDuanUsed;
};

//圣枪
class ShengQiang: public PlayerEntity
{
    Q_OBJECT
public:
    ShengQiang(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    void HuiYao(QList<void*> args);
    void ChengJie(QList<void*> args);
    void HuiYao2(QList<void*> args);
    void ChengJie2(QList<void*> args);
    void ShengJi(QList<void*> args);
    void TianQiang(QList<void*> args);
    void DiQiang(QList<void*> args);
    void ShengGuangQiYu(QList<void*> args);
    void ShengGuangQiYu2(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool ShengGuangQiYuUsed;
    bool TianQiangUsed;
};
//祈祷师
class QiDao : public PlayerEntity
{
    Q_OBJECT
public :
    QiDao(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine* engine);
public slots:
    //威力赐福
    void WeiLiCiFu1(QList<void*> args);
    void WeiLiCiFu2(QList<void*> args);
    //迅捷赐福
    void XunJieCiFu1(QList<void*> args);
    void XunJieCiFu2(QList<void*> args);
    void XunJieCiFu3(QList<void *> args);
    //光辉信仰
    void GuangHuiXinYang(QList<void*> args);
    //漆黑信仰
    void QiHeiXinYang(QList<void*> args);
    //法力潮汐
    void FaLiChaoXi1(QList<void*> args);
    void FaLiChaoXi2(QList<void*> args);
    //启动 祈祷
    void Pray1(QList<void*> args);
    void Pray2(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool XunJieCounted;
    bool FaLiChaoXiUsed;
};

//神官
class ShenGuan: public PlayerEntity
{
    Q_OBJECT
public:
    ShenGuan(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void ShenShengQiShi(QList<void*> args);
    void ShenShengQiFu(QList<void*> args);
    void ShuiZhiShenLi(QList<void*> args);
    void ShengShiShouHu(Harm harm, PlayerEntity* src, PlayerEntity* dst, int *crossAvailable,QString magicReason = "");
    void ShenShengQiYue(QList<void*> args);
    void ShenShengLingYu(QList<void*> args);
};

//死灵
class SiLing: public PlayerEntity
{
    Q_OBJECT
public:
    SiLing(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void BuXiu(QList<void*> args);
    void ShengDu(Harm harm, PlayerEntity* src, PlayerEntity* dst, int *crossAvailable,QString magicReason = "");
    void WenYi(QList<void*> args);
    void SiWangZhiChu(QList<void*> args);
    void MuBeiYunLuo(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool SiWangZhiChuUsed;
};
//贤者
class XianZhe : public PlayerEntity
{
    Q_OBJECT
public:
    XianZhe(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void ZhiHuiFaDian(QList<void*> args);
    void FaShuFanTan(QList<void*> args);
    void MoDaoFaDian(QList<void*> args);
    void ShengJieFaDian(QList<void*> args);
private:
};


//格斗
class GeDouJia : public PlayerEntity
{
    Q_OBJECT
public:
    GeDouJia(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void NianQiLiChang(QList<void*> args);
    void CangYanXuLi(QList<void*> args);
    void XuLiYiJi2(QList<void *> args);
    void XuLiYiJi3(QList<void *> args);
    void NianDan(QList<void*> args);
    void CangYanZhiHun2(QList<void *> args);
    void BaiShiHuanLingQuan1(QList<void*> args);
    void BaiShiHuanLingQuan2(QList<void *> args);
    void BaiShiHuanLingQuan3(QList<void *> args);
    void DouShenTianQu(QList<void*> args);
    void BaiShiTargetSet(QList<void*>args);
    void BaiShiQuXiao1(QList<void*> args);//攻击其他人。。t2
    void BaiShiQuXiao2(QList<void *> args);//法术行动
    void BaiShiQuXiao3(QList<void *> args);//特殊行动。。
    void skillReset(QList<void*> args);
private:
    int BaiShiTargetID;
    bool BaiShiUsed;
    bool XuLiUesd;
    bool BaiShiFirst;
    bool StartUsed;
    bool CangYanUsed;
};

//巫女
class WuNv: public PlayerEntity
{
    Q_OBJECT
public:
    WuNv(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void TongShengGongSi(QList<void*> args);
    void XueZhiAiShang(QList<void*> args);
    void ToPuTongXingtai(PlayerEntity* dst);
    void ToLiuXueXingTai(int harmed, int* howMany, PlayerEntity* dst);
    void LiuXue(QList<void*> args);
    void NiLiu(QList<void*> args);
    void XueZhiBeiMing(QList<void*> args);
    void XueZhiZuZhou(QList<void*> args);
private:
    int TongShengID;
};

//红莲骑士 28
class HongLian:public PlayerEntity
{
    Q_OBJECT
public:
    HongLian(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void XingHongShengYue(QList<void*> args);
    void XingHongXinYang(Harm harm, PlayerEntity* src, PlayerEntity* dst, int *crossAvailable,QString magicReason = "");
    void XueXingDaoYan(QList<void*> args);
    void ShaLuShengYan(QList<void*> args);
    void ToReXueFeiTeng(int harmed, int* howMany, PlayerEntity* dst);
    void ReXueFeiTeng(int harmed, int* howMany, PlayerEntity* dst);
    void OutReXueFeiTeng(PlayerEntity* currunt);
    void JieJiaoJieZao1(QList<void*> args);
    void JieJiaoJieZao2(QList<void *> args);
    void XingHongShiZi(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool XingHongShengYueUsed;
};

//灵魂
class LingHun : public PlayerEntity
{
    Q_OBJECT
public:
    LingHun(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void LingHunTunShi(int harmed,int*howMany,PlayerEntity*dst);
    void LingHunTunShi2(QList<void*> args);
    void LingHunTunShi3(int harmed,int*howMany,PlayerEntity*dst);
    void LingHunZhaoHuan(QList<void*> args);
    void LingHunJingXiang(QList<void*> args);
    void LingHunZhenBao(QList<void*> args);
    void LingHunCiYu(QList<void*> args);
    void LingHunZhuanHuan(QList<void*> args);
    void LingHunLianJie1(QList<void*> args);
    void LingHunLianJie2(QList<void*> args);
    void LingHunLianJie3(QList<void *> args);
    void LingHunZengFu(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    int LianJieID;
    bool LianJieUsed;
    bool StartUsed;
    bool LianJieChuFa;
    bool HeCheng;
};

class LingFu:public PlayerEntity
{
    Q_OBJECT
public:
    LingFu(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void leiMing(QList<void*> args);
    void fengXing(QList<void*> args);
    void nianZhou(QList<void*> args);
    void lingLiBengJie(Harm &hurt);
    void baiGuiYeXing(QList<void*> args);
private:

};

//魔枪
class MoQiang : public PlayerEntity
{
    Q_OBJECT
public:
    MoQiang(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void AnZhiJieFang1(QList<void*> args);
    void AnZhiJieFang2(QList<void *> args);
    void AnZhiJieFang3(QList<void *> args);
    void HuanYingXingChen(QList<void*> args);
    void HuanYingXingChen2(int harmed,int*howMany,PlayerEntity*dst);
    void AnZhiBiZhang(QList<void*> args);
    void ChongYing1(QList<void*> args);
    void ChongYing2(QList<void *> args);
    void ChongYing3(QList<void *> args);
    void ChongYing4(QList<void *> args);
    void QiHeiZhiQiang(QList<void*> args);
    void skillReset(QList<void*> args);
private:
    bool JieFangUsed;
    bool JieFangFirst;
    bool HuanYingUsed;
    bool ChongYingUsed;
    bool StartUsed;
    int AddAttackPoint;
};


//剑帝
class JianDi : public PlayerEntity
{
    Q_OBJECT
public:
    JianDi(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void JianHunShouHu(QList<void*> args);
    void YangGong(QList<void*> args);
    void JianQiZhan(QList<void*> args);
    void TianYuEMo(QList<void*> args);
    void TianShiZhiHun(QList<void*> args);
    void TianShiZhiHun1(QList<void *> args);
    void TianShiZhiHun2(QList<void *> args);
    void EMoZhiHun(QList<void*> args);
    void EMoZhiHun1(QList<void *> args);
    void EMoZhiHun2(QList<void *> args);
    void BuQuYiZhi(QList<void*> args);
    void BuQuYiZhi2(QList<void *> args);
private:
    int TianshiOrEmo;//0-皆不是，1-天使，2-恶魔
    bool HunUsed;
};


class DieWu: public PlayerEntity
{
    Q_OBJECT
public:
    DieWu(BackgroundEngine* engine,int id,int color);
    void makeConnection(BackgroundEngine *engine);
    void DiaoLing(int cardID, bool removed);
public slots:
    void WuDong(QList<void*> args);
    void DuFen(QList<void*> args);
    void ChaoSheng(QList<void*> args);
    void JingHuaShuiYue(QList<void*> args);
    void DiaoLingFix(int harmed, int* howMany, PlayerEntity* dst);
    void DiaoLingFixHeCheng(int harmed, int* howMany, PlayerEntity* dst);
    void YongHua(QList<void*> args);
    void DaoNiZhiDie(QList<void*> args);
    void DaoNiZhiDieJudge(Harm harm, PlayerEntity* src, PlayerEntity* dst, int *crossAvailable,QString magicReason = "");
    void skillReset(QList<void*> args);
private:

};
/*
class MoGong: public PlayerEntity
{
    Q_OBJECT
public:
    MoGong(BackgroundEngine* engine, int id, int color);
    void makeConnection(BackgroundEngine *engine);
public slots:
    void MoGuanChongJi(QList<void*> args);
    void MoGuanChongJiHit(QList<void*> args);
    void MoGuanChongJiMiss(QList<void *> args);
    void LeiGuangSanShe(QList<void*> args);
    void DuoChongSheJi(QList<void*> args);

};
*/

#endif // CHARACTERS_H
