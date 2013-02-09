#include "Characters.h"
#include "BackgroundEngine.h"
#include "Server.h"

Berserker::Berserker(BackgroundEngine *engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = BERSERKER;
    this->star = 3;
    this->makeConnection(engine);
}

void Berserker::wild(QList<void*> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;

    Harm *harm = (Harm*)args[2];
    if(harm->type != ATTACK)
        return;
    harm->harmPoint++;
    coder.notice("狂战士发动【狂化】");
}

void Berserker::tear(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(this->getGem() <= 0)
        return;

    //此处询问是否使用
    coder.askForSkill(this->getID(),"撕裂");
    if(messageBuffer::readInfor() == 0)
        return;

    coder.notice("狂战士发动【撕裂】");
    this->setGem(this->getGem()-1);
    Harm *harm = (Harm*)args[2];
    harm->harmPoint += 2;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
}

void Berserker::wildBladeAsk(QList<void*> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    this->wildBladeUsed = false;
    if(!*(bool*)args[4])
        return;
    CardEntity *card = (CardEntity*)args[3];
    if(!card->getHasSpeciality())
        return;
    if(card->getProperty() != tr("血"))
        return;
    if(!card->getSpecialityList().contains(tr("血影狂刀")))
        return;
    int handNum = ((PlayerEntity*)args[1])->getHandCardNum();
    if(handNum != 2 && handNum != 3)
        return;
    coder.askForSkill(this->getID(),"血影狂刀");
    if(messageBuffer::readInfor() == 1)
        this->wildBladeUsed = true;
    coder.notice("狂战士发动【血影狂刀】");
}

void Berserker::wildBlade(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    CardEntity *card = (CardEntity*)args[3];
    if(!card->getHasSpeciality())
        return;
    if(card->getProperty() != tr("血"))
        return;
    if(card->getSpecialityList().contains(tr("血影狂刀")))
    {
        if(!this->wildBladeUsed)
            return;
        Harm *harm = (Harm*)args[2];
        switch(((PlayerEntity*)args[1])->getHandCards().length())
        {
        case 3:
            harm->harmPoint += 1;
            break;
        case 2:
            harm->harmPoint += 2;
        default:
            break;
        }
    }
}

void Berserker::roar(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    CardEntity *card = (CardEntity*)args[3];
    if(!card->getHasSpeciality())
        return;
    if(card->getProperty() != tr("血"))
        return;

    if(card->getSpecialityList().contains(tr("血腥咆哮")))
    {
        if(((PlayerEntity*)args[1])->getCrossNum() == 2)
        {
            //询问是否发动
            coder.askForSkill(this->getID(),"血腥咆哮");
            if(messageBuffer::readInfor() == 0)
                return;
            coder.notice("狂战士发动【血腥咆哮】");
            *(int*)args[5] = NOMISS;
        }
    }
}

void Berserker::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(wild(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(tear(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(wildBladeAsk(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(wildBlade(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(roar(QList<void*>)));
}

BowLady::BowLady(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = BOWLADY;
    this->star = 3;
    this->makeConnection(engine);
}

void BowLady::thunderArrow(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(card->getElement() == tr("thunder"))
    {
        *((int*)args[5]) = NOREPLY;
        coder.notice("弓之女神发动【闪电箭】");
    }
}

void BowLady::transfixtion(QList<void *> args)
{
    if(this !=  (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4] || handCards.size()==0)
        return;
    coder.askForSkill(this->getID(),"贯穿射击");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;
    else
    {
        CardEntity *card = getCardByID(ans.CardID);
        QList<CardEntity*> cards;
        cards << card;
        this->removeHandCards(cards,true);
        coder.notice("弓之女神发动【贯穿射击】");
        coder.discardNotice(this->getID(),1,"y",cards);
        Harm transHurt;
        transHurt.harmPoint = 2;
        transHurt.type = MAGIC;
        engine->timeLine3(transHurt,this,(PlayerEntity*)args[1],"贯穿射击");
    }
}

void BowLady::accurateShoot(QList<void *> args)
{
    //精准射击连接于timeLine1SIG(QList<void*>)信号，同样我们应该先仔细查看信号的参数列表。
    //首先仍然是检查是否符合技能发动条件
    if(this !=  (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(card->getProperty() != tr("技"))
        return;
    if(card->getHasSpeciality() == 0)
        return;
    if(card->getSpecialityList().contains(tr("精准射击")))
    {
        //此处使用35号通讯协议询问client是否发动技能
        coder.askForSkill(this->getID(),"精准射击");
        //请注意,messageBuffer::readInfor()会从缓冲中读取回复.这是个阻塞函数,直到它从缓冲中获得回复信息才会返回
        if(1 == messageBuffer::readInfor())
        {
            //根据36号协议,回复"1"表示玩家发动此技能
            //通过信号传递来的指针修改伤害数据
            Harm* harm = (Harm*)args[2];
            harm->harmPoint--;
            *(int*)args[5] = NOMISS;
            coder.notice("弓之女神发动【精准射击】");
        }
    }
}

void BowLady::trap(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != BOWLADY_TRAP)
        return;
    coder.notice("弓之女神发动【闪光陷阱】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    this->engine->useCard(cards,this,dst);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"闪光陷阱");
}

void BowLady::snipe(QList<void *> args)
{
    BatInfor* magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != BOWLADY_SNIPE)
        return;
    if(this->getGem() + this->getCrystal() < 1)
        return;
    coder.notice("弓之女神对玩家"+QString::number(magic->dstID)+"发动【狙击】");
    switch(magic->infor2)
    {
    case 0:
        this->crystal--;
        break;
    case 1:
        this->gem--;
        break;
    default:
        break;
    }
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());

    PlayerEntity* dst = this->engine->getPlayerByID(magic->dstID);
    if(dst->getHandCardNum() < 5)
    {
        this->engine->drawCards(5 - dst->getHandCardNum(),NOTHARMED,dst);
    }
    this->engine->addActionNum(ATTACK);
}

void BowLady::snipe2(QList<void*>args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=304)
        return;
    coder.notice("弓之女神使用【狙击】的额外攻击行动");
}

void BowLady::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(accurateShoot(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(thunderArrow(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(transfixtion(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(trap(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(snipe(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(snipe2(QList<void*>)));
}

MoDao::MoDao(BackgroundEngine* engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 8;
    this->star = 3;
    this->makeConnection(engine);
}

void MoDao::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoDanRongHe(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoBaoChongJi(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(HuiMeiFengBao(QList<void*>)));
}

void MoDao::MoDanRongHe(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 801 && magic->infor1 != 802)
        return;
    coder.notice("魔导师发动【魔弹融合】");
    QList<CardEntity*> cards;
    CardEntity* card;
    card=getCardByID(magic->CardID);
    cards << card;
    engine->useCard(cards,this,engine->getPlayerByID(magic->dstID));
    this->engine->missileProcess(card,magic->srcID,magic->dstID);
}

void MoDao::MoBaoChongJi(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 803)
        return;
    int dst1=magic->dstID;
    int dst2=magic->infor2;
    QString msg;
    msg="魔导师对玩家"+QString::number(dst1)+"和玩家"+QString::number(dst2)+"发动【魔爆冲击】";
    coder.notice(msg);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);

    bool flag1,flag2,flag;
    flag1=flag2=true;
    flag=false;
    PlayerEntity* ptr = engine->getNext(this);
    do
    {
        if(flag1 && ptr->getID()==dst1)
        {
            flag1=false;
            coder.askForDiscardMagic(dst1);

            BatInfor ans = messageBuffer::readBatInfor();
            if(ans.reply == 0)
            {
                Harm MoBao;
                MoBao.harmPoint = 2;
                MoBao.type = MAGIC;
                engine->timeLine3(MoBao,this,ptr,"魔爆冲击");
                flag=true;
                if(engine->checkEnd())
                    break;
            }
            else
            {
                CardEntity *card = getCardByID(ans.CardID);
                QList<CardEntity*> cards;
                cards << card;
                ptr->removeHandCards(cards,true);
                coder.discardNotice(dst1,1,"y",cards);
            }
        }
        else if(flag2 && ptr->getID()==dst2)
        {
            flag2=false;
            coder.askForDiscardMagic(dst2);

            BatInfor ans = messageBuffer::readBatInfor();
            if(ans.reply == 0)
            {
                Harm MoBao;
                MoBao.harmPoint = 2;
                MoBao.type = MAGIC;
                engine->timeLine3(MoBao,this,ptr,"魔爆冲击");
                flag=true;
                if(engine->checkEnd())
                    break;
            }
            else
            {
                CardEntity *card = getCardByID(ans.CardID);
                QList<CardEntity*> cards;
                cards << card;
                ptr->removeHandCards(cards,true);
                coder.discardNotice(dst2,1,"y",cards);
            }
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
    if(flag)
    {
        teamArea.setGem(color,teamArea.getGem(color)+1);
        coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    }

}

void MoDao::HuiMeiFengBao(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 804)
        return;
    int dst1=magic->dstID;
    int dst2=magic->infor2;
    QString msg;
    msg="魔导师对玩家"+QString::number(dst1)+"和玩家"+QString::number(dst2)+"发动【毁灭风暴】";
    coder.notice(msg);
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());

    bool flag1,flag2;
    flag1=flag2=true;
    PlayerEntity* ptr = engine->getNext(this);
    do
    {
        if(flag1 && ptr->getID()==dst1)
        {
            flag1=false;
            Harm HuiMie;
            HuiMie.harmPoint = 2;
            HuiMie.type = MAGIC;
            engine->timeLine3(HuiMie,this,ptr,"毁灭风暴");
            if(engine->checkEnd())
                break;
        }
        else if(flag2 && ptr->getID()==dst2)
        {
            flag2=false;
            Harm HuiMie;
            HuiMie.harmPoint = 2;
            HuiMie.type = MAGIC;
            engine->timeLine3(HuiMie,this,ptr,"毁灭风暴");
            if(engine->checkEnd())
                break;
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
}
AnSha:: AnSha(BackgroundEngine* engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 5;
    this->star = 3;
    this->makeConnection(engine);
}

void AnSha::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(FanShi(QList<void*>)));
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(ShuiYing(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(QianXing(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(AttackCheck(QList<void*>)));
}

void AnSha::ShuiYing(QList<void*> args)
{
    int howMany;
    if(this != ((PlayerEntity*)args[1])||handCards.size()==0)
        return;
    coder.askForSkill(this->getID(),"水影");

    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    howMany=arg[0].toInt();
    if(howMany == 0)
        return;
    else
    {
        QList<CardEntity*>cards;
        arg=arg[1].split(",");
        int i;
        for(i=0;i<howMany;i++)
        {
            cards<<getCardByID(arg[i].toInt());
        }
        this->removeHandCards(cards,true);
        coder.notice("暗杀者发动【水影】");
        coder.discardNotice(this->getID(),howMany,"y",cards);
    }
}

void AnSha::QianXing(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap==1)
    {
        setTap(0);
        coder.tapNotice(id,0,"【普通形态】");
        this->addHandCardsRange(1);
        coder.handcardMaxNotice(id,handCardsMax);
    }
    if(getGem()==0)
        return;
    coder.askForSkill(id,"潜行");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    this->gem--;
    coder.energyNotice(id,this->getGem(),this->getCrystal());
    coder.notice("暗杀者发动【潜行】");
    setTap(1);
    coder.tapNotice(id,1,"【潜行形态】");
    this->addHandCardsRange(-1);
    coder.handcardMaxNotice(id,handCardsMax);
}
/*
#define NOTHARMED  0
#define ATTACKHARM 1
#define MAGICHARM  2
*/
void AnSha::FanShi(QList<void*> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type!=1)
            return;
    coder.notice("暗杀者发动【反噬】");
    engine->drawCards(1,0,(PlayerEntity*)args[0]);

}

void AnSha::AttackCheck(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0] || tap==0)
        return;
    if(*(bool*)args[4])
    {
        ((Harm*)args[2])->harmPoint = 2+getEnergy();
        *((int*)args[5]) = NOREPLY;
        coder.notice("暗杀者发动【潜行】效果");
    }
}

TianShi::TianShi(BackgroundEngine* engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 7;
    this->star = 3;
    this->makeConnection(engine);
}

void TianShi::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(FengZhiJieJin(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TianShiZhuFu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TianShiZhiQiang(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(TianShiZhiGe(QList<void*>)));
    connect(engine,SIGNAL(loseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(ShenZhiBiHu(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(usedShield(int)),this,SLOT(TianShiJiBan(int)));
}

void TianShi::FengZhiJieJin(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 701)
        return;
    coder.notice("天使发动【风之洁净】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);
    int dstID=magic->dstID;
    PlayerEntity* ptr=engine->getPlayerByID(dstID);
    CardEntity*card=getCardByID(magic->infor2);
    if(ptr->removeBasicEffect(card))
    {
        coder.notice("天使移除玩家"+QString::number(dstID)+"面前的"+card->getName());
        TianShiJiBan(id);
    }
}

void TianShi::TianShiZhuFu(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 702)
        return;
    bool flag1,flag2;
    int n=2;
    flag1=true;
    flag2=false;
    int howMany=magic->infor2;
    int dst1=magic->dstID;
    int dst2=-1;

    QString msg;
    msg="天使对玩家"+QString::number(dst1);
    if(howMany==2)
    {
        dst2=magic->infor3;
        msg+="和玩家"+QString::number(dst2);
        flag2=true;
        n=1;
    }
    msg+="发动【天使祝福】";
    coder.notice(msg);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);

    PlayerEntity* ptr = engine->getNext(this);
    do
    {
        if(flag1 && ptr->getID()==dst1)
        {
            cards.clear();
            flag1=false;
            if(engine->getPlayerByID(dst1)->getHandCardNum()>=n)
            {
                coder.askToGiveCard(dst1,n);
                cards=messageBuffer::readCardID(n);
                ptr->giveHandCards(cards,this);
            }
            else if(engine->getPlayerByID(dst1)->getHandCardNum()==1)
            {
                coder.askToGiveCard(dst1,1);
                cards=messageBuffer::readCardID(1);
                ptr->giveHandCards(cards,this);
            }
        }
        else if(flag2 && ptr->getID()==dst2)
        {
            cards.clear();
            flag2=false;
            if(engine->getPlayerByID(dst2)->getHandCardNum()>=n)
            {
                coder.askToGiveCard(dst2,n);
                cards=messageBuffer::readCardID(n);
                ptr->giveHandCards(cards,this);
            }
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
}

void TianShi::TianShiZhiQiang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 703)
        return;
    coder.notice("天使发动【天使之墙】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}

void TianShi::TianShiZhiGe(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0] || gem+crystal==0)
        return;
    PlayerEntity*ptr=this;
    int i,n=engine->getPlayerNum();
    for(i=0;i<n;i++)
    {
        if(!ptr->getBasicEffect().empty())
            break;
        ptr=ptr->getNext();
    }
    if(i==n)
        return;
    coder.askForSkill(id,"天使之歌");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply==0)
        return;
    if(ans.infor1)
        gem--;
    else
        crystal--;
    coder.energyNotice(id,gem,crystal);
    int dstID=ans.dstID;
    ptr=engine->getPlayerByID(dstID);
    CardEntity*card=getCardByID(ans.CardID);
    if(ptr->removeBasicEffect(card))
    {
        coder.notice("天使移除玩家"+QString::number(dstID)+"面前的"+card->getName());
        TianShiJiBan(id);
    }
}

/*
#define NOTHARMED  0
#define ATTACKHARM 1
#define MAGICHARM  2
*/
void TianShi::ShenZhiBiHu(int harmed,int*howMany,PlayerEntity*dst)
{
    if(harmed!=2 || dst->getColor()!=color || gem+crystal==0|| *howMany==0)
        return;
    coder.askForSkill(id,"神之庇护",TOQSTR(*howMany));
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;
    else
    {
        *howMany-=ans.infor1+ans.infor2;
        gem-=ans.infor1;
        crystal-=ans.infor2;
        coder.energyNotice(this->getID(),gem,crystal);
        coder.notice("天使发动【神之庇护】抵御"+TOQSTR(ans.infor1+ans.infor2)+"点士气下降");
    }

}

void TianShi::TianShiJiBan(int userID)
{
    if(userID!=id)
        return;
    coder.askForSkill(id,"天使羁绊");
    BatInfor ans = messageBuffer::readBatInfor();
    int dstID=ans.dstID;
    PlayerEntity* player= engine->getPlayerByID(dstID);
    coder.notice("天使对玩家"+TOQSTR(dstID)+"发动【天使羁绊】");
    int cross=player->getCrossNum();
    if(player->getCrossMax()>cross)
    {
        player->setCrossNum(cross+1);
        coder.crossChangeNotice(dstID,cross+1);
    }
}
//圣女初始化
Saintness::Saintness(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 6;
    this->star = 3;
    this->makeConnection(engine);
}

//冰霜祷言
void Saintness::prayerOfFrost(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(card->getElement() != tr("water"))
        return;
    coder.askForSkill(this->getID(),"冰霜祷言");
    BatInfor ans = messageBuffer::readBatInfor();
    int dstID= ans.dstID;
    PlayerEntity* player= engine->getPlayerByID(dstID);
    coder.notice("圣女对玩家"+TOQSTR(dstID)+"发动【冰霜祷言】，增加1点治疗");
    player->setCrossNum(player->getCrossNum()+1);
    coder.crossChangeNotice(dstID,player->getCrossNum());
}

//治疗术
void Saintness::cure(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 602)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    int dstID=dst->getID();
    coder.notice("圣女对玩家"+TOQSTR(dstID)+"发动【治疗术】，增加2点治疗");
    this->engine->useCard(cards,this,dst);        
    dst->setCrossNum(dst->getCrossNum()+2);
    coder.crossChangeNotice(dstID, dst->getCrossNum());
}

//治愈之光
void Saintness::healingLight(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 603)
        return;
    coder.notice("圣女发动【治愈之光】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* player = engine->getPlayerByID(magic->dstID);
    this->engine->useCard(cards,this,player);
    int dst[3], n;
    n=magic->infor2;
    dst[0] = magic->dstID;
    if (n>1)
        dst[1] = magic->infor3;
    if (n>2)
        dst[2] = magic->infor4;
    for(int i=0;i<n; i++)
    {
        player = engine->getPlayerByID(dst[i]);
        coder.notice("圣女为玩家"+TOQSTR(dst[i])+"增加1点治疗");
        player->setCrossNum(player->getCrossNum()+1);
        coder.crossChangeNotice(dst[i],player->getCrossNum());
    }
}

//怜悯
void Saintness::mercy(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap == 1)
        return;
    if(getGem()==0)
        return;
    coder.askForSkill(this->getID(),"怜悯");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("圣女发动【怜悯】");
    setTap(1);
    setHandCardsMaxFixed(true, 7);
    coder.tapNotice(id,1,"【怜悯形态】");
    coder.handcardMaxNotice(id,handCardsMax);
}

//圣疗
void Saintness::layOnHands(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 605)
        return;
    int dst[3], addcross[3];
    dst[0] = magic->dstID;
    dst[1] = magic->infor2;
    dst[2] = magic->infor3;
    PlayerEntity* player;
    for(int i=0; i<3; i++)
        addcross[i]=0;
    switch(magic->infor4)
    {
    case 0:
        this->crystal--;
        break;
    case 1:
        this->gem--;
        break;
    default:
        break;
    }
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(dst[i] == dst[j])
            {
                addcross[j]++;
                break;
            }
    for (int i=0; i<3; i++)
    {
        if(addcross[i] == 0)
            continue;
        player = engine->getPlayerByID(dst[i]);
        coder.notice("圣女使用【圣疗】为玩家"+TOQSTR(dst[i])+"增加"+TOQSTR(addcross[i])+"点治疗");
        player->setCrossNum(player->getCrossNum()+addcross[i]);
        coder.crossChangeNotice(dst[i], player->getCrossNum());
    }
    this->engine->addActionNum(ATTACKORMAGIC);
}

void Saintness::layOnHands2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=606)
        return;
    coder.notice("圣女使用【圣疗】的攻击或法术行动");
}

//
void Saintness::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(prayerOfFrost(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(cure(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(healingLight(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(mercy(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(layOnHands(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(layOnHands2(QList<void*>)));
}

JianSheng::JianSheng(BackgroundEngine *engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 1;
    this->star = 3;
    this->makeConnection(engine);
}

//烈风技
void JianSheng::LieFengJi1(QList<void*> args)
{
    LieFengJiUsed=false;
    if(this != (PlayerEntity*)args[0])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(!card->getSpecialityList().contains(tr("烈风技")))
        return;
    PlayerEntity* dst=(PlayerEntity*)args[1];
    bool flag=false;
    for(int i = 0;i < dst->getBasicEffect().size();i++)
        if(dst->getBasicEffect().at(i)->getMagicName() == SHIELDCARD || dst->getBasicEffect().at(i)->getSpecialityList().contains(tr("天使之墙")))
        {
            flag=true;
            break;
        }
    if(!flag)
        return;
    coder.askForSkill(id,"烈风技");
    if(messageBuffer::readInfor() == 0)
        return;
    *((int*)args[5]) = NOREPLY;
    coder.notice("剑圣发动【烈风技】");
    LieFengJiUsed=true;
}

void JianSheng::LieFengJi2(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!LieFengJiUsed)
        return;
    *((bool*)args[2]) =  false;
}

//疾风技
void JianSheng::JiFengJi1(QList<void *> args)
{
    PlayerEntity* myself=(PlayerEntity*)args[0];
    if(this != myself ||!*(bool*)args[4])
        return;
    CardEntity* card = (CardEntity*)args[3];
    if(!card->getSpecialityList().contains(tr("疾风技")))
        return;
    coder.notice("剑圣发动【疾风技】");
    engine->addActionNum(ATTACK);
}
void JianSheng::JiFengJi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=103)
        return;
    coder.notice("剑圣使用【疾风技】的额外攻击行动");
}

//连续技
void JianSheng::LianXuJi1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(getHandCardNum()<=0 ||LianXuJiUsed)
        return;
    engine->addActionNum(ATTACK);
    LianXuJiUsed=true;
}
void JianSheng::LianXuJi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=101)
        return;
    coder.notice("剑圣发动【连续技】");
}

//剑影
void JianSheng::JianYing1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(this->getEnergy() <= 0 || getHandCardNum()<=0 ||JianYingUsed)
        return;
    engine->addActionNum(ATTACK);
    JianYingUsed=true;
}
void JianSheng::JianYing2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=102)
        return;
    if(crystal>0)
        crystal--;
    else
        gem--;
    coder.notice("剑圣发动【剑影】");
    coder.energyNotice(id,gem,crystal);
}

void JianSheng::skillReset(QList<void*>args){
    if(this != ((PlayerEntity*)args[0]))
        return;
    JianYingUsed=false;
    LianXuJiUsed=false;
    attackCount=0;
}
//圣剑
void JianSheng::ShengJian(QList<void *> args){
    if(this != ((PlayerEntity*)args[0])||!*(bool*)args[4])
        return;
    attackCount++;
    if(attackCount==3){
        coder.notice("剑圣发动【圣剑】");
        *((int*)args[5]) = NOMISS;
    }
}

void JianSheng::makeConnection(BackgroundEngine *engine)
{    
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(LieFengJi1(QList<void*>)));
    connect(engine,SIGNAL(shieldSIG(QList<void*>)),this,SLOT(LieFengJi2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(JiFengJi1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JiFengJi2(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(LianXuJi1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(LianXuJi2(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(JianYing1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JianYing2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(ShengJian(QList<void*>)));
}

FengYin::FengYin(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 4;
    this->star = 3;
    this->makeConnection(engine);
    shuFuID=-1;
}
//法术激荡
void FengYin::FaShuJiDang1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(getHandCardNum()<=0)
        return;
    engine->addActionNum(ATTACK);
}
void FengYin::FaShuJiDang2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=404)
        return;
    coder.notice("封印师发动【法术激荡】");
}

//封印法术
void FengYin::FengYinFaShu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 401)
        return;
    coder.notice("封印师发动【封印法术】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}
void FengYin::FengYinFaShu2(QList<CardEntity*> cards,PlayerEntity* user)
{
    foreach(CardEntity*fengYin, user->getBasicEffect())
        if(fengYin->getType()=="attack" && fengYin->getProperty()==tr("幻"))
            foreach(CardEntity*ptr, cards)
                if(ptr->getElement()==fengYin->getElement())
                {
                    coder.notice("玩家"+QString::number(user->getID())+"触发"+fengYin->getSpecialityList().at(0));
                    user->removeBasicEffect(fengYin);
                    Harm harm;
                    harm.harmPoint = 3;
                    harm.type = MAGIC;
                    engine->timeLine3(harm,this,user,fengYin->getSpecialityList().at(0));
                    break;
                }
}

//五系束缚
void FengYin::WuXiShuFu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 402)
        return;
    if(magic->infor2)
        gem--;
    else
        crystal--;
    shuFuID=magic->dstID;
    coder.notice("封印师对玩家"+QString::number(shuFuID)+"发动【五系束缚】");
    coder.energyNotice(id,gem,crystal);
    coder.specialNotice(shuFuID,0,1);
}

void FengYin::WuXiShuFu2(PlayerEntity* player, bool* withoutTrap)
{
    if(player->getID()!=shuFuID)
        return;
    bool weak=false;
    *withoutTrap=true;
    int howMany=2;
    PlayerEntity* ptr=player;
    for(int i=0;i<engine->getPlayerNum();i++)
    { 
        foreach(CardEntity*card,ptr->getBasicEffect())
            if(card->getType()=="attack"&& card->getProperty()==tr("幻"))
                howMany++;
        ptr=ptr->getNext();
    }
    if(howMany>4)
        howMany=4;
    for(int i = 0;i < player->getBasicEffect().size();i++)
        if(player->getBasicEffect().at(i)->getMagicName() == WEAKCARD)
        {
            engine->weakProcess(player,3+howMany);
            player->removeBasicEffect(player->getBasicEffect()[i]);
            weak=true;
            break;
        }
    if(!weak)
        engine->weakProcess(player,howMany);
    coder.specialNotice(shuFuID,0,0);
    shuFuID=-1;
}

//封印破碎
void FengYin::FengYinPoSui(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 403)
        return;
    coder.notice("封印师发动【封印破碎】");
    if(magic->infor2)
        gem--;
    else
        crystal--;
    coder.energyNotice(id,gem,crystal);
    int dstID=magic->dstID;
    PlayerEntity* ptr=engine->getPlayerByID(dstID);
    CardEntity*card=getCardByID(magic->CardID);
    QList<CardEntity*>cards;
    cards.append(card);
    if(ptr->removeBasicEffect(card,id,HAND))
    {
        coder.notice("封印师收走玩家"+QString::number(dstID)+"面前的"+card->getName());
        coder.getCardNotice(1,cards,id,true);
        addHandCards(cards,0,false);
    }
}

void FengYin::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(FengYinFaShu1(QList<void*>)));
    connect(engine,SIGNAL(showHandCards(QList<CardEntity*>,PlayerEntity*)),this,SLOT(FengYinFaShu2(QList<CardEntity*>,PlayerEntity*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(WuXiShuFu1(QList<void*>)));
    connect(engine,SIGNAL(weakSIG(PlayerEntity*,bool*)),this,SLOT(WuXiShuFu2(PlayerEntity*,bool*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(FengYinPoSui(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(FaShuJiDang1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(FaShuJiDang2(QList<void*>)));
}

MoJian::MoJian(BackgroundEngine *engine,int id,int color):PlayerEntity(engine,id,color)
{
    this->characterID = 9;
    this->star = 3.5;
    this->makeConnection(engine);
}

//黑暗震颤
void MoJian::HeiAnZhenChan1(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0]||HeiAnZhenChanUsed||gem<=0||!*(bool*)args[4])
        return;
    coder.askForSkill(id,"黑暗震颤");
    isHeiAnZhenChan=false;
    if(messageBuffer::readInfor() == 0)
        return;
    *((int*)args[5]) = NOREPLY;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice("魔剑发动【黑暗震颤】");
    HeiAnZhenChanUsed=true;
    isHeiAnZhenChan=true;
}

void MoJian::HeiAnZhenChan2(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0]||!isHeiAnZhenChan)
        return;
    isHeiAnZhenChan=false;
    engine->drawCards(getHandCardMax()-getHandCardNum(),0,this);
}

void MoJian::HeiAnZhenChan3(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||!isHeiAnZhenChan)
        return;
    isHeiAnZhenChan=false;
}

//修罗连斩
void MoJian::XiuLuoLianZhan1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(getHandCardNum()<=0 ||XiuLuoLianZhanUsed)
        return;
    engine->addActionNum(ATTACK);
    XiuLuoLianZhanUsed=true;
}

void MoJian::XiuLuoLianZhan2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=901)
        return;
    coder.notice("魔剑发动【修罗连斩】");
}

void MoJian::skillReset(QList<void*>args){
    if(this != ((PlayerEntity*)args[0]))
        return;
    XiuLuoLianZhanUsed=false;
    HeiAnZhenChanUsed=false;
    isHeiAnZhenChan=false;
}

//暗影凝聚
void MoJian::AnYingNingJu(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap==1)
    {
        setTap(0);
        coder.tapNotice(id,0,"【普通形态】");
    }
    coder.askForSkill(this->getID(),"暗影凝聚");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    coder.notice("魔剑发动【暗影凝聚】");
    Harm anying;
    anying.harmPoint=1;
    anying.type=MAGICHARM;
    engine->timeLine3(anying,this,this,"暗影凝聚");
    setTap(1);
    coder.tapNotice(id,1,"【暗影形态】");
}

//暗影之力
void MoJian::AnYingZhiLi(QList<void*> args)
{
    if(this != ((PlayerEntity*)args[0])||!tap)
        return;

    Harm *harm = (Harm*)args[2];
    if(harm->type != ATTACK)
        return;
    harm->harmPoint++;
    coder.notice("魔剑发动【暗影之力】");
}

//暗影流星
void MoJian::AnYingLiuXing(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 902)
        return;
    coder.notice("魔剑发动【暗影流星】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    coder.discardNotice(id,2,"y",cards);
    this->removeHandCards(cards,true);

    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"暗影流星");
}

void MoJian::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(AnYingLiuXing(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(XiuLuoLianZhan1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(XiuLuoLianZhan2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(HeiAnZhenChan1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(HeiAnZhenChan2(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(HeiAnZhenChan3(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(AnYingNingJu(QList<void*>)));
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(AnYingZhiLi(QList<void*>)));
}

MaoXian::MaoXian(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 12;
    this->star = 3.5;
    this->makeConnection(engine);
}
void MaoXian::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillAttack(QList<void*>)),this,SLOT(QiZha(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TouTianHuanRi(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TeShuJiaGong(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(TouTianHuanRi2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(TeShuJiaGong2(QList<void*>)));
    connect(engine,SIGNAL(skillSpecial(QList<void*>)),this,SLOT(MaoXianZheTianTang(QList<void*>)));
}

void MaoXian::QiZha(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=1201)
        return;
    coder.notice("冒险家发动【欺诈】");
    setCrystal(crystal+1);
    coder.energyNotice(id,gem,crystal);
    QList<CardEntity*>cards;
    cards.append(getCardByID(skill->infor3));
    cards.append(getCardByID(skill->infor4));
    if(skill->infor2==3)
        cards.append(getCardByID(skill->infor5));
    *(int*)args[1]=0;
    coder.discardNotice(id,skill->infor2,"y",cards);
    this->removeHandCards(cards,true);
}

//偷天换日
void MaoXian::TouTianHuanRi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id)
        return;
    if(magic->infor1 != 1203)
        return;
    if(magic->infor2)
        gem--;
    else
        crystal--;
    coder.notice("冒险家发动【偷天换日】");
    coder.energyNotice(id,gem,crystal);
    teamArea.setGem(color,teamArea.getGem(color)+1);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    int other=1-color;
    teamArea.setGem(other,teamArea.getGem(other)-1);
    coder.stoneNotice(other,teamArea.getGem(other),teamArea.getCrystal(other));
    engine->addActionNum(ATTACKORMAGIC);
}
void MaoXian::TouTianHuanRi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1203)
        return;
    coder.notice("冒险家使用【偷天换日】的额外攻击行动");
}

//特殊加工
void MaoXian::TeShuJiaGong(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id)
        return;
    if(magic->infor1 != 1202)
        return;
    if(magic->infor2)
        gem--;
    else
        crystal--;
    coder.notice("冒险家发动【特殊加工】");
    coder.energyNotice(id,gem,crystal);
    int toChange=teamArea.getCrystal(color);
    teamArea.setCrystal(color,0);
    teamArea.setGem(color,toChange+teamArea.getGem(color));
    coder.stoneNotice(color,teamArea.getGem(color),0);
    engine->addActionNum(ATTACKORMAGIC);
}
void MaoXian::TeShuJiaGong2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1203)
        return;
    coder.notice("冒险家使用【偷天换日】的额外攻击行动");
}
//冒险者天堂
void MaoXian::MaoXianZheTianTang(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id)
        return;
    if(magic->infor1 != 1204)
        return;
    coder.notice("冒险家对玩家"+QString::number(magic->dstID)+"发动【冒险者天堂】");
    PlayerEntity*dst=engine->getPlayerByID(magic->dstID);
    dst->setGem(dst->getGem()+magic->infor2);
    dst->setCrystal(dst->getCrystal()+magic->infor3);
    teamArea.setGem(color,teamArea.getGem(color)-magic->infor2);
    teamArea.setCrystal(color,teamArea.getCrystal(color)-magic->infor3);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    coder.energyNotice(magic->dstID,dst->getGem(),dst->getCrystal());
}

YuanSu::YuanSu(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 11;
    this->star = 3.5;
    tokenMax[0]=3;
    ignite=false;
    this->makeConnection(engine);
}
void YuanSu::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(YuanSuFaShu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(YuanSuDianRan(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(YuanSuFaShu2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(YuanSuDianRan2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(YueGuang(QList<void*>)));
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(YuanSuXiShou(QList<void*>)));
}

//元素吸收
void YuanSu::YuanSuXiShou(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0] || token[0]==3)
        return;
    if(ignite){
        ignite=false;
        return;
    }
    Harm* h=(Harm*)args[2];
    if(h->type==MAGICHARM)
    {
        setToken(0,token[0]+1);
        coder.notice("元素师发动【元素吸收】");
        coder.tokenNotice(id,0,token[0]);
    }
}

//元素法术
void YuanSu::YuanSuFaShu(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1101)
        return;
    int harmPoint=magic->infor3;
    QString skill;
    switch(magic->infor2)
    {
    case 1:
        skill="风刃";
        break;
    case 2:
        skill="冰冻";
        break;
    case 3:
        skill="火球";
        harmPoint++;
        break;
    case 4:
        skill="陨石";
        break;
    case 5:
        skill="雷击";
        break;
    }
    coder.notice("元素师对玩家"+QString::number(magic->dstID)+"发动【"+skill+"】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    if(magic->infor3==2)
        cards << getCardByID(magic->infor4);
    coder.discardNotice(id,magic->infor3,"y",cards);
    this->removeHandCards(cards,true);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = harmPoint;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,skill);
    switch(magic->infor2)
    {
    case 1:
        engine->addActionNum(ATTACK);
        break;
    case 2:
        dst=engine->getPlayerByID(magic->infor5);
        dst->setCrossNum(dst->getCrossNum()+1);
        coder.notice("元素的【冰冻】为玩家"+QString::number(magic->infor5)+"增加1治疗");
        coder.crossChangeNotice(magic->infor5,dst->getCrossNum());
        break;
    case 4:
        engine->addActionNum(MAGIC);
        break;
    case 5:
        teamArea.setGem(color,teamArea.getGem(color)+1);
        coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
        break;
    }
}
void YuanSu::YuanSuFaShu2(QList<void*> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id)
        return;
    if(skill->infor1==1104)
        coder.notice("元素师使用【风刃】的额外攻击行动");
    if(skill->infor1==1105)
        coder.notice("元素师使用【陨石】的额外法术行动");
}

//元素点燃
void YuanSu::YuanSuDianRan(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id||magic->infor1 != 1102)
        return;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    coder.notice("元素师对玩家"+QString::number(magic->dstID)+"发动【元素点燃】");
    setToken(0,0);
    ignite=true;
    coder.tokenNotice(id,0,0);
    engine->timeLine3(harm,this,dst,"元素点燃");
    engine->addActionNum(MAGIC);
}
void YuanSu::YuanSuDianRan2(QList<void*> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1106)
        return;
    coder.notice("元素师使用【元素点燃】的额外法术行动");
}

//月光
void YuanSu::YueGuang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id||magic->infor1 != 1103)
        return;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    coder.notice("元素师对玩家"+QString::number(magic->dstID)+"发动【月光】");
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    Harm harm;
    harm.harmPoint = gem+crystal+1;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,dst,"月光");
}

ZhongCai::ZhongCai(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=14;
    this->star=3.5;
    tokenMax[0]=4;
    crystal=2;
    startUsed=false;
    this->makeConnection(engine);
}

void ZhongCai::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(YiShiZhongDuan(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(ZhongCaiYiShi1(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(ZhongCaiYiShi2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoRiShenPan(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(PanJueTianPing(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(ShenPanLangChao(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

//仪式中断
void ZhongCai::YiShiZhongDuan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap!=1 || startUsed)
        return;
    coder.askForSkill(this->getID(),"仪式中断");
    if(messageBuffer::readInfor() == 0)
        return;
    coder.notice("仲裁者发动【仪式中断】");
    setTap(0);
    coder.tapNotice(this->getID(),0,"【普通形态】");
    startUsed=true;
    setHandCardsMaxFixed(false);
    coder.handcardMaxNotice(this->getID(),handCardsMax);
    teamArea.setGem(color,teamArea.getGem(color)+1);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
}

//仲裁仪式发动
void ZhongCai::ZhongCaiYiShi1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap!=0 || this->getGem()==0 || startUsed)
        return;
    coder.askForSkill(this->getID(),"仲裁仪式");
    if(messageBuffer::readInfor() == 0)
        return;
    coder.notice("仲裁者发动【仲裁仪式】");
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    setTap(1);
    coder.tapNotice(this->getID(),1,"【审判形态】");
    startUsed=true;
    setHandCardsMaxFixed(true, 5);
    coder.handcardMaxNotice(this->getID(),handCardsMax);
}

//仲裁仪式触发
void ZhongCai::ZhongCaiYiShi2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap!=1)
        return;
    if(token[0]==4)
        return;
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
}

//末日审判
void ZhongCai::MoRiShenPan(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1403)
        return;
    QString msg;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    msg="仲裁者对玩家"+QString::number(magic->dstID)+"发动【末日审判】";
    coder.notice(msg);
    int tokenNum=token[0];
    setToken(0,0);
    coder.tokenNotice(id,0,0);
    Harm harm;
    harm.harmPoint=tokenNum;
    harm.type=MAGICHARM;
    engine->timeLine3(harm,this,dst,"末日审判");
}

//判决天平
void ZhongCai::PanJueTianPing(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1404)
        return;
    if(crystal>0)
        crystal--;
    else
        gem--;
    setGem(gem);
    setCrystal(crystal);
    coder.energyNotice(id,gem,crystal);
    coder.notice("仲裁者发动【判决天平】");
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
    if(magic->infor2)
    {
        int drawNum=this->getHandCardMax()-this->getHandCardNum();
        engine->drawCards(drawNum,0,this);
        teamArea.setGem(color,teamArea.getGem(color)+1);
        coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
    }
    else
    {
        coder.discardNotice(this->getID(),this->getHandCards().length(),"n",this->getHandCards());
        this->removeHandCards(this->getHandCards(),false,true);
    }
}

//审判浪潮
void ZhongCai::ShenPanLangChao(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1])
        return;
    coder.notice(tr("仲裁者发动【审判浪潮】"));
    setToken(0,token[0]+1);
    coder.tokenNotice(this->getID(),0,token[0]);
}

//重置启动
void ZhongCai::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    startUsed=false;
}

YongZhe::YongZhe(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=21;
    this->star=4.5;
    tokenMax[0]=4;
    tokenMax[1]=4;
    fireNum=0;
    tiaoXinID=-1;
    crystal=2;
    nuHouUsed=false;
    tiaoXinUsed=false;
    tiaoXinChuFa=false;
    jinDuanUsed=false;
    this->makeConnection(engine);
}

void YongZhe::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(NuHou(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(NuHou1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(NuHou2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(MingJingZhiShui(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(JinDuanZhiLi1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(JinDuanZhiLi2(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(SiDou(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TiaoXin1(QList<void*>)));
    connect(engine,SIGNAL(tiaoXinPhaseSIG(PlayerEntity*,int*,bool*)),this,SLOT(TiaoXin2(PlayerEntity*,int*,bool*)));
    connect(engine,SIGNAL(turnEndPhaseSIG(PlayerEntity*)),this,SLOT(TiaoXin3(PlayerEntity*)));
    //connect(engine,SIGNAL(turnEndPhaseSIG(QList<void*>)),this,SLOT(TiaoXin4(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(JingPiLiJie1(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(JingPiLiJie2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JingPiLiJie3(QList<void*>)));
}

//怒吼询问
void YongZhe::NuHou(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || this->getToken(0)== 0)
        return;
    this->nuHouUsed=false;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(this->getID(),"怒吼");
    if(messageBuffer::readInfor() == 0)
        return;
    setToken(0,token[0]-1);
    coder.notice("勇者发动【怒吼】");
    this->nuHouUsed=true;
    coder.tokenNotice(id,0,token[0]);
}

//怒吼命中
void YongZhe::NuHou1(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!this->nuHouUsed)
        return;
    nuHouUsed=false;
    Harm* harm=(Harm*)args[2];
    harm->harmPoint+=2;
}

//怒吼未命中
void YongZhe::NuHou2(QList<void *> args)
{
    if(this!=(PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!this->nuHouUsed)
        return;
    nuHouUsed=false;
    if(this->getToken(1)!=4)
    {
        setToken(1,token[1]+1);
        coder.tokenNotice(id,1,token[1]);
    }
}

//明镜止水
void YongZhe::MingJingZhiShui(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || this->getToken(1)<4)
        return;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(this->getID(),"明镜止水");
    if(messageBuffer::readInfor() == 0)
        return;
    setToken(1,0);
    coder.notice("勇者发动【明镜止水】");
    coder.tokenNotice(id,1,0);
    *(int*)args[5] = NOREPLY;
}

//禁断命中
void YongZhe::JinDuanZhiLi1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || this->getEnergy()==0)
        return;
    if(!*(bool*)args[4])
        return;
    if(this->getHandCards().length()== 0)
        return;
    coder.askForSkill(this->getID(),"禁断之力");
    if(messageBuffer::readInfor() == 0)
        return;
    if(this->getCrystal()>0)
        setCrystal(crystal-1);
    else
        setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice("勇者发动【禁断之力】");
    jinDuanUsed=true;
    for(int i=0;i < this->getHandCards().length();i++)
    {
        if(this->getHandCards().at(i)->getType() != "attack")
        {
            if(this->getToken(0)!=4)
                setToken(0,token[0]+1);
        }
        if(this->getHandCards().at(i)->getElement() == "fire")
            fireNum++;
    }
    coder.tokenNotice(id,0,token[0]);
    coder.discardNotice(this->getID(),this->getHandCards().length(),tr("y"),this->getHandCards());
    this->removeHandCards(this->getHandCards(),true,true);
    Harm* harm=(Harm*)args[2];
    harm->harmPoint+=fireNum;
    Harm jinduanzhili;
    jinduanzhili.harmPoint=fireNum;
    jinduanzhili.type=MAGICHARM;
    if(fireNum != 0){
        engine->timeLine3(jinduanzhili,this,this,"禁断之力");
        if(engine->checkEnd())
            return;
    }
    setTap(1);
    fireNum=0;
}

//禁断未命中
void YongZhe::JinDuanZhiLi2(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || this->getEnergy()==0)
        return;
    if(!*(bool*)args[4])
        return;
    if(this->getHandCards().length()== 0)
        return;
    coder.askForSkill(this->getID(),"禁断之力");
    if(messageBuffer::readInfor() == 0)
        return;
    if(this->getCrystal()>0)
        setCrystal(crystal-1);
    else
        setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice("勇者发动【禁断之力】");
    jinDuanUsed=true;
    for(int i=0;i < this->getHandCards().length();i++)
    {
        if(this->getHandCards().at(i)->getType() != "attack")
        {
            if(this->getToken(0)!=4)
                setToken(0,token[0]+1);
        }
        if(this->getHandCards().at(i)->getElement() == "water")
        {
            if(this->getToken(1)!=4)
                setToken(1,token[1]+1);
        }
    }
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
    coder.discardNotice(this->getID(),this->getHandCards().length(),tr("y"),this->getHandCards());
    this->removeHandCards(this->getHandCards(),true,true);
    setTap(1);
}

//死斗
void YongZhe::SiDou(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type != MAGICHARM || this->getGem()==0)
        return;
    coder.askForSkill(this->getID(),"死斗");
    if(messageBuffer::readInfor() == 0)
        return;
    setGem(gem-1);
    coder.energyNotice(this->getID(),gem,crystal);
    coder.notice("勇者发动【死斗】");
    if(token[0]+3<=4)
        setToken(0,token[0]+3);
    else
        setToken(0,4);
    coder.tokenNotice(id,0,token[0]);
}

//挑衅放置
void YongZhe::TiaoXin1(QList<void *> args)
{
    BatInfor* magic= (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2101)
        return;
    if(tiaoXinUsed)
        return;
    setToken(0,token[0]-1);
    setToken(1,token[1]+1);
    tiaoXinID=magic->dstID;
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
    coder.notice("勇者对玩家"+QString::number(tiaoXinID)+"发动【挑衅】");
    coder.specialNotice(tiaoXinID,1,1);
    tiaoXinUsed=true;
}

//挑衅触发判定1
void YongZhe::TiaoXin2(PlayerEntity *player, int *act,bool* canGiveUp)
{
    if(player->getID() != tiaoXinID)
        return;
    if(player->getRoleID()==14 && player->getToken(0)==4)
        *act=-1;
    else
        *act=1;
    *canGiveUp=true;
    if(!tiaoXinUsed)
        return;
    tiaoXinChuFa=true;
}

//挑衅移除
void YongZhe::TiaoXin3(PlayerEntity *player)
{
    if(player->getID() != tiaoXinID)
        return;
    if(!tiaoXinUsed)
        return;
    if(!tiaoXinChuFa)
        return;
    coder.specialNotice(tiaoXinID,1,0);
    tiaoXinID=-1;
    tiaoXinUsed=false;
}

//精疲力竭触发
void YongZhe::JingPiLiJie1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(this->tap != 1)
        return;
    if(jinDuanUsed){
    coder.tapNotice(this->getID(),1,"【精疲力竭】");
    setHandCardsMaxFixed(true, 4);
    coder.handcardMaxNotice(id,handCardsMax);
    jinDuanUsed=false;
    engine->addActionNum(ATTACK);
    }
}

//精疲力竭结算
void YongZhe::JingPiLiJie2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(this->tap != 1)
        return;
    setTap(0);
    coder.tapNotice(this->getID(),0,"【普通形态】");
    setHandCardsMaxFixed(false);
    coder.handcardMaxNotice(id,handCardsMax);
    Harm jingpilijie;
    jingpilijie.harmPoint=3;
    jingpilijie.type=MAGICHARM;
    engine->timeLine3(jingpilijie,this,this,"精疲力竭");
}
void YongZhe::JingPiLiJie3(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=2103)
        return;
    coder.notice("勇者使用【精疲力竭】的额外攻击行动");
}

/******************
  圣枪 10
  *******************/
ShengQiang::ShengQiang(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 10;
    this->star = 3.5;
    //神圣信仰
    this->crossMax = 3;
    this->makeConnection(engine);
}

//辉耀
void ShengQiang::HuiYao(QList<void *> args)
{
     BatInfor *magic = (BatInfor*)args[0];
     if(magic->srcID != this->getID())
         return;
     if(magic->infor1 != 1001)
         return;

     QList<CardEntity*> cards;
     cards << getCardByID(magic->CardID);
     coder.notice("圣枪发动【辉耀】");
     this->removeHandCards(cards,true);
     coder.discardNotice(this->getID(),1,"y",cards);
     PlayerEntity* dst = this;
     do{
         dst->setCrossNum(dst->getCrossNum()+1);
         coder.crossChangeNotice(dst->getID(), dst->getCrossNum());
         dst = dst->getNext();
     }while(dst!= this);
     this->engine->addActionNum(ATTACK);
}
void ShengQiang::HuiYao2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1003)
        return;
    coder.notice("圣枪使用【辉耀】的额外攻击行动");
}

//惩戒
void ShengQiang::ChengJie(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1002)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.notice("圣枪对玩家"+QString::number(magic->dstID)+"发动【惩戒】");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), 1, "y", cards);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    dst->setCrossNum(dst->getCrossNum()-1);
    coder.crossChangeNotice(dst->getID(), dst->getCrossNum());
    coder.notice("玩家"+QString::number(magic->dstID)+"减少1点治疗，圣枪增加1治疗");
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    this->engine->addActionNum(ATTACK);
}
void ShengQiang::ChengJie2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1004)
        return;
    coder.notice("圣枪使用【惩戒】的额外攻击行动");
}
//圣击，只判断非主动攻击
void ShengQiang::ShengJi(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(*(bool*)args[4])
        return;
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    coder.notice("圣枪发动【圣击】，增加1治疗");
}

//天枪
void ShengQiang::TianQiang(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    if(this->ShengGuangQiYuUsed)
        return;
    int cross = this->getCrossNum();
    if(cross<2)
        return;
    coder.askForSkill(this->getID(),"天枪");
    if(messageBuffer::readInfor() == 0)
        return;
    TianQiangUsed = true;
    this->setCrossNum(getCrossNum()-2);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    coder.notice("圣枪发动【天枪】");
    *(int*)args[5] = NOREPLY;
}

//地枪，不发动且未发动天枪则发动圣击
void ShengQiang::DiQiang(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    int cross = this->getCrossNum();
    if(cross > 0)
    {
        coder.askForSkill(this->getID(),"地枪");
        int reply = messageBuffer::readInfor();
        if(reply != 0)
        {
            this->setCrossNum(getCrossNum()-reply);
            coder.crossChangeNotice(this->getID(), getCrossNum());
            coder.notice("圣枪发动【地枪】，使用"+QString::number(reply)+"点治疗");
            Harm *harm = (Harm*)args[2];
            harm->harmPoint += reply;
        }
        else
            if(!TianQiangUsed)
        {
            this->setCrossNum(getCrossNum()+1);
            coder.crossChangeNotice(this->getID(), getCrossNum());
            coder.notice("圣枪发动【圣击】，增加1治疗");
        }
    }
    else
    {
        if(TianQiangUsed)
            return;
        this->setCrossNum(getCrossNum()+1);
        coder.crossChangeNotice(this->getID(), getCrossNum());
        coder.notice("圣枪发动【圣击】，增加1治疗");
    }

}

//圣光祈愈
void ShengQiang::ShengGuangQiYu(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1006)
        return;
    if(getGem()==0)
        return;
    ShengGuangQiYuUsed = true;
    coder.notice("圣枪发动【圣光祈愈】，增加2治疗");
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    this->setCrossNum(getCrossNum()+2,5);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    this->engine->addActionNum(ATTACK);
}
void ShengQiang::ShengGuangQiYu2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->srcID!=id || skill->infor1!=1007)
        return;
    coder.notice("圣枪使用【圣光祈愈】的额外攻击行动");
}
void ShengQiang::skillReset(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    ShengGuangQiYuUsed = false;
    TianQiangUsed = false;
}

void ShengQiang::makeConnection(BackgroundEngine* engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(HuiYao(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ChengJie(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(HuiYao2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(ChengJie2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShengGuangQiYu(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(ShengGuangQiYu2(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(TianQiang(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(ShengJi(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(DiQiang(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

QiDao::QiDao(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 16;
    this->star = 4;
    this->makeConnection(engine);
    tokenMax[0]=3;
}
//法力潮汐
void QiDao::FaLiChaoXi1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]) || FaLiChaoXiUsed || getEnergy()==0)
        return;
    if(getHandCardNum()<=0 && getToken(0)==0)
        return;
    FaLiChaoXiUsed=true;
    engine->addActionNum(MAGIC);
}
void QiDao::FaLiChaoXi2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=1606)
        return;
    if(crystal>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(id,gem,crystal);
    coder.notice("祈祷师发动【法力潮汐】");
}

//威力赐福
void QiDao::WeiLiCiFu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1602)
        return;
    coder.notice("祈祷师发动【威力赐福】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}
void QiDao::WeiLiCiFu2(QList<void*> args)
{
    PlayerEntity*user=(PlayerEntity*)args[0];
    foreach(CardEntity*weili, user->getBasicEffect())
        if(weili->getSpecialityList().contains(tr("威力赐福")))
        {
            //此处询问是否使用
            coder.askForSkill(user->getID(),"威力赐福");
            if(messageBuffer::readInfor() == 0)
                return;
            user->removeBasicEffect(weili);
            coder.notice("玩家"+QString::number(user->getID())+"发动【威力赐福】");
            Harm *harm = (Harm*)args[2];
            harm->harmPoint += 2;
            break;
        }
}

//迅捷赐福
void QiDao::XunJieCiFu1(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1603)
        return;
    coder.notice("祈祷师发动【迅捷赐福】");
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    engine->useCard(cards,this,dst,true);
}
void QiDao::XunJieCiFu2(QList<void*> args)
{
    PlayerEntity*user=(PlayerEntity*)args[0];
    foreach(CardEntity*xunjie, user->getBasicEffect())
        if(xunjie->getSpecialityList().contains(tr("迅捷赐福"))){
            engine->addActionNum(ATTACK);
            break;
        }
}
void QiDao::XunJieCiFu3(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(skill->infor1!=1607)
        return;
    PlayerEntity*user=engine->getPlayerByID(skill->srcID);
    foreach(CardEntity*xunjie, user->getBasicEffect())
        if(xunjie->getSpecialityList().contains(tr("迅捷赐福"))){
            user->removeBasicEffect(xunjie);
            break;
        }
    coder.notice("玩家"+QString::number(skill->srcID)+"发动【迅捷赐福】");
}

//光辉信仰
void QiDao::GuangHuiXinYang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1604)
        return;
    coder.notice("祈祷师对玩家"+QString::number(magic->dstID)+"发动【光辉信仰】");
    token[0]--;
    coder.tokenNotice(id,0,token[0]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    coder.discardNotice(id,cards.size(),"n",cards);
    this->removeHandCards(cards,false);
    dst->setCrossNum(dst->getCrossNum()+1);
    coder.crossChangeNotice(magic->dstID,dst->getCrossNum());
    teamArea.setGem(color,teamArea.getGem(color)+1);
    coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
}

//漆黑信仰
void QiDao::QiHeiXinYang(QList<void*> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1605)
        return;
    coder.notice("祈祷师对玩家"+QString::number(magic->dstID)+"发动【漆黑信仰】");
    token[0]--;
    coder.tokenNotice(id,0,token[0]);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"漆黑信仰");
    if(engine->checkEnd())
        return;
    this->engine->timeLine3(harm,this,this,"漆黑信仰");
}

void QiDao::Pray1(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(tap == 1)
        return;
    if(getGem()==0)
        return;
    coder.askForSkill(this->getID(),"祈祷");
    int reply=messageBuffer::readInfor();
    if(reply==0)
        return;
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("祈祷师发动【祈祷】");
    setTap(1);
    coder.tapNotice(id,1,"【祈祷形态】");
}
void QiDao::Pray2(QList<void*> args)
{
    if(this != (PlayerEntity*)args[0]||!*(bool*)args[4]||!tap)
        return;
    setToken(0,token[0]+2);
    coder.tokenNotice(id,0,token[0]);
}
void QiDao::skillReset(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    FaLiChaoXiUsed = false;
}

void QiDao::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(WeiLiCiFu1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(WeiLiCiFu2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XunJieCiFu1(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(XunJieCiFu2(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(XunJieCiFu2(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(XunJieCiFu3(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(FaLiChaoXi1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(FaLiChaoXi2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(GuangHuiXinYang(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(QiHeiXinYang(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(Pray1(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(Pray2(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}


/******************
  神官 15
  *******************/
ShenGuan::ShenGuan(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 15;
    this->star = 4;
    //神圣信仰
    this->crossMax = 6;
    this->makeConnection(engine);
}

//神圣启示
void ShenGuan::ShenShengQiShi(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    coder.askForSkill(this->getID(), "神圣启示");
    if(messageBuffer::readInfor() == 0)
        return;

    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(id, getCrossNum());

    coder.notice("神官发动【神圣启示】，增加1治疗");
}
//神圣祈福
void ShenGuan::ShenShengQiFu(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID()||magic->infor1 != 1502)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    coder.notice("神官发动【神圣祈福】，增加2治疗");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), 2, "y", cards);
    this->setCrossNum(getCrossNum()+2);
    coder.crossChangeNotice(this->getID(), getCrossNum());
}
void ShenGuan::ShengShiShouHu(Harm harm, PlayerEntity *src, PlayerEntity *dst, int *crossAvailable)
{
    if(dst!=this)
        return;
    if(crossNum>1)
        *crossAvailable = 1;
}

//水之神力
void ShenGuan::ShuiZhiShenLi(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID()||magic->infor1 != 1503)
        return;
    int dst = magic->dstID;
    coder.notice("神官对玩家"+QString::number(dst)+"发动水之神力");

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);

    PlayerEntity* ptr = engine->getPlayerByID(dst);
    if(this->getHandCardNum()!=0)
    {
        coder.askForSkill(this->getID(), "水之神力给牌");
        cards.clear();
        cards << messageBuffer::readCardID(1);
        this->giveHandCards(cards, ptr);
    }
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    ptr->setCrossNum(ptr->getCrossNum()+1);
    coder.crossChangeNotice(dst, ptr->getCrossNum());
    coder.notice("神官和目标各增加1治疗");
}

//神圣契约
void ShenGuan::ShenShengQiYue(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||this->getCrossNum()==0||this->getEnergy()==0)
        return;
    coder.askForSkill(this->getID(),"神圣契约");
    BatInfor start = messageBuffer::readBatInfor();
    if(start.infor2==0)
        return;
    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("神官对玩家"+QString::number(start.dstID)+"发动神圣契约，转移"+QString::number(start.infor3)+"点治疗");
    this->setCrossNum(getCrossNum()-start.infor3);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    PlayerEntity* dst = engine->getPlayerByID(start.dstID);
    dst->setCrossNum(dst->getCrossNum()+start.infor3, 4);
    coder.crossChangeNotice(start.dstID, dst->getCrossNum());
}

//神圣领域
void ShenGuan::ShenShengLingYu(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID() || magic->infor1!=1505||this->getEnergy()==0)
        return;
    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("神官对玩家"+QString::number(magic->dstID)+"发动神圣领域");
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    QList<CardEntity*> cards;
    int n = 0;
    if(magic->CardID!=-1)
    {
        cards << getCardByID(magic->CardID);
        n++;
    }
    if(magic->infor3!=-1)
    {
        cards << getCardByID(magic->infor3);
        n++;
    }
    if(n>0)
    {
        this->removeHandCards(cards,true);
        coder.discardNotice(this->getID(), n, "n", cards);
    }
    if(magic->infor2 == 1)
    {
        this->setCrossNum(getCrossNum()-1);
        coder.crossChangeNotice(this->getID(), getCrossNum());
        coder.notice("神官移除1点治疗");
        Harm harm;
        harm.harmPoint = 2;
        harm.type = MAGIC;
        this->engine->timeLine3(harm,this,dst,"神圣领域");
    }
    else
    {
        coder.notice("神官增加2点治疗，玩家"+QString::number(magic->dstID)+"增加1点治疗");
        this->setCrossNum(getCrossNum()+2);
        coder.crossChangeNotice(this->getID(), getCrossNum());
        dst->setCrossNum(dst->getCrossNum()+1);
        coder.crossChangeNotice(dst->getID(), dst->getCrossNum());
    }
}

void ShenGuan::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(specialFinishSIG(QList<void*>)),this,SLOT(ShenShengQiShi(QList<void*>)));
    connect(engine,SIGNAL(askForHeal(Harm,PlayerEntity*,PlayerEntity*,int*)),this,SLOT(ShengShiShouHu(Harm,PlayerEntity*,PlayerEntity*,int*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShenShengQiFu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShuiZhiShenLi(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(ShenShengQiYue(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShenShengLingYu(QList<void*>)));
}


/*
  死灵 13
  */
SiLing::SiLing(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 13;
    this->star = 3.5;
    //圣渎
    this->crossMax = 5;
    this->makeConnection(engine);
}
//不朽
void SiLing::BuXiu(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(SiWangZhiChuUsed)
        return;
    coder.askForSkill(this->getID(), "不朽");
    if(messageBuffer::readInfor() == 0)
        return;

    this->setCrossNum(crossNum+1);
    coder.crossChangeNotice(this->getID(), crossNum);
    coder.notice("死灵发动【不朽】，增加1治疗");
}

void SiLing::ShengDu(Harm harm, PlayerEntity *src, PlayerEntity *dst, int *crossAvailable)
{
    if(dst!=this)
        return;
    if(harm.type == ATTACK)
        *crossAvailable=0;
}

//瘟疫
void SiLing::WenYi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1302)
        return;

    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.notice("死灵法师发动【瘟疫】");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(),1,"y",cards);
    PlayerEntity* dst = this->getNext();
    do{
        Harm harm;
        harm.harmPoint = 1;
        harm.type = MAGIC;
        this->engine->timeLine3(harm,this,dst,"瘟疫");
        if(engine->checkEnd())
            break;
        dst = dst->getNext();
    }while(dst!= this);
}
//死亡之触
void SiLing::SiWangZhiChu(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 1303)
        return;
    SiWangZhiChuUsed = true;
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*> cards;
    for(int i=0;i<magic->infor3;i++)
        cards << getCardByID(cardNum[i].toInt());
    coder.notice("死灵法师对玩家"+QString::number(magic->dstID)+"发动【死亡之触】，移除"+
                 QString::number(magic->infor2)+"点治疗，弃"+QString::number(magic->infor3)+"张同系牌");
    this->setCrossNum(getCrossNum()-magic->infor2);
    coder.crossChangeNotice(magic->srcID, getCrossNum());
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), magic->infor3, "y", cards);
    Harm harm;
    harm.harmPoint = magic->infor2+magic->infor3-3;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"死亡之触");
}

//墓碑陨落
void SiLing::MuBeiYunLuo(QList<void *> args)
{
    BatInfor* magic =(BatInfor*)args[0];
    if(magic->srcID != this->getID()||magic->infor1 != 1304||getGem()==0)
        return;
    coder.notice("死灵法师发动【墓碑陨落】");
    this->gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    PlayerEntity* dst = this->getNext();
    do{
        Harm harm;
        harm.harmPoint = 2;
        harm.type = MAGIC;
        this->engine->timeLine3(harm,this,dst,"墓碑陨落");
        if(engine->checkEnd())
            break;
        dst = dst->getNext();
    }while(dst!= this);
    this->setCrossNum(getCrossNum()+1);
    coder.crossChangeNotice(this->getID(), getCrossNum());
    coder.notice("死灵法师发动【墓碑陨落】，增加1治疗");
}

void SiLing::skillReset(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    SiWangZhiChuUsed = false;
}

void SiLing::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(BuXiu(QList<void*>)));
    connect(engine,SIGNAL(askForHeal(Harm,PlayerEntity*,PlayerEntity*,int*)),this,SLOT(ShengDu(Harm,PlayerEntity*,PlayerEntity*,int*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(WenYi(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(SiWangZhiChu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MuBeiYunLuo(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}
XianZhe::XianZhe(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 17;
    this->star = 4;
    this->makeConnection(engine);
    energyMax=4;
}

void XianZhe::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(ZhiHuiFaDian(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(FaShuFanTan(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(MoDaoFaDian(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(ShengJieFaDian(QList<void*>)));
}
void XianZhe::ZhiHuiFaDian(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type != MAGICHARM || ((Harm*)args[2])->harmPoint<=3)
        return;
    setGem(gem+2);
    coder.energyNotice(id,gem,crystal);
}

void XianZhe::FaShuFanTan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->type != MAGICHARM || ((Harm*)args[2])->harmPoint!=1)
        return;
    if(this->getHandCards().size()<2)
        return;
    coder.askForSkill(id,"法术反弹");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    if(arg[3].toInt() != id || arg[0].toInt() != 1701)
        return;
    QList<CardEntity*>cards;
    for(int i=0;i<arg[4].toInt();i++)
        cards.append(getCardByID(arg[i+5].toInt()));
    coder.discardNotice(id,arg[4].toInt(),"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("贤者对玩家"+QString::number(arg[2].toInt())+"发动【法术反弹】");
    PlayerEntity* dst = engine->getPlayerByID(arg[2].toInt());
    Harm harm;
    harm.harmPoint = (arg[4].toInt())-1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"法术反弹");
    if(engine->checkEnd())
        return;
    Harm harm1;
    harm1.harmPoint = arg[4].toInt();
    harm1.type = MAGIC;
    this->engine->timeLine3(harm1,this,this,"法术反弹");
}

void XianZhe::MoDaoFaDian(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1702)
        return;
    if(this->getGem()==0)
        return;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*>cards;
    for(int i=0;i<magic->infor2;i++)
        cards.append(getCardByID(cardNum[i].toInt()));
    coder.discardNotice(id,magic->infor2,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("贤者对玩家"+QString::number(magic->dstID)+"发动【魔道法典】");
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = (magic->infor2)-1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,dst,"魔道法典");
    if(engine->checkEnd())
        return;
    this->engine->timeLine3(harm,this,this,"魔道法典");
}

void XianZhe::ShengJieFaDian(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 1703)
        return;
    if(this->getGem()==0)
        return;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    QStringList cardNum = magic->inforstr.split(":");
    QStringList playerNum = magic->inforstrp.split(":");
    QList<CardEntity*>cards;
    QList<PlayerEntity*>players;
    int i=0;
    int j=0;
    for(i;i<magic->infor2;i++)
        cards.append(getCardByID(cardNum[i].toInt()));
    for(j;j<magic->infor3;j++)
        players.append(engine->getPlayerByID(playerNum[j].toInt()));
    coder.discardNotice(id,magic->infor2,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("贤者发动【圣洁法典】");
    foreach(PlayerEntity*ptr,players)
    {
        ptr->setCrossNum(ptr->getCrossNum()+2);
        coder.crossChangeNotice(ptr->getID(),ptr->getCrossNum());
        coder.notice("贤者使用【圣洁法典】为玩家"+TOQSTR(ptr->getID())+"增加2点治疗");
    }
    Harm harm;
    harm.harmPoint = (magic->infor2)-1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,this,"圣洁法典");
}


GeDouJia::GeDouJia(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=20;
    this->star=4.5;
    tokenMax[0]=6;
    BaiShiTargetID=-1;
    BaiShiUsed=false;
    BaiShiFirst=false;
    StartUsed=false;
    XuLiUesd=false;
    CangYanUsed=false;
    this->makeConnection(engine);
}

void GeDouJia::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine3SIG(QList<void*>)),this,SLOT(NianQiLiChang(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(CangYanXuLi(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(XuLiYiJi2(QList<void*>)));
    connect(engine,SIGNAL(timeLine2missedSIG(QList<void*>)),this,SLOT(XuLiYiJi3(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(NianDan(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(BaiShiHuanLingQuan1(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(BaiShiHuanLingQuan2(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(BaiShiHuanLingQuan3(QList<void*>)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(CangYanZhiHun2(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(DouShenTianQu(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(BaiShiTargetSet(QList<void*>)));
    connect(engine,SIGNAL(timeLine1ProSIG(QList<void*>)),this,SLOT(BaiShiQuXiao1(QList<void*>)));
    connect(engine,SIGNAL(beforeMagicSIG(QList<void*>)),this,SLOT(BaiShiQuXiao2(QList<void*>)));
    connect(engine,SIGNAL(specialFinishSIG(QList<void*>)),this,SLOT(BaiShiQuXiao3(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

void GeDouJia::NianQiLiChang(QList<void *> args)
{
    if(this != (PlayerEntity*)args[1] || ((Harm*)args[2])->harmPoint<5)
        return;
    Harm* harm= (Harm*)args[2];
    harm->harmPoint=4;
    coder.notice(tr("格斗家发动【念气力场】"));
}

void GeDouJia::CangYanXuLi(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(id,"苍炎蓄力");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt()==0)
        return;
    if(arg[2].toInt()==0){
    if(BaiShiUsed)
    {
        BaiShiUsed=false;
        setTap(0);
        coder.tapNotice(id,0,"【普通形态】");
    }
    coder.notice(tr("格斗家发动【蓄力一击】"));
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
    XuLiUesd=true;
    }
    else
    {
        setToken(0,token[0]-1);
        coder.tokenNotice(id,0,token[0]);
        coder.notice(tr("格斗家发动【苍炎之魂】"));
        *(int*)args[5]=NOREPLY;
        CangYanUsed=true;
    }
}

void GeDouJia::XuLiYiJi2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!XuLiUesd)
        return;
    Harm* harm= (Harm*)args[2];
    harm->harmPoint++;
    XuLiUesd=false;
}

void GeDouJia::XuLiYiJi3(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(!XuLiUesd)
        return;
    Harm harm;
    harm.type=MAGICHARM;
    harm.harmPoint=this->getToken(0);
    engine->timeLine3(harm,this,this,"蓄力一击");
    XuLiUesd=false;
}

void GeDouJia::NianDan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || this->getToken(0)==6)
        return;
    coder.askForSkill(id,"念弹");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt()==0)
        return;
    coder.notice("格斗家对玩家"+QString::number(arg[2].toInt())+"发动【念弹】");
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);
    PlayerEntity* ptr=engine->getPlayerByID(arg[2].toInt());
    int cross=ptr->getCrossNum();
    Harm harm;
    harm.harmPoint=1;
    harm.type=MAGICHARM;
    engine->timeLine3(harm,this,ptr,"念弹");
    if(cross==0)
    {
        Harm harm2;
        harm2.harmPoint=this->getToken(0);
        harm2.type=MAGICHARM;
        engine->timeLine3(harm2,this,this,"念弹");
    }
}

void GeDouJia::BaiShiHuanLingQuan1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || this->getToken(0)<3 || BaiShiUsed || StartUsed)
        return;
    coder.askForSkill(id,"百式幻龙拳");
    if(messageBuffer::readInfor() == 0)
        return;
    setToken(0,token[0]-3);
    coder.tokenNotice(id,0,token[0]);
    coder.notice(tr("格斗家发动【百式幻龙拳】"));
    setTap(1);
    coder.tapNotice(id,1,"【百式幻龙拳】");
    BaiShiUsed=true;
    BaiShiFirst=true;
    StartUsed=true;
}

void GeDouJia::BaiShiHuanLingQuan2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    if(!*(bool*)args[4])
        return;
    coder.notice(tr("格斗家发动【百式幻龙拳】"));
    Harm* harm=(Harm*)args[2];
    harm->harmPoint+=2;
}

void GeDouJia::BaiShiHuanLingQuan3(QList<void *> args)
{
    if(this !=(PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    if(*(bool*)args[4])
        return;
    coder.notice(tr("格斗家发动【百式幻龙拳】"));
    Harm* harm=(Harm*)args[2];
    harm->harmPoint++;
}


void GeDouJia::CangYanZhiHun2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !CangYanUsed)
        return;
    Harm harm;
    harm.type=MAGICHARM;
    harm.harmPoint=this->getToken(0);
    if(this->getToken(0)!=0)
        engine->timeLine3(harm,this,this,"苍炎之魂");
    CangYanUsed=false;
}

void GeDouJia::DouShenTianQu(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||StartUsed||this->getEnergy()==0)
        return;
    coder.askForSkill(id,"斗神天驱");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    if(this->getCrystal()>0)
        crystal--;
    else
        gem--;
    setGem(gem);
    setCrystal(crystal);
    coder.energyNotice(id,gem,crystal);
    coder.notice(tr("格斗家发动【斗神天驱】"));
    QList<CardEntity*>cards;
    for(int i=0;i<arg[2].toInt();i++)
        cards.append(getCardByID(arg[i+3].toInt()));
    coder.discardNotice(id,arg[2].toInt(),"n",cards);
    this->removeHandCards(cards,false);
    this->setCrossNum(this->getCrossNum()+2);
    coder.crossChangeNotice(id,this->getCrossNum());
    coder.notice("格斗家使用【斗神天驱】为自己增加2点治疗");
    StartUsed=true;
}

void GeDouJia::BaiShiTargetSet(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed || !BaiShiFirst)
        return;
    PlayerEntity* dst=(PlayerEntity*)args[1];
    BaiShiTargetID=dst->getID();
    BaiShiFirst=false;
}

void GeDouJia::BaiShiQuXiao1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || BaiShiFirst || !BaiShiUsed)
        return;
    if(!*(bool*)args[4])
        return;
    PlayerEntity*dst=(PlayerEntity*)args[1];
    if(BaiShiTargetID==dst->getID())
        return;
    setTap(0);
    coder.tapNotice(id,0,"【普通形态】");
    BaiShiUsed=false;
    BaiShiFirst=false;
    BaiShiTargetID=-1;
}

void GeDouJia::BaiShiQuXiao2(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    setTap(0);
    coder.tapNotice(id,0,"【普通形态】");
    BaiShiTargetID=-1;
    BaiShiUsed=false;
    BaiShiFirst=false;
}

void GeDouJia::BaiShiQuXiao3(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0] || !BaiShiUsed)
        return;
    setTap(0);
    coder.tapNotice(id,0,"【普通形态】");
    BaiShiFirst=false;
    BaiShiUsed=false;
    BaiShiTargetID=-1;
}

void GeDouJia::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    StartUsed=false;
}


WuNv::WuNv(BackgroundEngine *engine, int id, int color):PlayerEntity(engine, id, color)
{
    this->characterID = 23;
    this->star = 5;
    this->makeConnection(engine);
    TongShengID = -1;
    tap = false;
}

void WuNv::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(TongShengGongSi(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(XueZhiAiShang(QList<void*>)));
    connect(engine,SIGNAL(handCardsChange(PlayerEntity*)),this,SLOT(ToPuTongXingtai(PlayerEntity*)));
    connect(engine,SIGNAL(trueLoseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(ToLiuXueXingTai(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(LiuXue(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(NiLiu(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XueZhiBeiMing(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XueZhiZuZhou(QList<void*>)));
}

void WuNv::TongShengGongSi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2301)
        return;
    coder.notice("巫女对玩家"+QString::number(magic->dstID)+"发动【同生共死】");
    engine->drawCards(2, 0, this);
    TongShengID = magic->dstID;
    coder.specialNotice(TongShengID,3,1);
    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    int change;
    if(tap)
        change = 1;
    else
        change = -2;
    if(TongShengID!=this->getID())
    {
        this->addHandCardsRange(change);
        coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
    }
    dst->addHandCardsRange(change);
    coder.handcardMaxNotice(dst->getID(),dst->getHandCardMax());
}

void WuNv::XueZhiAiShang(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||TongShengID==-1)
        return;
    coder.askForSkill(this->getID(),"血之哀伤");
    BatInfor start = messageBuffer::readBatInfor();
    if(start.infor2 == 0)
        return;

    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,this,"血之哀伤");

    PlayerEntity* oldDst = engine->getPlayerByID(TongShengID);
    int change;
    if(start.dstID == -1)
    {
        coder.notice("巫女取消【同生共死】");
        coder.specialNotice(TongShengID,3,0);
        TongShengID=-1;
        if(tap)
            change = -1;
        else
            change = 2;
        if(TongShengID!=this->getID())
        {
        this->addHandCardsRange(change);
        coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
        }
        oldDst->addHandCardsRange(change);
        coder.handcardMaxNotice(oldDst->getID(),oldDst->getHandCardMax());
    }
    else
    {
        PlayerEntity* newDst = engine->getPlayerByID(start.dstID);
        coder.notice("巫女将【同生共死】转移至玩家"+QString::number(start.dstID));
        coder.specialNotice(TongShengID,3,0);
        if(tap)
            change = -1;
        else
            change = 2;
        if(TongShengID!=this->getID())
        {
            oldDst->addHandCardsRange(change);
            coder.handcardMaxNotice(oldDst->getID(),oldDst->getHandCardMax());
        }
        TongShengID = start.dstID;
        coder.specialNotice(TongShengID,3,1);
        if(tap)
            change = 1;
        else
            change = -2;
        if(TongShengID!=this->getID())
        {
            newDst->addHandCardsRange(change);
            coder.handcardMaxNotice(newDst->getID(), newDst->getHandCardMax());
        }
    }
}

void WuNv::ToPuTongXingtai(PlayerEntity *dst)
{
    if(this != dst)
        return;
    if(this->getHandCardNum()>2||!tap)
        return;
    tap = false;
    coder.tapNotice(this->getID(),0,"【普通形态】");
    if(TongShengID!=-1)
    {
        PlayerEntity* dst = engine->getPlayerByID(TongShengID);
        if(TongShengID!=this->getID())
        {
            this->addHandCardsRange(-3);
            coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
        }
        dst->addHandCardsRange(-3);
        coder.handcardMaxNotice(dst->getID(),dst->getHandCardMax());
    }
}

void WuNv::ToLiuXueXingTai(int harmed, int *howMany, PlayerEntity* dst)
{
    if(dst!=this||tap||harmed == 0||*howMany<=0)
        return;
    tap = true;
    coder.tapNotice(this->getID(),1,"【流血形态】");
    if(TongShengID!=-1)
    {
        PlayerEntity* dst = engine->getPlayerByID(TongShengID);
        if(TongShengID!=this->getID())
        {
            this->addHandCardsRange(3);
            coder.handcardMaxNotice(this->getID(),this->getHandCardMax());
        }
        dst->addHandCardsRange(3);
        coder.handcardMaxNotice(dst->getID(),dst->getHandCardMax());
    }
    this->setCrossNum(this->getCrossNum()+1);
    coder.crossChangeNotice(this->getID(),this->getCrossNum());
}

void WuNv::LiuXue(QList<void *> args)
{
    if(this != (PlayerEntity*) args[0]||!tap)
        return;
    Harm harm;
    harm.harmPoint = 1;
    harm.type = MAGIC;
    this->engine->timeLine3(harm,this,this,"流血");
}

void WuNv::NiLiu(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2303)
        return;
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    coder.notice("巫女发动【逆流】，弃2张牌，增加1治疗");
    this->removeHandCards(cards,false);
    coder.discardNotice(this->getID(), 2, "n", cards);
    this->setCrossNum(this->getCrossNum()+1);
    coder.crossChangeNotice(this->getID(),this->getCrossNum());
}

void WuNv::XueZhiBeiMing(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2304)
        return;
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.notice("巫女对玩家"+QString::number(magic->dstID)+"发动【血之悲鸣】");
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(),1,"y",cards);
    Harm harm;
    harm.harmPoint = magic->infor2;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,engine->getPlayerByID(magic->dstID),"血之悲鸣");
    engine->timeLine3(harm,this,this,"血之悲鸣");
}

void WuNv::XueZhiZuZhou(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != this->getID())
        return;
    if(magic->infor1 != 2305)
        return;

    gem--;
    coder.energyNotice(id,gem,crystal);
    coder.notice("巫女对玩家"+QString::number(magic->dstID)+"发动血之诅咒");

    PlayerEntity* dst = engine->getPlayerByID(magic->dstID);
    Harm harm;
    harm.harmPoint = 2;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,dst,"血之诅咒");

    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*> cards;
    if(magic->infor2>0)
    {
        for(int i=0;i<magic->infor2;i++)
            cards << getCardByID(cardNum[i].toInt());
        this->removeHandCards(cards,false);
        coder.discardNotice(this->getID(), magic->infor2, "n", cards);
    }
}

//灵魂 ps.仲裁【审判浪潮】加发动广播 格斗【念弹】,【苍炎蓄力】小修
LingHun::LingHun(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID=22;
    this->star=4.5;
    tokenMax[0]=6;
    tokenMax[1]=6;
    LianJieID=-1;
    LianJieUsed=false;
    StartUsed=false;
    LianJieChuFa=false;
    HeCheng=false;
    this->makeConnection(engine);
}

void LingHun::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(trueLoseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(LingHunTunShi(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(specialFinishSIG(QList<void*>)),this,SLOT(LingHunTunShi2(QList<void*>)));
    connect(engine,SIGNAL(loseMoraleHeChengSIG(int,int*,PlayerEntity*)),this,SLOT(LingHunTunShi3(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunZhaoHuan(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunJingXiang(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunZhenBao(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(LingHunCiYu(QList<void*>)));
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(LingHunZhuanHuan(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(LingHunLianJie1(QList<void*>)));
    connect(engine,SIGNAL(timeLine6SIG(QList<void*>)),this,SLOT(LingHunLianJie2(QList<void*>)));
    connect(engine,SIGNAL(timeLine6DrawedSIG(QList<void*>)),this,SLOT(LingHunLianJie3(QList<void*>)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(LingHunZengFu(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

void LingHun::LingHunTunShi(int harmed, int *howMany, PlayerEntity *dst)
{
    if(dst->getColor()!=color || *howMany==0)
        return;
    setToken(0,token[0]+*howMany);
    coder.tokenNotice(id,0,token[0]);
    coder.notice(tr("灵魂术士发动【灵魂吞噬】"));
}

void LingHun::LingHunTunShi2(QList<void *> args)
{
    PlayerEntity* ptr=(PlayerEntity*)args[0];
    if(ptr->getColor()==this->getColor())
        return;
    HeCheng=true;
}

void LingHun::LingHunTunShi3(int harmed, int *howMany, PlayerEntity *dst)
{
    if(!HeCheng)
        return;
    HeCheng=false;
    if(dst->getColor()==color)
        return;
    setToken(0,token[0]+*howMany);
    coder.tokenNotice(id,0,token[0]);
    coder.notice(tr("灵魂术士发动【灵魂吞噬】"));
}

void LingHun::LingHunZhaoHuan(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2201)
        return;
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*>cards;
    for(int i=0;i<magic->infor2;i++)
        cards.append(getCardByID(cardNum[i].toInt()));
    coder.discardNotice(id,magic->infor2,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice(tr("灵魂术士发动【灵魂召还】"));
    setToken(1,token[1]+1+magic->infor2);
    coder.tokenNotice(id,1,token[1]);
}

void LingHun::LingHunJingXiang(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2202)
        return;
    setToken(0,token[0]-2);
    coder.tokenNotice(id,0,token[0]);
    QStringList cardNum = magic->inforstr.split(":");
    QList<CardEntity*>cards;
    coder.notice(tr("灵魂术士发动【灵魂镜像】"));
    if(magic->infor2 != 0)
    {
        for(int i=0;i<magic->infor2;i++)
            cards.append(getCardByID(cardNum[i].toInt()));
        coder.discardNotice(id,magic->infor2,"y",cards);
        this->removeHandCards(cards,true);
    }
    else
    {
        coder.discardNotice(this->getID(),this->getHandCards().length(),"n",this->getHandCards());
        this->removeHandCards(this->getHandCards(),false,true);
    }
    PlayerEntity* dst=engine->getPlayerByID(magic->dstID);
    int drawNum=3;
    if(dst->getHandCardMax()-dst->getHandCardNum()>=3)
        engine->drawCards(drawNum,0,dst);
    else
    {
        drawNum=dst->getHandCardMax()-dst->getHandCardNum();
        engine->drawCards(drawNum,0,dst);
    }
}

void LingHun::LingHunZhenBao(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2203)
        return;
    setToken(0,token[0]-3);
    coder.tokenNotice(id,0,token[0]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("灵魂术士对玩家"+QString::number(magic->dstID)+"发动【灵魂震爆】");
    PlayerEntity* dst=engine->getPlayerByID(magic->dstID);
    Harm harm;
    if(dst->getHandCardNum()<3 && dst->getHandCardMax()>5)
        harm.harmPoint=5;
    else
        harm.harmPoint=3;
    harm.type=MAGICHARM;
    engine->timeLine3(harm,this,dst,"灵魂震爆");
}

void LingHun::LingHunCiYu(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2204)
        return;
    setToken(1,token[1]-3);
    coder.tokenNotice(id,1,token[1]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    coder.discardNotice(this->getID(),1,"y",cards);
    this->removeHandCards(cards,true);
    coder.notice("灵魂术士对玩家"+QString::number(magic->dstID)+"发动【灵魂赐予】");
    PlayerEntity* dst=engine->getPlayerByID(magic->dstID);
    dst->setGem(dst->getGem()+2);
    coder.energyNotice(magic->dstID,dst->getGem(),dst->getCrystal());
}

void LingHun::LingHunZhuanHuan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(!*(bool*)args[4])
        return;
    if(this->getToken(0)==0 && this->getToken(1)==0)
        return;
    coder.askForSkill(id,"灵魂转换");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    coder.notice(tr("灵魂术士发动【灵魂转换】"));
    if(arg[2].toInt() == 0)
    {
        setToken(0,token[0]-1);
        setToken(1,token[1]+1);
        coder.tokenNotice(id,0,token[0]);
        coder.tokenNotice(id,1,token[1]);
        coder.notice(tr("灵魂术士转换1点黄魂"));
    }
    else
    {
        setToken(0,token[0]+1);
        setToken(1,token[1]-1);
        coder.tokenNotice(id,0,token[0]);
        coder.tokenNotice(id,1,token[1]);
        coder.notice(tr("灵魂术士转换1点蓝魂"));
    }
}

void LingHun::LingHunLianJie1(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    if(this->getToken(0)==0 || this->getToken(1)==0)
        return;
    if(LianJieUsed || StartUsed)
        return;
    coder.askForSkill(id,"灵魂链接");
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0)
        return;
    setToken(0,token[0]-1);
    setToken(1,token[1]-1);
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
    LianJieID=arg[2].toInt();
    coder.notice("灵魂术士对玩家"+QString::number(LianJieID)+"发动【灵魂链接】");
    coder.specialNotice(LianJieID,2,1);
    LianJieUsed=true;
    StartUsed=true;
}

void LingHun::LingHunLianJie2(QList<void *> args)
{
    if(LianJieChuFa || !LianJieUsed)
        return;
    Harm* harm=(Harm*)args[2];
    PlayerEntity* dst=(PlayerEntity*)args[1];
    PlayerEntity* src=(PlayerEntity*)args[0];
    if(dst->getID()!=this->getID() && dst->getID()!=LianJieID)
        return;
    if(this->getToken(1)==0)
        return;
    if(harm->harmPoint==0)
        return;
    coder.askForSkill(id,"灵魂链接",QString::number(harm->harmPoint));
    QString msg=messageBuffer::readMsg();
    QStringList arg=msg.split(";");
    if(arg[1].toInt() == 0 || arg[2].toInt() == 0)
        return;
    int howMany=arg[2].toInt();
    coder.notice("灵魂术士发动【灵魂链接】，转移"+QString::number(howMany)+"点伤害");
    LianJieChuFa=true;
    setToken(1,token[1]-howMany);
    coder.tokenNotice(id,1,token[1]);
    harm->harmPoint-=howMany;
    bool flag1,flag2;
    flag1=flag2=true;
    PlayerEntity* ptr = engine->getNext(src);
    do
    {
        if(flag1 && ptr->getID()==id)
        {
            flag1=false;
            Harm zhuanyi1;
            zhuanyi1.type=MAGICHARM;
            zhuanyi1.harmPoint=howMany;
            Harm zhuanyi2;
            zhuanyi2.type=harm->type;
            zhuanyi2.harmPoint=harm->harmPoint;
            if(dst->getID()==this->getID())
                engine->timeLine6(zhuanyi2,src,this);
            else
                engine->timeLine6(zhuanyi1,src,this);
            if(engine->checkEnd())
                break;
            LianJieChuFa=true;
        }
        else if(flag2 && ptr->getID()==LianJieID)
        {
            flag2=false;
            Harm zhuanyi1;
            zhuanyi1.type=MAGICHARM;
            zhuanyi1.harmPoint=howMany;
            Harm zhuanyi2;
            zhuanyi2.type=harm->type;
            zhuanyi2.harmPoint=harm->harmPoint;
            if(dst->getID()==this->getID())
                engine->timeLine6(zhuanyi2,src,engine->getPlayerByID(LianJieID));
            else
                engine->timeLine6(zhuanyi1,src,engine->getPlayerByID(LianJieID));
            if(engine->checkEnd())
                break;
            LianJieChuFa=true;
        }
        ptr = engine->getNext(ptr);
    }while(flag1||flag2);
    harm->harmPoint=0;
    LianJieChuFa=false;
}

void LingHun::LingHunLianJie3(QList<void *> args)
{
    if(!LianJieChuFa)
        return;
    PlayerEntity* dst=(PlayerEntity*)args[1];
    if(dst->getID()!=this->getID() && dst->getID()!=LianJieID)
        return;
    LianJieChuFa=false;
}

void LingHun::LingHunZengFu(QList<void *> args)
{
    if(StartUsed)
        return;
    if(this->getGem()==0)
        return;
    if(this != (PlayerEntity*)args[0])
        return;
    coder.askForSkill(id,"灵魂增幅");
    if(messageBuffer::readInfor() == 0)
        return;
    setGem(gem-1);
    coder.energyNotice(id,gem,crystal);
    coder.notice(tr("灵魂术士发动【灵魂增幅】"));
    StartUsed=true;
    setToken(0,token[0]+2);
    setToken(1,token[1]+2);
    coder.tokenNotice(id,0,token[0]);
    coder.tokenNotice(id,1,token[1]);
}

void LingHun::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    StartUsed=false;
}

//红莲骑士 28
HongLian::HongLian(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 28;
    this->star = 4;
    //猩红信仰
    crossMax = 4;
    tokenMax[0]=2;
    XingHongShengYueUsed = false;
    tap = false;
    this->makeConnection(engine);
}

void HongLian::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(timeLine1SIG(QList<void*>)),this,SLOT(XingHongShengYue(QList<void*>)));
    connect(engine,SIGNAL(askForHeal(Harm,PlayerEntity*,PlayerEntity*,int*)),this,SLOT(XingHongXinYang(Harm,PlayerEntity*,PlayerEntity*,int*)));
    connect(engine,SIGNAL(actionPhaseSIG(QList<void*>)),this,SLOT(XueXingDaoYan(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(ShaLuShengYan(QList<void*>)));
    connect(engine,SIGNAL(trueLoseMoraleSIG(int,int*,PlayerEntity*)),this,SLOT(ToReXueFeiTeng(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(beforeLoseMoralSIG(int,int*,PlayerEntity*)),this,SLOT(ReXueFeiTeng(int,int*,PlayerEntity*)));
    connect(engine,SIGNAL(turnEndPhaseSIG(PlayerEntity*)),this,SLOT(OutReXueFeiTeng(PlayerEntity*)));
    connect(engine,SIGNAL(attackFinishSIG(QList<void*>)),this,SLOT(JieJiaoJieZao1(QList<void*>)));
    connect(engine,SIGNAL(magicFinishSIG(QList<void*>)),this,SLOT(JieJiaoJieZao1(QList<void*>)));
    connect(engine,SIGNAL(additonalActionSIG(QList<void*>)),this,SLOT(JieJiaoJieZao2(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(XingHongShiZi(QList<void*>)));
    connect(engine,SIGNAL(turnBeginPhaseSIG(QList<void*>)),this,SLOT(skillReset(QList<void*>)));
}

void HongLian::XingHongShengYue(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0]))
        return;
    if(!*(bool*)args[4])
        return;
    if(XingHongShengYueUsed)
        return;
    coder.askForSkill(this->getID(), "猩红圣约");
    if(messageBuffer::readInfor() == 0)
        return;

    XingHongShengYueUsed = true;
    this->setCrossNum(crossNum+1);
    coder.crossChangeNotice(this->getID(), crossNum);
    coder.notice("红莲骑士发动【猩红圣约】，增加1治疗");
}

void HongLian::XingHongXinYang(Harm harm, PlayerEntity *src, PlayerEntity *dst, int *crossAvailable)
{
    if(dst!=this)
        return;
    if(src!=this)
        *crossAvailable = 0;
}

void HongLian::XueXingDaoYan(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0]||this->getCrossNum()<=0)
        return;
    coder.askForSkill(this->getID(),"血腥祷言");
    BatInfor start = messageBuffer::readBatInfor();
    if(start.infor2 == 0||(start.infor3+start.infor5)<=0)
        return;
    setCrossNum(crossNum-start.infor3-start.infor5);
    coder.crossChangeNotice(this->getID(),crossNum);

    Harm harm;
    harm.harmPoint = start.infor3+start.infor5;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,this,"血腥祷言");

    PlayerEntity* dst = engine->getPlayerByID(start.dstID);
    dst->setCrossNum(dst->getCrossNum()+start.infor3);
    coder.crossChangeNotice(start.dstID,dst->getCrossNum());
    if(start.infor4!=-1)
    {
        dst = engine->getPlayerByID(start.infor4);
        dst->setCrossNum(dst->getCrossNum()+start.infor5);
        coder.crossChangeNotice(start.infor4,dst->getCrossNum());
    }
    setToken(0,token[0]+1);
    coder.tokenNotice(id,0,token[0]);

}

void HongLian::ShaLuShengYan(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0])||token[0]<=0)
        return;
    if(!*(bool*)args[4])
        return;
    coder.askForSkill(this->getID(),"杀戮盛宴");
    if(messageBuffer::readInfor() == 0)
        return;

    coder.notice("红莲骑士发动【杀戮盛宴】");
    setToken(0,token[0]-1);
    coder.tokenNotice(this->getID(),0,token[0]);

    Harm selfHarm;
    selfHarm.harmPoint = 4;
    selfHarm.type = MAGIC;
    engine->timeLine3(selfHarm,this,this,"杀戮盛宴");

    Harm *harm = (Harm*)args[2];
    harm->harmPoint += 2;
}

void HongLian::ToReXueFeiTeng(int harmed, int *howMany, PlayerEntity *dst)
{
    if(dst!=this||tap||harmed == 0||*howMany<=0)
        return;
    tap = true;
    coder.tapNotice(this->getID(),1,"【热血沸腾状态】");
}

void HongLian::ReXueFeiTeng(int harmed, int *howMany, PlayerEntity *dst)
{
    if(dst!=this||!tap||harmed == 0||*howMany<=0)
        return;
    *howMany = 0;
    coder.notice("红莲骑士处于【热血沸腾状态】，承受伤害不造成士气下降");
}

void HongLian::OutReXueFeiTeng(PlayerEntity * currunt)
{
    if(currunt!=this||!tap)
        return;
    tap = false;
    coder.tapNotice(this->getID(),0,"【普通形态】");

    this->setCrossNum(crossNum+2);
    coder.crossChangeNotice(this->getID(), crossNum);
    coder.notice("红莲骑士脱离【热血沸腾状态】，增加2治疗");
}

void HongLian::JieJiaoJieZao1(QList<void *> args)
{
    if(this != ((PlayerEntity*)args[0])||this->getEnergy() <= 0||!tap)
        return;
    coder.askForSkill(this->getID(),"戒骄戒躁");
    if(messageBuffer::readInfor() == 0)
        return;
    coder.notice("红莲骑士发动【戒骄戒躁】");
    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),gem,crystal);

    tap = false;
    coder.tapNotice(this->getID(),0,"【普通形态】");
    engine->addActionNum(ATTACKORMAGIC);
}

void HongLian::JieJiaoJieZao2(QList<void *> args)
{
    BatInfor *skill = (BatInfor*)args[0];
    if(id != skill->srcID||skill->infor1!=2802)
        return;
    coder.notice("红莲骑士使用【戒骄戒躁】的攻击或法术行动");
}

void HongLian::XingHongShiZi(QList<void *> args)
{
    BatInfor *magic = (BatInfor*)args[0];
    if(magic->srcID != id || magic->infor1 != 2803||getEnergy()<=0)
        return;

    if(getCrystal()>0)
        crystal--;
    else
        gem--;
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());
    coder.notice("红莲骑士对玩家"+QString::number(magic->dstID)+"发动【猩红十字】");
    setToken(0,token[0]-1);
    coder.tokenNotice(this->getID(),0,token[0]);
    QList<CardEntity*> cards;
    cards << getCardByID(magic->CardID);
    cards << getCardByID(magic->infor2);
    this->removeHandCards(cards,true);
    coder.discardNotice(this->getID(), 2, "y", cards);

    Harm selfHarm;
    selfHarm.harmPoint = 4;
    selfHarm.type = MAGIC;
    engine->timeLine3(selfHarm,this,this,"猩红十字");

    Harm harm;
    harm.harmPoint = 3;
    harm.type = MAGIC;
    engine->timeLine3(harm,this,engine->getPlayerByID(magic->dstID),"猩红十字");
}

void HongLian::skillReset(QList<void *> args)
{
    if(this != (PlayerEntity*)args[0])
        return;
    XingHongShengYueUsed = false;
}


LingFu::LingFu(BackgroundEngine *engine, int id, int color):PlayerEntity(engine,id,color)
{
    this->characterID = 18;
    this->star = 4;
    this->makeConnection(engine);

}

void LingFu::nianZhou(QList<void *> args)
{
    if(this->coverCards.count() >= 2)
        return;
    coder.askForSkill(this->getID(),"念咒");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;
    coder.notice("灵符师发动【念咒】");

    CardEntity* yaoLi = getCardByID(ans.CardID);
    QList<CardEntity*> yaoLis;
    yaoLis << yaoLi;

    engine->moveCardFrom(yaoLi);
    engine->moveCardToCover(yaoLi,this->getID());
    coder.moveCardNotice(1,yaoLis,this->getID(),HAND,this->id,COVERED);
    coder.coverCardNotice(this->id,1,yaoLis,false,false);

    this->setToken(2,this->getCoverCards().count());
    coder.tokenNotice(this->getID(),2,this->getCoverCards().count());
}

void LingFu::leiMing(QList<void *> args)
{
    BatInfor* action = (BatInfor*) args[0];
    if(action->srcID != this->id)
        return;
    if(action->infor1 != 1802)
        return;
    coder.notice("灵符师发动【灵符-雷鸣】");

    CardEntity* card = getCardByID(action->CardID);
    QList<CardEntity*> cards;
    cards << card;
    this->removeHandCards(cards,true);
    coder.discardNotice(this->id,1,"y",cards);

    Harm harm;
    harm.type = MAGIC;
    harm.harmPoint = 1;
    this->lingLiBengJie(harm);

    PlayerEntity* target = this;
    for(int counter = 0;counter < 2;target = target->getNext())
    {
        if(target->getID() != action->infor2 && target->getID() != action->infor3)
            continue;
        engine->timeLine3(harm,this,target,"灵符-雷鸣");
        counter++;
    }

    this->nianZhou(args);
}

void LingFu::fengXing(QList<void *> args)
{
    BatInfor* action = (BatInfor*) args[0];
    if(action->srcID != this->id)
        return;
    if(action->infor1 != 1803)
        return;
    coder.notice("灵符师发动【灵符-风行】");

    CardEntity* card = getCardByID(action->CardID);
    QList<CardEntity*> cards;
    cards << card;
    this->removeHandCards(cards,true);
    coder.discardNotice(this->id,1,"y",cards);

    Harm harm;
    harm.type = MAGIC;
    harm.harmPoint = 1;

    this->lingLiBengJie(harm);
    PlayerEntity* target = this;
    for(int counter = 0;counter < 2;target = target->getNext())
    {
        if(target->getID() != action->infor2 && target->getID() != action->infor3)
            continue;
        counter++;

        if(target->getHandCards().count() == 0)
            continue;
        coder.askForDiscard(target->getID(),1,"n");

        QList<CardEntity*> cardChosen;
        cardChosen = messageBuffer::readCardID(1);
        target->removeHandCards(cardChosen,false);
        coder.discardNotice(target->getID(),1,"n",cardChosen);

    }

    this->nianZhou(args);
}

void LingFu::makeConnection(BackgroundEngine *engine)
{
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(leiMing(QList<void*>)));
    connect(engine,SIGNAL(skillMagic(QList<void*>)),this,SLOT(fengXing(QList<void*>)));
    connect(engine,SIGNAL(timeLine2hitSIG(QList<void*>)),this,SLOT(baiGuiYeXing(QList<void*>)));
}

void LingFu::lingLiBengJie(Harm &hurt)
{
    if(this->getEnergy() <= 0)
        return;
    coder.askForSkill(this->getID(),"灵力崩解");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;

    coder.notice("灵符师发动【灵力崩解】");
    if(ans.infor1 == 0)
        this->setCrystal(this->getCrystal()-1);
    else
        this->setGem(this->getGem()-1);
    coder.energyNotice(this->getID(),this->getGem(),this->getCrystal());

    hurt.harmPoint++;
}

void LingFu::baiGuiYeXing(QList<void *> args)
{
    PlayerEntity *src = (PlayerEntity*)args[0];
    bool isActive = *(bool*)args[4];
    if(src->getID() != this->getID())
        return;
    if(!isActive)
        return;
    if(this->getCoverCards().count() == 0)
        return;

    coder.askForSkill(this->getID(),"百鬼夜行");
    BatInfor ans = messageBuffer::readBatInfor();
    if(ans.reply == 0)
        return;

    coder.notice("灵符师发动【百鬼夜行】");

    CardEntity *yaoLi = getCardByID(ans.CardID);
    QList<CardEntity*> yaoLis;
    yaoLis << yaoLi;
    PlayerEntity *dst1,*dst2;
    BatInfor ans2;




    if(yaoLi->getElement() == "fire")
    {
        coder.askForSkillNumber(this->getID(),180501);
        ans2 = messageBuffer::readBatInfor();
        dst1 = engine->getPlayerByID(ans2.infor1);
        if(ans2.reply != 0)
        {
            engine->moveCardFromCoverToDiscard(yaoLi,true);
            this->setToken(2,this->getCoverCards().count());
            coder.tokenNotice(this->getID(),2,this->getCoverCards().count());
            coder.coverCardNotice(this->getID(),1,yaoLis,true,true);
            dst2 = engine->getPlayerByID(ans2.infor2);
            PlayerEntity *target = this;

            Harm hurt;
            hurt.type = MAGIC;
            hurt.harmPoint = 1;
            this->lingLiBengJie(hurt);

            for(int counter = 0;counter < engine->getPlayerNum();counter++,target = target->getNext())
            {
                if(target->getID() == dst1->getID() || target->getID() == dst2->getID())
                    continue;
                engine->timeLine3(hurt,this,target,"百鬼夜行");
            }
            return;
        }
    }
    else
    {
        coder.askForSkillNumber(this->getID(),180503);
        int dstID = messageBuffer::readInfor();
        dst1 = engine->getPlayerByID(dstID);
    }

    engine->moveCardFromCoverToDiscard(yaoLi,false);
    this->setToken(2,this->getCoverCards().count());
    coder.tokenNotice(this->getID(),2,this->getCoverCards().count());
    coder.coverCardNotice(this->getID(),1,yaoLis,true,false);

    Harm hurt;
    hurt.type = MAGIC;
    hurt.harmPoint = 1;
    this->lingLiBengJie(hurt);
    engine->timeLine3(hurt,this,dst1,"百鬼夜行");

}
