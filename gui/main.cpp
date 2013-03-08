#include <QtGui>
#include <QApplication>
#include "widget/Animation.h"
#include "widget/RoomView.h"
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "client/ClientUI.h"
#include "logic/Logic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("simsun", 9));
    dataInterface=new DataInterface;
    logic=new Logic;

/**/ClientUI c;

    if(c.exec()==QDialog::Accepted)
    {

        animation = new Animation;
        gui=new GUI;
        RoomView view;

        animation->setRoomScene(view.getScene());

        view.show();
//GUI初始化完成
        /**/
        logic->readyToStart();

/*
        dataInterface->getPlayerList().at(4)->setGem(1);
        dataInterface->getPlayerList().at(3)->setCrystal(1);
        dataInterface->getPlayerList().at(3)->setGem(2);
        dataInterface->getPlayerList().at(0)->setCrystal(3);
        dataInterface->getPlayerList().at(1)->setGem(4);
        dataInterface->getPlayerList().at(3)->setCrossNum(23);
        dataInterface->addHandCard(dataInterface->getCard(45));
        dataInterface->addHandCard(dataInterface->getCard(84));
        dataInterface->addHandCard(dataInterface->getCard(75));
        dataInterface->addHandCard(dataInterface->getCard(2));
        dataInterface->addHandCard(dataInterface->getCard(3));
        dataInterface->addHandCard(dataInterface->getCard(4));
        dataInterface->addHandCard(dataInterface->getCard(5));
        gui->logAppend("text");
        gui->logAppend("text");
        gui->logAppend("text");
        gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");gui->logAppend("text");
        dataInterface->getPlayerList().at(3)->addBasicStatus(1,dataInterface->getCard(0));
        dataInterface->getPlayerList().at(2)->addBasicStatus(2,dataInterface->getCard(3));
        dataInterface->getPlayerList().at(3)->addBasicStatus(1,dataInterface->getCard(4));
        dataInterface->getPlayerList().at(5)->addBasicStatus(0,dataInterface->getCard(45));
        dataInterface->getPlayerList().at(1)->removeBasicStatus(dataInterface->getCard(3));
*/
        return a.exec();
    }
    else return 0;
}
