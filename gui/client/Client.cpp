#include <QThread>
#include "client/Client.h"
#include "widget/GUI.h"
#include "logic/Logic.h"
Client::Client()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(logic,SIGNAL(gameStart()),this,SIGNAL(readyToStart()));
    connect(logic,SIGNAL(sendCommand(QString)),this,SLOT(sendMessage(QString)));
    connect(this,SIGNAL(getMessage(QString)),logic,SLOT(getCommand(QString)));
    logic->setClient(this);
}
void Client::link(QString addr,int port)
{
    abort();
    connectToHost(addr,port);
}
void Client::readMessage()
{
    QDataStream in(this);
    quint16 nextBlockSize;
    QString message;
    in.setVersion(QDataStream::Qt_5_0);
    while(1){
    if(bytesAvailable() < (int)sizeof(quint16)) return;
    in >> nextBlockSize;
    if(bytesAvailable() < nextBlockSize) return;
    //如果没有得到全部的数据，则返回，继续接收数据
    in >> message;
    //将接收到的数据存放到变量中
    emit getMessage(message);
    }
}
void Client::sendMessage(QString message)
{
    QByteArray block; //用于暂存我们要发送的数据

    QDataStream out(&block,QIODevice::WriteOnly);

    //使用数据流写入数据

    out.setVersion(QDataStream::Qt_5_0);

    //设置数据流的版本，客户端和服务器端使用的版本要相同

    out<<(quint16) 0;

    out<<message;

    out.device()->seek(0);
    out<<(quint16)(block.size() - sizeof(quint16));
    write(block);

}
