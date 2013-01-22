#include "YongZhe.h"

YongZhe::YongZhe()
{
    makeConnection();

    Button *tiaoXin;
    tiaoXin=new Button(3,tr("挑衅"));
    buttonArea->addButton(tiaoXin);
    connect(tiaoXin,SIGNAL(buttonSelected(int)),this,SLOT(TiaoXin()));
}

void YongZhe::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    //挑衅
    if(myself->getToken(0)>0)
        buttonArea->enable(3);
        foreach(Player* ptr,dataInterface->getPlayerList())
            if(ptr->getSpecial(1)==1){
                buttonArea->disable(3);
                break;
            }
    unactionalCheck();
}

void YongZhe::NuHou()
{
    state=36;
    tipArea->setMsg(tr("是否发动怒吼？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::MingJingZhiShui()
{
    state=36;
    tipArea->setMsg(tr("是否发动明镜止水？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::JinDuanZhiLi()
{
    state=2102;
    tipArea->setMsg(tr("是否发动禁断之力？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::SiDou()
{
    state=36;
    tipArea->setMsg(tr("是否发动死斗？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YongZhe::TiaoXin()
{
    state=2101;

    handArea->reset();
    tipArea->reset();
    playerArea->reset();

    playerArea->enableEnemy();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YongZhe::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;

    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    //怒吼答复
    //明镜止水答复
    //死斗答复
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
    //额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0].digitValue()==1)
        {
            jinDuanZhiLi--;         
            actions.removeOne(tr("1.攻击行动（精疲力竭）"));
            attackAction();
        }
        break;
    //挑衅
    case 2101:
        command="2101;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    //禁断之力
    case 2102:
        jinDuanZhiLi++;
        command="2102;1;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void YongZhe::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 1:
    case 2101:
        normal();
        break;
    //怒吼答复
    //明镜止水答复
    //死斗答复
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    //禁断之力
    case 2102:
        if(jinDuanZhiLi>0)
            jinDuanZhiLi=0;
        command="2102;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void YongZhe::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch(arg[0].toInt())
    {
    //回合开始
    case 3:
        targetID=arg[1].toInt();
        if(targetID==myID)
            jinDuanZhiLi=0;
        break;
    //行动阶段
    case 29:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            if(flag=="0")
                normal();
        }
        break;
    //技能响应询问
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("怒吼"))
                NuHou();
            else if(flag==tr("明镜止水"))
                MingJingZhiShui();
            else if(flag==tr("禁断之力"))
                JinDuanZhiLi();
            else if(flag==tr("死斗"))
                SiDou();
        }
        break;
    //额外行动询问
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            if(jinDuanZhiLi>0)
                actions.append(tr("1.攻击行动（精疲力竭）"));

            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();
            state=42;
        }
        break;
    }
}
