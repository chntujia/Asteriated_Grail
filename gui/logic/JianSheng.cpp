#include "JianSheng.h"

JianSheng::JianSheng()
{
    makeConnection();
}

void JianSheng::LieFengJi()
{
    state=36;
    tipArea->setMsg(tr("是否发动烈风技？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianSheng::LianXuJi()
{
    //先借用基类的额外攻击行动状态
    state=10;
    onceUsed=true;
    gui->reset();
    handArea->setQuota(1);
    handArea->enableElement("wind");
    handArea->disableMagic();
    playerArea->setQuota(1);
    decisionArea->enable(3);
}

void JianSheng::JianYing(){
    attackAction();
    onceUsed2=true;
}

void JianSheng::onOkClicked()
{
    QList<Card*> selectedCards=handArea->getSelectedCards();
    QList<Player*> selectedPlayers=playerArea->getSelectedPlayers();
    Role::onOkClicked();
    QString command,text,cardID,targetID,sourceID;

    switch(state)
    {
//NORMALACTION
    case 1:
//追加行动
    case 10:
    case 12:
        if(selectedCards[0]->getSpecialityList().contains(tr("疾风技")))
            JiFengJi++;
        break;
//烈风技询问
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();        
        switch (text[0].digitValue()){
        case 1:
            JiFengJi--;
            actions.removeOne(tr("1.攻击行动（疾风技）"));
            emit sendCommand("103;"+QString::number(myID)+";");
            attackAction();            
            break;
        case 2:
            actions.removeOne(tr("2.连续技"));
            emit sendCommand("101;"+QString::number(myID)+";");
            LianXuJi();
            break;
        case 3:
            actions.removeOne(tr("3.剑影"));
            emit sendCommand("102;"+QString::number(myID)+";");
            JianYing();
            break;
        }
        break;
    }
}

void JianSheng::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//烈风技询问
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void JianSheng::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch (arg[0].toInt())
    {
//回合开始
    case 3:
        targetID=arg[1].toInt();
        if(targetID==myID){
            onceUsed2=false;
            JiFengJi=0;
        }
        break;
//烈风技询问
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("烈风技"))
                LieFengJi();
        }
        break;
//额外行动询问
    case 42:
        targetID=arg[1].toInt();
        if(targetID==myID)
        {
            QList<Card*> handcards=dataInterface->getHandCards();

            if(JiFengJi>0 && !actions.contains(tr("1.攻击行动（疾风技）"))){
                actions.append(tr("1.攻击行动（疾风技）"));
            }
            if(!onceUsed&&usedAttack && !actions.contains(tr("2.连续技")))
            {
                foreach(Card*ptr,handcards)
                    if(ptr->getElement()=="wind" && ptr->getType()=="attack")
                    {
                        actions.append(tr("2.连续技"));
                        break;
                    }
            }
            if(!onceUsed2 && dataInterface->getMyself()->getEnergy()>0 &&usedAttack &&!actions.contains(tr("3.剑影"))){
                actions.append(tr("3.剑影"));
            }
            if(actions.size()==0)
                decisionArea->disable(0);
            foreach(QString ptr,actions)
                tipArea->addBoxItem(ptr);
            tipArea->showBox();

            state=42;

        }
        break;
    }
}
