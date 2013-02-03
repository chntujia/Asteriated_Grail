#include "SiLing.h"
#include <QApplication>
SiLing::SiLing()
{
    makeConnection();
    setMyRole(this);
    Button *wenYi, *siWangZhiChu, *muBeiYunLuo;
    wenYi = new Button(3,tr("瘟疫"));
    buttonArea->addButton(wenYi);
    connect(wenYi,SIGNAL(buttonSelected(int)),this,SLOT(WenYi()));

    siWangZhiChu = new Button(4,tr("死亡之触"));
    buttonArea->addButton(siWangZhiChu);
    connect(siWangZhiChu,SIGNAL(buttonSelected(int)),this,SLOT(SiWangZhiChu()));

    muBeiYunLuo = new Button(5,tr("墓碑陨落"));
    buttonArea->addButton(muBeiYunLuo);
    connect(muBeiYunLuo,SIGNAL(buttonSelected(int)),this,SLOT(MuBeiYunLuo()));
}

void SiLing::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    if(handArea->checkElement("earth"))
        buttonArea->enable(3);
    bool siChu = false;
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                siChu = true;
                break;
            }
    if(siChu&&myself->getCrossNum()>1)
        buttonArea->enable(4);
    if(myself->getGem()>0)
        buttonArea->enable(5);
}

void SiLing::BuXiu()
{
    state = 36;
    tipArea->setMsg(tr("是否发动不朽？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void SiLing::WenYi()
{
    state = 1302;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("earth");
}

void SiLing::SiWangZhiChu()
{
    state = 1303;
    Player* myself=dataInterface->getMyself();
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2,7);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();

    tipArea->setMsg("请选择移除的治疗，最少为2");
    int cross = myself->getCrossNum();
    for(;cross>1;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->showBox();
}

void SiLing::MuBeiYunLuo()
{
    state = 1304;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void SiLing::cardAnalyse()
{
    Role::cardAnalyse();
    bool siChu = true;
    QList<Card*> selectedCards=handArea->getSelectedCards();
    switch (state)
    {
    case 1302:
        decisionArea->enable(0);
        break;
    case 1303:
        foreach(Card*ptr,selectedCards)
            if(ptr->getElement()!=selectedCards[0]->getElement()){
                playerArea->disableAll();
                decisionArea->disable(0);
                siChu = false;
                break;
            }
        if(siChu)
            playerArea->enableAll();
        break;
    }
}

void SiLing::onOkClicked()
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
    case 1302:
        command = "1302;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        command+=cardID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1303:
        command = "1303;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        text=tipArea->getBoxCurrentText();
        command+=targetID+";"+sourceID+";"+text+";";
        command+=QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+":";
            dataInterface->removeHandCard(ptr);
        }
        command+=";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1304:
        command = "1304;";
        sourceID=QString::number(myID);
        command+=sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void SiLing::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
    case 1302:
    case 1303:
    case 1304:
        normal();
        break;
    }
}

void SiLing::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("不朽"))
        BuXiu();
}

void SiLing::cure(int cross, int harmPoint, int type)
{
    int min=cross<harmPoint?cross:harmPoint;
    QString msg=tr("你受到")+QString::number(harmPoint)+tr("点");
    if(type==1)
        msg+=tr("攻击");
    else
        msg+=tr("法术");
    msg+=tr("伤害，要使用多少个治疗抵御之？");
    if(type==1)
        min = 0;
    state=9;
    decisionArea->enable(0);
    decisionArea->enable(1);

    tipArea->setMsg(msg);
    for(;min>=0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
    QApplication::alert((QWidget*)playerArea->window());
}
