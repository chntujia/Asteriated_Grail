#include "QiDao.h"

QiDao::QiDao()
{
    makeConnection();
    setMyRole(this);

    Button *weiLi,*xunJie,*guangHui,*qiHei;
    weiLi=new Button(3,tr("ÍþÁ¦´Í¸£"));
    buttonArea->addButton(weiLi);
    connect(weiLi,SIGNAL(buttonSelected(int)),this,SLOT(WeiLiCiFu()));

    xunJie=new Button(4,tr("Ñ¸½Ý´Í¸£"));
    buttonArea->addButton(xunJie);
    connect(xunJie,SIGNAL(buttonSelected(int)),this,SLOT(XunJieCiFu()));

    guangHui=new Button(5,tr("¹â»ÔÐÅÑö"));
    buttonArea->addButton(guangHui);
    connect(guangHui,SIGNAL(buttonSelected(int)),this,SLOT(GuangHuiXinYang()));

    qiHei=new Button(6,tr("ÆáºÚÐÅÑö"));
    buttonArea->addButton(qiHei);
    connect(qiHei,SIGNAL(buttonSelected(int)),this,SLOT(QiHeiXinYang()));
}

void QiDao::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(tr("ÍþÁ¦´Í¸£")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(tr("Ñ¸½Ý´Í¸£")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
    unactionalCheck();
}

void QiDao::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(tr("ÍþÁ¦´Í¸£")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(tr("Ñ¸½Ý´Í¸£")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
}

void QiDao::QiDong()
{
    state=1601;
    gui->reset();
    tipArea->setMsg(tr("ÊÇ·ñ·¢¶¯Æíµ»£¿"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
    }
    if(flag)
        decisionArea->enable(0);
}

void QiDao::WeiLiCiFu()
{
    state=1602;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(tr("ÍþÁ¦´Í¸£"));
}

void QiDao::XunJieCiFu()
{
    state=1603;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(tr("Ñ¸½Ý´Í¸£"));
}

void QiDao::GuangHuiXinYang()
{
    state=1604;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    int howMany=handArea->getHandCardItems().size();
    howMany=2<howMany?2:howMany;
    handArea->setQuota(howMany);
    handArea->enableAll();
    if(howMany==0)
        playerArea->enableMate();
    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::QiHeiXinYang()
{
    state=1605;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Player*>players=dataInterface->getPlayerList();
    switch (state)
    {
//ÍþÁ¦´Í¸£
    case 1602:
        playerArea->enableMate();
        for(int i=0;i<players.size();i++)
           if(players[i]->checkStatus(4))
               playerArea->disablePlayerItem(i);
        break;
//Ñ¸½Ý´Í¸£
    case 1603:
        playerArea->enableMate();
        for(int i=0;i<players.size();i++)
           if(players[i]->checkStatus(5))
               playerArea->disablePlayerItem(i);
        break;
//¹â»ÔÐÅÑö
    case 1604:
        playerArea->enableMate();
        break;
    }
}

void QiDao::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//¶îÍâÐÐ¶¯Ñ¯ÎÊ
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            onceUsed=true;
            emit sendCommand("1606;"+QString::number(myID)+";");
            magicAction();

        }
        break;
//Æíµ»
    case 1601:
        command="1601;1;";
        start = true;
        emit sendCommand(command);
        gui->reset();
        break;
//ÍþÁ¦´Í¸£
    case 1602:
        command="1602;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
//Ñ¸½Ý´Í¸£
    case 1603:
        command="1603;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
//¹â»ÔÐÅÑö
    case 1604:
        command="1604;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";"+QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        for(int i=0;i<(2-selectedCards.size());i++)
            command+="-1;";
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
//ÆáºÚÐÅÑö
    case 1605:
        command="1605;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void QiDao::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//Æíµ»
    case 1601:
        command="1601;0;;";
        emit sendCommand(command);
        gui->reset();
        break;
//ÍþÁ¦´Í¸£
    case 1602:
//Ñ¸½Ý´Í¸£
    case 1603:
//¹â»ÔÐÅÑö
    case 1604:
//ÆáºÚÐÅÑö
    case 1605:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
        break;
    }
}
void QiDao::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("Æíµ»"))
        QiDong();
}
void QiDao::additionalAction()
{
    Role::additionalAction();
    if(usedMagic && dataInterface->getMyself()->getEnergy()>0 && !onceUsed)
        tipArea->addBoxItem(tr("1.·¨Á¦³±Ï«"));
}
