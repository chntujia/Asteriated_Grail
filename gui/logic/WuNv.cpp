#include "WuNv.h"

WuNv::WuNv()
{
    makeConnection();
    setMyRole(this);
    tongShengID = -1;
    Button *tongShengGongSi, *niLiu, *xueZhiBeiMing, *xueZhiZuZhou;
    tongShengGongSi = new Button(3,tr("同生共死"));
    buttonArea->addButton(tongShengGongSi);
    connect(tongShengGongSi,SIGNAL(buttonSelected(int)),this,SLOT(TongShengGongSi()));

    niLiu = new Button(4,tr("逆流"));
    buttonArea->addButton(niLiu);
    connect(niLiu,SIGNAL(buttonSelected(int)),this,SLOT(NiLiu()));

    xueZhiBeiMing = new Button(5,tr("血之悲鸣"));
    buttonArea->addButton(xueZhiBeiMing);
    connect(xueZhiBeiMing,SIGNAL(buttonSelected(int)),this,SLOT(XueZhiBeiMing()));

    xueZhiZuZhou = new Button(6,tr("血之诅咒"));
    buttonArea->addButton(xueZhiZuZhou);
    connect(xueZhiZuZhou,SIGNAL(buttonSelected(int)),this,SLOT(XueZhiZuZhou()));
}

void WuNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    if(tongShengID==-1)
        buttonArea->enable(3);
    if(myself->getTap())
        buttonArea->enable(4);
    if(myself->getTap()&&handArea->checkSpecility(tr("血之悲鸣")))
        buttonArea->enable(5);
    if(myself->getGem()>0)
        buttonArea->enable(6);
    unactionalCheck();
}

void WuNv::TongShengGongSi()
{
    state = 2301;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();
    playerArea->disablePlayerItem(dataInterface->getMyself()->getID());

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiAiShang()
{
    state = 2302;
    tipArea->setMsg(tr("是否发动血之哀伤？不选择目标并确定为移除【同生共死】"));

    playerArea->setQuota(0,1);
    playerArea->enableAll();
    playerArea->disablePlayerItem(dataInterface->getMyself()->getID());

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void WuNv::NiLiu()
{
    state = 2303;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);
    handArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiBeiMing()
{
    state = 2304;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg("请选择【血之悲鸣】伤害");
    for(int i =3;i>0;i--)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();

    handArea->setQuota(1);
    handArea->enableSpecility(tr("血之悲鸣"));
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiZuZhou()
{
    state = 2305;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>2)
        handArea->setQuota(3);
    else if(handcards.size()>0)
        handArea->setQuota(handcards.size());
    else
        playerArea->enableAll();

    handArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case 2301:
        playerArea->enableAll();
        break;
    case 2303:
        decisionArea->enable(0);
        break;
    case 2304:
        playerArea->enableAll();
        break;
    case 2305:
        playerArea->enableAll();
        break;
    }
}

void WuNv::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    static QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();
    switch(state)
    {
    case 2301:
        command = "2301;";
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        command+=targetID+";"+sourceID+";";
        tongShengID = selectedPlayers[0]->getID();
        emit sendCommand(command);
        gui->reset();
        break;
    case 2302:
        command = "2302;1;";
        start = true;
        if(selectedPlayers.size()!=0)
        {
            targetID=QString::number(selectedPlayers[0]->getID());
            tongShengID = selectedPlayers[0]->getID();
        }
        else
        {
            targetID=QString::number(-1);
            tongShengID = -1;
        }
        command+=targetID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2303:
        command = "2303;";
        sourceID=QString::number(myID);
        command+=sourceID+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(command);
        gui->reset();
        break;
    case 2304:
        command = "2304;";
        targetID=QString::number(selectedPlayers[0]->getID());
        cardID=QString::number(selectedCards[0]->getID());
        dataInterface->removeHandCard(selectedCards[0]);
        text=tipArea->getBoxCurrentText();
        sourceID=QString::number(myID);
        command+=targetID+";"+cardID+";"+text+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2305:
        command = "2305;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";"+QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+":";
            dataInterface->removeHandCard(ptr);
        }
        command+=";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void WuNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 2301:
    case 2303:
    case 2304:
    case 2305:
        normal();
        break;
    case 2302:
        command = "2302;0;-1;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void WuNv::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("血之哀伤"))
        XueZhiAiShang();
}
