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
    ClientUI c;

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
        return a.exec();
    }
    else return 0;
}
