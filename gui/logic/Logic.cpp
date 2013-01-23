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

#define LOGINPERMIT 1
#define GAMESTART 2

Logic* logic=NULL;
Logic::Logic(QObject *parent) :
    QObject(parent)
{
    count=0;
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
    case 11:
        new YuanSu;
        break;
    case 12:
        new MaoXian;
        break;
    case 14:
        new ZhongCai;
        break;
    case 21:
        new YongZhe;
        break;
    }
}

void Logic::getCommand(QString command)
{
    QStringList arg=command.split(';');
    int myID,playerMax,targetID,roleID;
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
        dataInterface->getPlayerList().at(targetID)->setRole(roleID);
        count++;
        if (dataInterface->getID()==targetID)            
            setMyRole(roleID);
        if(count==6)
            disconnect(this);
        break;
    }
}
