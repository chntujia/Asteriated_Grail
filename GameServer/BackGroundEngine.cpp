#include "BackgroundEngine.h"
#include "Characters.h"
#include "Server.h"
#include <QTextStream>
#include <QFile>

void TeamArea::initialTeam()
{
    this->moraleBLUE = 15;
    this->moraleRED = 15;
    this->gemBLUE = 0;
    this->gemRED = 0;
    this->crystalBLUE = 0;
    this->crystalRED = 0;
    this->cupBLUE = 0;
    this->cupRED = 0;
}

void TeamArea::setCrystal(int color, int value)
{
    if(color == RED && value+gemRED<=5)
        this->crystalRED = value;
    else if(color == BLUE && value+gemBLUE<=5)
        this->crystalBLUE = value;
}

void TeamArea::setCup(int color, int value)
{
    if(color == RED)
        this->cupRED = value;
    else if(color == BLUE)
        this->cupBLUE = value;
}

void TeamArea::setGem(int color, int value)
{
    if(color == RED && value+crystalRED<=5)
        this->gemRED = value;
    else if(color == BLUE && value+crystalBLUE<=5)
        this->gemBLUE = value;
}

void TeamArea::setMorale(int color, int value)
{
    if(color == RED)
        this->moraleRED = value;
    else if(color == BLUE)
        this->moraleBLUE = value;

}

BackgroundEngine::BackgroundEngine():QObject(0)
{
    this->initial();
}

PlayerEntity* BackgroundEngine::setRole(int roleID,BackgroundEngine* engine,int id,int color)
{
    switch(roleID)
    {
    case 1:
        return new JianSheng(engine,id,color);
        break;
    case 2:
        return new Berserker(engine,id,color);
        break;
    case 3:
        return new BowLady(engine,id,color);
        break;
    case 4:
        return new FengYin(engine,id,color);
        break;
    case 5:
        return new AnSha(engine,id,color);
        break;
    case 6:
        return new Saintness(engine,id,color);
        break;
    case 7:
        return new TianShi(engine,id,color);
        break;
    case 8:
        return new MoDao(engine,id,color);
        break;
    case 9:
        return new MoJian(engine,id,color);
        break;
	case 10:
        return new ShengQiang(engine, id, color);
		break;
    case 11:
        return new YuanSu(engine,id,color);
        break;
    case 12:
        return new MaoXian(engine,id,color);
        break;
    case 13:
        return new SiLing(engine,id,color);
        break;
    case 14:
        return new ZhongCai(engine,id,color);
        break;
    case 15:
        return new ShenGuan(engine,id,color);
        break;
    case 16:
        return new QiDao(engine,id,color);
        break;
    case 17:
        return new XianZhe(engine,id,color);
        break;

    case 20:
        return new GeDouJia(engine,id,color);
        break;
    case 18:
        return new LingFu(engine,id,color);  //debugó?
        break;
    case 21:
        return new YongZhe(engine,id,color);
        break;
    case 22:
        return new LingHun(engine,id,color);
        break;
    case 23:
        return new WuNv(engine,id,color);
        break;
    case 28:
        return new HongLian(engine,id,color);
        break;
    case 29:
        return new MoQiang(engine,id,color);
        break;
    }

}
void BackgroundEngine::randomize(QList<int> *queue)
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec());
    int n=queue->size();
    int choosen;
    for(int i=n-1;i>0;i--)
    {
        choosen=qrand() % (i+1);
        queue->swap(i,choosen);
    }
}

void BackgroundEngine::seatPrearrange(int id, bool isRed)
{
    if(isRed && red.size()<playerSum/2)
        red<<id;
    if(!isRed && blue.size()<playerSum/2)
        blue<<id;
}

void BackgroundEngine::seatArrange()
{
    queue="";
    QList<int> ids;
    for(int i=0;i<playerSum;i++)
        ids<<i;
    randomize(&ids);

    int colors[]={1,0,1,0,0,1};
    for(int i=0;i<playerSum;i++)
        if(!red.contains(ids[i])&& !blue.contains(ids[i]))
            if(red.size()<playerSum/2)
                red<<ids[i];
            else
                blue<<ids[i];
    randomize(&red);
    randomize(&blue);
    for(int i=0;i<playerSum;i++)
    {
        if(colors[i]==1)
            queue+=QString::number(red.takeFirst());
        else
            queue+=QString::number(blue.takeFirst());
    }
    queue+="101001";
    coder.beginNotice(queue);
    playerList.clear();
    for(int i = 0;i < this->getPlayerNum();i++)
        playerList << NULL;

    for(int i=1; i<= 18 ;i++)
        roles<<i;
    roles<<21;
    roles<<20;
    roles<<22;
    roles<<23;
    roles<<28;
    roles<<29;
    randomize(&roles);

}

void BackgroundEngine::seatPostarrange()
{
    for(int i = 0;i < this->getPlayerNum() - 1;i++)
        this->playerList.at(i)->setNext(this->playerList.at(i+1));
    this->playerList.at(this->getPlayerNum()-1)->setNext(this->playerList.at(0));
    for(int i = 0;i < this->playerList.size();i++)
        this->playerList[i]->setSeatNum(i);
}

void BackgroundEngine::roleRandom()
{
    for(int i=0;i<playerSum;i++){
        playerList[i]=setRole(roles[i],this,queue[i].digitValue(),queue[i+playerSum].digitValue());
        coder.roleNotice(queue[i].digitValue(),roles[i]);
    }
    seatPostarrange();
    gameStart();
}

void BackgroundEngine::role3Pick1()
{    
    for(int i=0;i<playerSum;i++)
        coder.askForRolePick(i,3,&roles);
}

void BackgroundEngine::role3Pick1Reply(int id,int roleID)
{
    for(int i=0;i<playerSum;i++)
        if(queue[i].digitValue()==id){
            playerList[i]=setRole(roleID,this,id,queue[i+playerSum].digitValue());
            coder.roleNotice(id,roleID);
            break;
        }
}



//ó??·??ê?,ó??·á??ì????
void BackgroundEngine::gameStart()
{
    for(int i = 0;i < this->getPlayerNum();i++)
    {
        this->drawCards(4,NOTHARMED,this->playerList.at(i));
    }

    this->setCurrentPlayer(this->playerList.at(0));

    teamArea.initialTeam();

    this->playing = true;
    while(this->playing)
    {
       this->turnBeginPhase(this->getCurrentPlayer());

       if(!this->playing)
           break;

       this->actionPhase();

       if(!this->playing)
           break;

       this->turnEndPhase();

       if(!this->playing)
           break;

       this->setCurrentPlayer(this->getCurrentPlayer()->getNext());

       if(!this->playing)
           break;
   }
   clearData();
}
//????±???ê???
void BackgroundEngine::clearData()
{
    for(int i = 0;i < cardList.size();i++)
    {
        delete cardList[i];
    }
    cardList.clear();
    for(int i = 0;i < this->playerList.size();i++)
    {
        delete playerList[i];
    }
    this->playerList.clear();
}


void BackgroundEngine::initial()
{
    this->pile.clear();
    this->discardPile.clear();
    this->discardPileCovered.clear();
    //?áè?????????
    QFile log("./log.txt");
    log.open(QIODevice::WriteOnly);
    QTextStream out(&log);

    out << "try to open ";

    QFile fp("./cardDB.txt");

    QTextStream in(&fp);
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    in.setCodec(codec);

    if (!fp.open(QIODevice::ReadOnly)) {
            out << "Cannot open file for reading. ";
    }

    out << "ready to read";

    QStringList cardEntry;
    while (!in.atEnd())
    {
        cardEntry=in.readLine().split("\t");
        cardList<<new CardEntity(&cardEntry);       
    }

    out << cardList.length();
    log.close();
    //????
    this->shuffle(false);
    fp.close();
    setPlayerNum(playerSum);

}

//°???????????
void BackgroundEngine::shuffle(bool reShuffle)
{
    QFile fp("./log.txt");
    fp.open(QIODevice::WriteOnly);
    QTextStream out(&fp);
    //ó??”í?ê±??éè?????ú???ó
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    out << "\n shuffle\n";
    fp.close();
    this->pile.clear();

    if(!reShuffle)
    {
        for(int i = cardList.size()-1;i >=0;i--)
        {
            this->pile << NULL;
        }
        int left = cardList.size();
        for(int i = 0;i < cardList.size();i++)
        {
            int order = qrand() % left;

            for(int j = 0;j < this->pile.size();j++)
            {
                if(this->pile.at(j) != NULL)
                    continue;
                if(order == 0)
                {
                    this->pile[j] = cardList.at(i);
                    this->pile.at(j)->setOwner(OWNERLESS);
                    this->pile.at(j)->setPlace(PILE);
                    left--;
                    break;
                }
                else
                    order--;
            }
        }
    }
    else
    {
        for(int i = 0;i < this->discardPile.size() + this->discardPileCovered.size();i++)
        {
            this->pile << NULL;
        }
        int left = this->discardPile.size() + this->discardPileCovered.size();
        for(int i = 0;i < this->discardPile.size();i++)
        {
            int order = qrand() % left;
            for(int j = 0;j < this->pile.size();j++)
            {
                if(this->pile.at(j) != NULL)
                    continue;
                if(order == 0)
                {
                    this->pile[j] = this->discardPile.at(i);
                    this->pile.at(j)->setOwner(OWNERLESS);
                    this->pile.at(j)->setPlace(PILE);
                    left--;
                    break;
                }
                else
                    order--;
            }
        }
        for(int i = 0;i < this->discardPileCovered.size();i++)
        {
            int order = qrand() % left;
            for(int j = 0;j < this->pile.size();j++)
            {
                if(this->pile.at(j) != NULL)
                    continue;
                if(order == 0)
                {
                    this->pile[j] = this->discardPileCovered.at(i);
                    this->pile.at(j)->setOwner(OWNERLESS);
                    this->pile.at(j)->setPlace(PILE);
                    left--;
                    break;
                }
                else
                    order--;
            }
        }
        this->discardPile.clear();
        this->discardPileCovered.clear();
        coder.reshuffleNotice(pile.size());
    }
}
//?ì?éê?·ń?áê?ó??·
bool BackgroundEngine::checkEnd()
{
    if(teamArea.getMorale(RED) <= 0 || teamArea.getCup(BLUE) == 5)
    {
        coder.endNotice(BLUE);

        this->playing = false;
    }
    else if(teamArea.getMorale(BLUE) <= 0 || teamArea.getCup(RED) == 5)
    {
        coder.endNotice(RED);

        this->playing = false;
    }
    return !playing;
}
//??????ê?
void BackgroundEngine::drawCards(int num,int harmed,PlayerEntity* player)
{
    QList<CardEntity*> newCards;
    for(int i = 0;i < num;i++)
    {
        if(this->pile.isEmpty())
        {
            shuffle(true);
        }
        newCards << this->pile.takeLast();
    }

    coder.drawNotice(player->getID(),num,newCards);
    //?üá???í??ò???óê???
    player->addHandCards(newCards,harmed);
}
//??????àí
void BackgroundEngine::posionProcess(PlayerEntity* player,CardEntity* card)
{
    Harm harm;
    harm.harmPoint = 1;
    harm.type = MAGIC;
    //coder.magicHurtNotice(player->getID(),1,card->getSrcUser(),"????");
    this->timeLine3(harm,getPlayerByID(card->getSrcUser()),player,"中毒");

}
//?éè???àí
void BackgroundEngine::weakProcess(PlayerEntity* player,int howMany)
{
    coder.askForWeak(player->getID(),howMany);

    int reply = messageBuffer::readInfor();


    if(reply == 0)
    {
        //ì???
        this->attackLeft = 0;
        this->magicLeft = 0;
        this->specialLeft = 0;
        this->attackOrMagicLeft = 0;
        this->actionLeft = 0;

        coder.weakNotice(player->getID(),0);
    }
    else if(reply == 1)
    {
        //????
        coder.weakNotice(player->getID(),1,howMany);
        this->drawCards(howMany,0,player);
    }
}

//??????ê?ê±?ì?a”±?°í??ò???°”??ù???§???í??ê??§??
void BackgroundEngine::checkEffect(PlayerEntity* player)
{
    for(int i = 0;i < player->getBasicEffect().size();i++)
    {
        if(player->getBasicEffect().at(i)->getMagicName() == POISONCARD)
        {
            posionProcess(player,player->getBasicEffect()[i]);
            player->removeBasicEffect(player->getBasicEffect()[i]);
            i--;
        }      
    }

    bool withoutTrap=true;
    emit weakSIG(player,&withoutTrap);
    for(int i = 0;i < player->getBasicEffect().size()&&withoutTrap;i++)
        if(player->getBasicEffect().at(i)->getMagicName() == WEAKCARD)
        { 
            weakProcess(player);
            player->removeBasicEffect(player->getBasicEffect()[i]);
            break;
        }    
}

//????ò??????ó”???àí???????à??????”?ê?óà??????ê?
void BackgroundEngine::acted(int kind)
{
    if(kind == ATTACK)
    {
        if(this->attackLeft != 0)
            this->attackLeft--;
        else if(this->attackOrMagicLeft != 0)
            this->attackOrMagicLeft--;
        else
            this->actionLeft--;
    }
    else if(kind == MAGIC)
    {
        if(this->magicLeft != 0)
            this->magicLeft--;
        else if(this->attackOrMagicLeft != 0)
            this->attackOrMagicLeft--;
        else
            this->actionLeft--;
    }
    else if(kind == SPECIAL)
    {
        if(this->specialLeft != 0)
            this->specialLeft--;
        else
            this->actionLeft--;
    }

//    qDebug() << attackNew<<magicNew<<attackOrMagicNew<<specialNew<<actionNew;
//    qDebug() << attackLeft<<magicLeft<<attackOrMagicLeft<<specialLeft<<actionLeft;
}

void BackgroundEngine::turnBeginPhase(PlayerEntity* currentPlayer)
{
    this->setCurrentPlayer(currentPlayer);
    this->currentPlayer->setYourTurn(true);
    this->resetActionNum();

    coder.turnBegineNotice(currentPlayer->getID());
    QList<void*> args ;
    args<<currentPlayer;
    emit turnBeginPhaseSIG(args);

    //checkEffect();
}
//ò???????ê?????ê?·ń?ü?????àó?????
bool BackgroundEngine::allowAttack()
{
    if(this->actionLeft != 0 || this->attackOrMagicLeft != 0 || this->attackLeft != 0)
        return true;
    else
        return false;
}

bool BackgroundEngine::allowMagic()
{
    if(this->actionLeft != 0 || this->attackOrMagicLeft != 0 || this->magicLeft != 0)
        return true;
    else
        return false;
}

bool BackgroundEngine::allowSpecial()
{
    if(this->actionLeft != 0)
        return true;
    else
        return false;
}

void BackgroundEngine::addActionNum(int kind)
{
    switch(kind)
    {
    case ATTACK:
        this->attackLeft++;
        break;
    case MAGIC:
        this->magicLeft++;
        break;
    case ATTACKORMAGIC:
        this->attackOrMagicLeft++;
        break;
    case SPECIAL:
        this->specialLeft++;
        break;
    case ALLACTION:
        this->actionLeft++;
        break;
    }
}



//??????????ê?
void BackgroundEngine::actionPhase()
{    
    //?a??±ê??±í??ê?·ń????????è???ò??-?????????????·?ó”??????????ú?á?éò?·??ú
    bool acted = false;
    bool firstTime=true;
    int actionFlag=0;
    bool canGiveUp=false;


    this->checkEffect(this->currentPlayer);
    QList<void*> args;
    args<<currentPlayer;

    while((this->allowAttack()||this->allowMagic()||this->allowSpecial())&&playing)
    {        
        //?ù???ê?í”?????àà±????êclient
        if(firstTime){
            emit actionPhaseSIG(args);
            emit tiaoXinPhaseSIG(currentPlayer,&actionFlag,&canGiveUp);
            firstTime=false;
        }
        if(!acted){
            coder.askForAction(currentPlayer->getID(),actionFlag,canGiveUp);
        }
        else{
            coder.askForAdditionalAction(currentPlayer->getID());
            BatInfor chosen = messageBuffer::readBatInfor();
            if(chosen.reply == FINISH)
                break;
            args.clear();
            args<<&chosen;
            emit additonalActionSIG(args);
        }

        //?áè?client”?????
        BatInfor bat = messageBuffer::readBatInfor();

        //·??ú??ía????
        if(bat.reply == FINISH)
            break;
        //??·?????
        if(bat.reply == UNACTIONAL)
        {                        
            //?ú??????
            this->reDraw();
            continue;
        }

        if(bat.reply == ATTACK || bat.reply==ATTACKSKILL)
        {
            //ê?ó???????
            CardEntity* usingCard = getCardByID(bat.CardID);
            PlayerEntity* dst = getPlayerByID(bat.dstID);
            PlayerEntity* src = getPlayerByID(bat.srcID);
            QList<CardEntity*> use;
            int realCard=1;
            use << usingCard;
            args.clear();
            args<<&bat;
            args<<&realCard;
            if(bat.reply==ATTACKSKILL)
                emit skillAttack(args);
            this->useCard(use,src,dst,false,realCard);
            this->timeLine1(usingCard,src,dst,true);

            acted = true;
            this->acted(ATTACK);
            args.clear();
            args<< src;
            args << dst;
            emit attackFinishSIG(args);

        }

        else if(bat.reply == MAGIC)
        {
            PlayerEntity* src1 = getPlayerByID(bat.srcID);
            args.clear();
            args<<src1;
            emit beforeMagicSIG(args);
            //ê?ó?·?ê???
            if(bat.infor1 == COMMONMAGIC)
                this->useMagicCard(bat.CardID,bat.srcID,bat.dstID);
            else
            {
                QList<void*> args;
                args << &bat;
                emit this->skillMagic(args);
            }
            acted = true;
            this->acted(MAGIC);
            PlayerEntity* src = getPlayerByID(bat.srcID);
            args.clear();
            args<< src;
            emit magicFinishSIG(args);
        }
        else if(bat.reply == SPECIAL)
        {
            //ì?êa????
            if(bat.CardID == BUY)
            {
                coder.notice("执行【购买】");
                this->drawCards(3,0,this->getCurrentPlayer());
                int color = this->currentPlayer->getColor();
                teamArea.setGem(color,teamArea.getGem(color) + bat.infor1);
                teamArea.setCrystal(color,teamArea.getCrystal(color) + bat.infor2);                
                coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
                emit specialFinishSIG(args);
            }
            else if(bat.CardID == SYNTHESIZE)
            {
                coder.notice("执行【合成】");
                this->drawCards(3,0,currentPlayer);
                int color = this->currentPlayer->getColor();
                teamArea.setGem(color,teamArea.getGem(color) - bat.infor1);
                teamArea.setCrystal(color,teamArea.getCrystal(color) - bat.infor2);
                teamArea.setCup(color,teamArea.getCup(color) + 1);
                teamArea.setMorale(!color,teamArea.getMorale(!color) - 1);                
                coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
                coder.cupNotice(color,teamArea.getCup(color));
                coder.moraleNotice(!color,teamArea.getMorale(!color));
                emit specialFinishSIG(args);
                int n=1;
                emit loseMoraleHeChengSIG(0,&n,currentPlayer);
                this->checkEnd();
            }
            else if(bat.CardID == EXTRACT)
            {
                coder.notice("执行【提炼】");
                int color = this->currentPlayer->getColor();
                teamArea.setGem(color,teamArea.getGem(color) - bat.infor1);
                teamArea.setCrystal(color,teamArea.getCrystal(color) - bat.infor2);
                currentPlayer->setGem(currentPlayer->getGem()+bat.infor1);
                currentPlayer->setCrystal(currentPlayer->getCrystal()+bat.infor2);                
                coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));
                coder.energyNotice(currentPlayer->getID(),currentPlayer->getGem(),currentPlayer->getCrystal());
                emit specialFinishSIG(args);
            }
            else {
                QList<void*> args;
                args << &bat;
                emit skillSpecial(args);
            }
            acted = true;
            this->acted(SPECIAL);
        }
    }
    emit turnEndPhaseSIG(currentPlayer);
}

void BackgroundEngine::turnEndPhase()
{
    //emit turnEndPhaseSIG();

}

//????”±?°ê?·ń?éò???????è?4ê??a”è?é????
//????ê???ê?ó????????ü??óéclientíê?é
bool BackgroundEngine::canAct()
{
    if(this->currentPlayer->getHandCards().length() <= this->currentPlayer->getHandCardMax() - 3)
        return true;
    for(int i = 0;i < this->currentPlayer->getHandCards().length();i++)
    {
        if(this->currentPlayer->getHandCards().at(i)->getName() != tr("ê??a"))
            return true;
    }
    return false;
}

void BackgroundEngine::reDraw()
{
    coder.unactionalNotice(this->currentPlayer->getID());
    int cardNum = this->currentPlayer->getHandCards().length();
    coder.discardNotice(this->currentPlayer->getID(),cardNum,"y",this->currentPlayer->getHandCards());
    this->currentPlayer->removeHandCards(this->getCurrentPlayer()->getHandCards(),true,true);    
    this->drawCards(cardNum,0,this->currentPlayer);
}

//éè???ù???§??”???í??ò???°
void BackgroundEngine::effectApply(CardEntity* card,PlayerEntity* user,PlayerEntity* dst)
{
    QList<CardEntity*> cards;
    cards << card;
    int userID=user->getID();
    int dstID=dst->getID();

    card->setSrcUser(userID);
    card->setOwner(dstID);
    card->setPlace(EFFECT);
    user->removeHandCards(cards,true,false);
    coder.moveCardNotice(1,cards,userID,HAND,dstID,EFFECT);
    dst->addBasicEffect(card);
    if(card->getName()==tr("ê??ü")||card->getSpecialityList().contains(tr("ììê?????")))
        emit usedShield(userID);

}

//"ê?ó?"????ê±”?ó?????ê?
//”ú??????ê?±í??????ê?ó??óê?·ńá??ú??é????ù???§????
void BackgroundEngine::useCard(QList<CardEntity*> cards,PlayerEntity* user,PlayerEntity* dst,bool stay,int realCard)
{    
    coder.useCardNotice(cards,(dst == NULL)?-1:dst->getID(),user->getID(),realCard);
    if(!realCard)
        return;
    if(!stay)
        user->removeHandCards(cards,true);
    else
    {
        this->effectApply(cards[0],user,dst);
    }    
}

void BackgroundEngine::useMagicCard(int cardID, int srcID, int dstID)
{
    CardEntity *magicCard = getCardByID(cardID);
    PlayerEntity *srcPlayer = this->getPlayerByID(srcID);
    PlayerEntity *dstPlayer = this->getPlayerByID(dstID);
    QList<CardEntity*> cards;
    cards << magicCard;


    if(magicCard->getMagicName() == POISONCARD)
    {   
        this->useCard(cards,srcPlayer,dstPlayer,true);

    }
    else if(magicCard->getMagicName() == SHIELDCARD)
    {
        this->useCard(cards,srcPlayer,dstPlayer,true);

    }
    else if(magicCard->getMagicName() == WEAKCARD)
    {
        this->useCard(cards,srcPlayer,dstPlayer,true);

    }
    else if(magicCard->getMagicName() == MISSILECARD)
    {
        this->useCard(cards,srcPlayer,dstPlayer);
        this->missileProcess(magicCard,srcID,dstID);
    }
}

//ò?????ê±???á??ê?
void BackgroundEngine::timeLine1(CardEntity* attackCard,PlayerEntity* src,PlayerEntity* dst,bool isActiveAttack)
{   
    Harm harm = getHarmFromCard(attackCard);
    int attackType = (attackCard->getElement() == "darkness")?NOREPLY:NORMAL;
    QList<void*> args;
    args << src;
    args << dst;
    args << &harm;
    args << attackCard;
    args << &isActiveAttack;
    args << &attackType;
    emit timeLine1ProSIG(args);//??????ó?óú?úé???ê±???á???°éú?§”????ü
    emit timeLine1SIG(args);
    timeLine2(attackCard,src,dst,isActiveAttack,attackType,harm);
}

void BackgroundEngine::timeLine2(CardEntity* harmCard,PlayerEntity* src,PlayerEntity* dst,bool isActiveAttack,int attackType,Harm harm)
{
    coder.askForReBat(attackType,harmCard->getID(),dst->getID(),src->getID());
    //”±????ê?±???ê±?????óêü?í?§????ó?
    //emit askForReply(dst->getHandCards(),element,dst->getID());
    BatInfor temp;
    bool checkShield=true;
    if(attackType != NOMISS)
    {
        //???·?????????útemp????è?ó??????ò±?????ó???”?????è?”?”????ò±?????ê??a?òê??ü
        temp = messageBuffer::readBatInfor();
    }
    else
    {
        temp.reply = HIT;
        checkShield=false;
    }

    if(temp.reply == REPLYBATTLE)
    {
        //ó???
        //????·?é?????”?ê±?ú??ò?éìè?
        CardEntity* usedCard = getCardByID(temp.CardID);
        QList<CardEntity*> use;
        use<<usedCard;
        this->useCard(use,getPlayerByID(temp.srcID),getPlayerByID(temp.dstID));

       // coder.hitNotice(BLOCKED,isActiveAttack,dst->getID(),src->getID());
        QList<void*> args;
        args << src;
        args << dst;
        args << &harm;
        args << harmCard;
        args << &isActiveAttack;
        emit timeLine2missedSIG(args);
        this->timeLine1(getCardByID(temp.CardID),getPlayerByID(temp.srcID),getPlayerByID(temp.dstID),false);
    }
    else if(temp.reply == BLOCKED)
    {
        //”?”?
        CardEntity* usedCard = getCardByID(temp.CardID);

        if(usedCard->getPlace() == HAND)
        {
            //ê??a
            QList<CardEntity*> use;
            use<<usedCard;
            this->useCard(use,getPlayerByID(temp.srcID));
        }
      //  coder.hitNotice(BLOCKED,isActiveAttack,dst->getID(),src->getID());
        QList<void*> args;
        args << src;
        args << dst;
        args << &harm;
        args << harmCard;
        args << &isActiveAttack;
        emit timeLine2missedSIG(args);

    }
    else if(temp.reply == HIT)
    {    
        //?ü??”??é??

        QList<void*> args;
        args << src;
        args << dst;
        args << &checkShield;
        emit shieldSIG(args);
        for(int i = 0;i < dst->getBasicEffect().size()&& checkShield;i++)
        {
            //?ì?éê?·ńó?ê??ü                        
            if(dst->getBasicEffect().at(i)->getMagicName() == SHIELDCARD || dst->getBasicEffect().at(i)->getSpecialityList().contains(tr("ììê?????")))
            {
                coder.shieldNotic(dst->getID());
                dst->removeBasicEffect(dst->getBasicEffect()[i]);

             //   coder.hitNotice(BLOCKED,isActiveAttack,dst->getID(),src->getID());
                args.clear();
                args << src;
                args << dst;
                args << &harm;
                args << harmCard;
                args << &isActiveAttack;
                emit timeLine2missedSIG(args);
                return;
            }
        }

        coder.hitNotice(HIT,isActiveAttack,dst->getID(),src->getID());
        args.clear();
        args << src;
        args << dst;
        args << &harm;
        args << harmCard;
        args << &isActiveAttack;
        emit timeLine2hitSIG(args);

        int color = src->getColor();
        //???ó??ê?
        if((teamArea.getCrystal(color) + teamArea.getGem(color)) < 5)
        {
            if(isActiveAttack)
                teamArea.setGem(color,teamArea.getGem(color) + 1);
            else
                teamArea.setCrystal(color,teamArea.getCrystal(color) + 1);
            coder.stoneNotice(color,teamArea.getGem(color),teamArea.getCrystal(color));

        }

        this->timeLine3(harm,src,dst);
    }
}
//?ó???????ńè?????é???
Harm BackgroundEngine::getHarmFromCard(CardEntity* card)
{
    Harm harm;
    if(card->getType() == "attack")
    {
        //harm.element = card->getElement();
        harm.harmPoint = 2;
        harm.type = ATTACK;
    }
    return harm;
}

void BackgroundEngine::timeLine3(Harm harm, PlayerEntity *src, PlayerEntity *dst,QString magicReason)
{
    QList<void*> arg;
    arg << src;
    arg << dst;
    arg << &harm;

    emit timeLine3SIG(arg);
    if(harm.type == ATTACK)
        coder.attackHurtNotice(dst->getID(),harm.harmPoint,src->getID());
    else if(harm.type == MAGIC)
        coder.magicHurtNotice(dst->getID(),harm.harmPoint,src->getID(),magicReason);
    timeLine4(harm,src,dst);
}

void BackgroundEngine::timeLine4(Harm harm,PlayerEntity *src,PlayerEntity *dst)
{
    int crossUsed = 0;
    int crossAvailable = dst->getCrossNum();
    emit askForHeal(harm,src,dst,&crossAvailable);
    if(crossAvailable != 0)
    {
        coder.askForCross(dst->getID(),harm.harmPoint,harm.type, crossAvailable);
        crossUsed = messageBuffer::readInfor();
    }

    timeLine5(harm,src,dst,crossUsed);
}

void BackgroundEngine::timeLine5(Harm harm,PlayerEntity *src,PlayerEntity *dst,int cross)
{
    if(cross>0)
    {
        harm.harmPoint -= cross;
        int newCross=dst->getCrossNum()-cross;
        dst->setCrossNum(newCross);
        coder.crossChangeNotice(dst->getID(),newCross);
    }
    if(harm.harmPoint == 0)
        return;
    //emit timeLine5SIG();

    timeLine6(harm,src,dst);
}

void BackgroundEngine::timeLine6(Harm harm,PlayerEntity *src,PlayerEntity *dst)
{
    QList<void*> arg;
    arg << src;
    arg << dst;
    arg << &harm;
    emit timeLine6SIG(arg);
    if(harm.harmPoint == 0)
        return;
    this->drawCards(harm.harmPoint,harm.type,dst);
    if(!playing)
        return;
    emit timeLine6DrawedSIG(arg);
}
//?é?ò??ó?ID”?í??ò
PlayerEntity* BackgroundEngine::getPlayerByID(int ID)
{
    PlayerEntity* one;
    //????????ió?????óúê”?êí??òê?
    for(int i = 0;i < this->playerNum;i++)
    {
        one = this->playerList.at(i);
        if(ID == one->getID())
            return one;      
    }
    return NULL;
}
//?aê?ó?
void BackgroundEngine::showTest()
{
    PlayerEntity* player;
    char temp[500];
    QString tempStr;
    for(int i = 0;i < this->getPlayerNum();i++)
    {
        player = this->playerList.at(i);
        sprintf(temp,"player%d:\n",player->getID());
        tempStr.append(temp);

        for(int j = 0;j < player->getHandCardNum();j++)
        {

            tempStr.append(player->getHandCards().at(j)->getName());
            tempStr += QString::number(player->getHandCards().at(j)->getID());
            tempStr += " ";
        }
        tempStr += "\n";
    }

    sprintf(temp,"red team:%d\n",teamArea.getMorale(RED));
    tempStr.append(temp);
    sprintf(temp,"blue team:%d\n",teamArea.getMorale(BLUE));
    tempStr.append(temp);
    emit this->toInforDisplay(tempStr);
}
//?ńè?é??ò
PlayerEntity* BackgroundEngine::getFront(PlayerEntity* player)
{
    int seat;
    seat = player->getSeatNum();
    seat--;
    if(seat < 0)
        seat = this->playerList.size() - 1;
    return this->playerList[seat];
}
//?§”???àí
void BackgroundEngine::missileProcess(CardEntity* card,int src,int dst)
{
    bool rightOrder;
    BatInfor reply;
    PlayerEntity* nextOpponent;

    //è·????”?·??ò
    nextOpponent = this->getCurrentPlayer()->getNext();
    while(nextOpponent->getColor() == this->currentPlayer->getColor())
        nextOpponent = nextOpponent->getNext();
    if(nextOpponent->getID() == dst)
        rightOrder = true;
    else
        rightOrder = false;

    bool passed[6];
    for(int i = 0;i < this->playerList.size();i++)
    {
        passed[i] = false;
    }
    int missilePoint = 2;

    QList<CardEntity*> cards;


    do
    {
        cards.clear();
        //?§”???”?”????ò
        missilePass(rightOrder,dst,src,passed,missilePoint);

        //?áè?ó??§????
        reply = messageBuffer::readBatInfor();

        if(reply.reply == 0)
        {
            //?ì????”?
            src = dst;
            dst = reply.dstID;
            missilePoint++;
            cards << getCardByID(reply.CardID);
            this->useCard(cards,getPlayerByID(src),getPlayerByID(dst),false);
            continue;
        }
        else if(reply.reply == 1)
        {
            //ê??a
            cards << getCardByID(reply.CardID);
            this->useCard(cards,getPlayerByID(dst));
            break;
        }
        else if(reply.reply == 2)
        {
            //??ó???
            PlayerEntity* dstPlayer = getPlayerByID(dst);
            bool shieldBlocked = false;
            //?ì?éê??ü
            for(int i = 0;i < dstPlayer->getBasicEffect().size();i++)
            {
                if(dstPlayer->getBasicEffect()[i]->getMagicName() == SHIELDCARD||dstPlayer->getBasicEffect().at(i)->getSpecialityList().contains(tr("ììê?????")))
                {
                    coder.shieldNotic(dst);
                    dstPlayer->removeBasicEffect(dstPlayer->getBasicEffect()[i]);
                    shieldBlocked = true;
                    break;
                }
            }
            if(shieldBlocked)
                break;
            Harm missileHurt;
            missileHurt.harmPoint = missilePoint;
            missileHurt.type = MAGIC;
            //??ê??ü,?ì?éé???
            this->timeLine3(missileHurt,getPlayerByID(src),dstPlayer,"魔弹");
            break;
        }
        else if(reply.reply == 802)
        {
            //?ì????”?
            src = dst;
            dst = reply.dstID;
            missilePoint++;
            cards << getCardByID(reply.CardID);
            useCard(cards,getPlayerByID(src),getPlayerByID(dst));
            coder.notice("魔导师发动【魔弹融合】");
            continue;
        }

    }while(1);

}


void BackgroundEngine::missilePass(bool rightOrder,int dst,int src,bool* passed,int missilePoint)
{
    int i;
    PlayerEntity* srcPlayer = getPlayerByID(src);
    PlayerEntity* dstPlayer = getPlayerByID(dst);
    PlayerEntity* next = dstPlayer;
    passed[srcPlayer->getSeatNum()] = true;

    for(i = 0;i < this->playerList.size();i++)
    {
        if(!*(passed+i) && (i != dstPlayer->getSeatNum()))
            break;
    }
    if(i == this->playerList.size())
    {
        for(int j = 0;j < this->playerList.size();j++)
            *(passed + j) = false;
    }

    if(rightOrder)
    {   
        while((next->getColor() == dstPlayer->getColor()) || *(passed + next->getSeatNum()))
            next = next->getNext();

        coder.askForMissile(dst,src,missilePoint,next->getID());
    }
    else
    {
        while((next->getColor() == dstPlayer->getColor()) || passed[next->getSeatNum()])
            next = this->getFront(next);

        coder.askForMissile(dst,src,missilePoint,next->getID());
    }
}
//?óê??????éò???????”??ú????”?????
void BackgroundEngine::toDiscardPileSLOT(QList<CardEntity*> cards,bool show)
{
    for(int i = 0;i < cards.size();i++)
    {
        if(show)
            this->discardPile << cards.at(i);
        else
            this->discardPileCovered << cards.at(i);
    }
}

//将某牌从原位置移走。不发送任何信息给client。

void BackgroundEngine::moveCardFrom(CardEntity* card)
{
    bool test;
    switch(card->getPlace())
    {
    case PILE:
        this->pile.removeOne(card);
        break;
    case DISCARDPILE:
        this->discardPile.removeOne(card);
        break;
    case DISCARDPILECOVERED:
        this->discardPileCovered.removeOne(card);
        break;
    case HAND:
        test = this->getPlayerByID(card->getOwner())->removeOneHandCard(card);
        card->setOwner(OWNERLESS);
        break;
    case COVERED:
        this->getPlayerByID(card->getOwner())->removeOneCoverCard(card);
        card->setOwner(OWNERLESS);
        break;
    case EFFECT:
        this->getPlayerByID(card->getOwner())->removeOneEffectCard(card);
        card->setOwner(OWNERLESS);
        card->setSrcUser(OWNERLESS);
        break;
    }
}

//将某牌移入某角色盖牌区。不发送任何信息给client。要求该卡牌应先从其他位置被移除。

void BackgroundEngine::moveCardToCover(CardEntity* card,int dstPlayerID)
{
    card->setOwner(dstPlayerID);
    card->setPlace(COVERED);
    QList<CardEntity*> cards;
    cards << card;
    this->getPlayerByID(dstPlayerID)->addCardsToCover(cards);

}


//将某牌从盖牌区移到弃牌堆。不发送任何信息给client。

void BackgroundEngine::moveCardFromCoverToDiscard(CardEntity* card,bool show)
{
    this->getPlayerByID(card->getOwner())->removeOneCoverCard(card);
    card->setOwner(OWNERLESS);
    if(show)
    {
        card->setPlace(DISCARDPILE);
        this->discardPile << card;
    }
    else
    {
        card->setPlace(DISCARDPILECOVERED);
        this->discardPileCovered << card;
    }
}

