#include "KuangZhan.h"

KuangZhan::KuangZhan()
{
    makeConnection();
}

void KuangZhan::SiLie()
{
    state=36;
    tipArea->setMsg(tr("是否发动撕裂？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void KuangZhan::XueXingPaoXiao()
{
    state=36;
    tipArea->setMsg(tr("是否发动血腥咆哮？"));
    decisionArea->enable(0);
    decisionArea->enable(1);

}

void KuangZhan::XueYingKuangDao()
{
    state=36;
    tipArea->setMsg(tr("是否发动血影狂刀？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void KuangZhan::onOkClicked()
{
    Role::onOkClicked();
    QString command;
    switch(state)
    {
//撕裂询问
//血影咆哮询问
//血影狂刀询问
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void KuangZhan::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//撕裂询问
//血影咆哮询问
//血影狂刀询问
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void KuangZhan::decipher(QString command)
{
    Role::decipher(command);
    QStringList arg=command.split(';');
    int targetID;
    QString flag;

    switch (arg[0].toInt())
    {
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID==myID)
        {
            gui->setEnable(1);
            if(flag==tr("撕裂"))
                SiLie();
            else if(flag==tr("血腥咆哮"))
                XueXingPaoXiao();
            else if(flag==tr("血影狂刀"))
                XueYingKuangDao();
        }
        break;

    }
}
