#include "logic/Logic.h"
#include "data/DataInterface.h"
#include "Role.h"
#include "JianSheng.h"
#include "KuangZhan.h"
#include "GongNv.h"
#include "FengYin.h"
#include "AnSha.h"
#include "ShengNv.h"
#include "TianShi.h"
#include "MoDao.h"
#include "MoJian.h"
#include "YuanSu.h"
#include "MaoXian.h"
#include "YongZhe.h"
#include "ZhongCai.h"
#include "ShengQiang.h"
#include "QiDao.h"
#include "ShenGuan.h"
#include "SiLing.h"

#define LOGINPERMIT 1
#define GAMESTART 2

Logic* logic=NULL;
Logic::Logic(QObject *parent) :
    QObject(parent)
{
    count=0;
    hasShownRole=false;
    memset(roles,-1,sizeof(roles));
}
void Logic::readyToStart()
{
    emit sendCommand("16;");
}
void Logic::setMyRole(int roleID)
{
    switch(roleID)
    {
    case 1:
        new JianSheng;
        break;
    case 2:
        new KuangZhan;
        break;
    case 3:
        new GongNv;
        break;
    case 4:
        new FengYin;
        break;
    case 5:
        new AnSha;
        break;
    case 6:
        new ShengNv;
        break;
    case 7:
        new TianShi;
        break;
    case 8:
        new MoDao;
        break;
    case 9:
        new MoJian;
        break;
    case 10:
        new ShengQiang;
        break;
    case 11:
        new YuanSu;
        break;
    case 12:
        new MaoXian;
        break;
    case 13:
        new SiLing;
        break;
    case 14:
        new ZhongCai;
        break;
    case 15:
        new ShenGuan;
        break;
    case 16:
        new QiDao;
        break;
    case 21:
        new YongZhe;
        break;
    }
}

void Logic::getCommand(QString command)
{
    QStringList arg=command.split(';');
    TipArea *tipArea;
    DecisionArea* decisionArea;
    int playerMax,targetID,roleID,howMany;

    switch (arg[0].toInt())
    {
    case LOGINPERMIT:
        myID=arg[1].toInt();
        dataInterface->setID(myID);
        break;
    case GAMESTART:
        playerMax=arg[1].count()/2;
        dataInterface->setPlayerMax(playerMax);
        dataInterface->initPlayerList(arg[1]);
        if (playerMax==8)
            dataInterface->initTeam(18);
        else
            dataInterface->initTeam(15);
//dataInterface初始化完成
        emit gameStart();
        break;
//角色宣告
    case 37:
        targetID=arg[1].toInt();
        roleID=arg[2].toInt();
        roles[targetID]=roleID;
        if(arg[3]=="1"){
            dataInterface->getPlayerList().at(targetID)->setRole(roleID);
            gui->getPlayerArea()->update();
            hasShownRole=true;
        }
        else if(targetID==myID){
            dataInterface->getPlayerList().at(targetID)->setRole(roleID);
            setMyRole(roleID);
            gui->getPlayerArea()->update();
        }
        count++;
        if(count==6)
        {
            disconnect(getClient(),0,this,0);
            if(!hasShownRole){
                for(int i=0;i<dataInterface->getPlayerMax();i++)
                    dataInterface->getPlayerList().at(i)->setRole(roles[i]);
                gui->getPlayerArea()->update();
            }
        }
        break;
//31选择角色
    case 46:
        state=46;
        tipArea=gui->getTipArea();
        decisionArea=gui->getDecisionArea();
        tipArea->reset();
        connect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        howMany=arg[1].toInt();
        for(int i=0;i<howMany;i++){
            roleID=arg[2+i].toInt();
            tipArea->addBoxItem(QString::number(roleID)+"."+dataInterface->getRoleList().at(roleID));
        }
        tipArea->setMsg(tr("请选择角色："));
        tipArea->showBox();
        decisionArea->enable(0);
        break;
    }
}
void Logic::onOkClicked()
{
    switch(state)
    {
    case 46:
        TipArea *tipArea=gui->getTipArea();
        QStringList chosen=tipArea->getBoxCurrentText().split(".");
        emit sendCommand("47;"+chosen[0]+";");
        disconnect(gui->getDecisionArea(),SIGNAL(okClicked()),this,SLOT(onOkClicked()));;
        gui->reset();
        break;
    }


}
