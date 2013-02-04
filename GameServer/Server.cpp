#include "Server.h"

#include <QString>
#include <QStringList>

QSemaphore messageBuffer::cardBufferFree(10);
QSemaphore messageBuffer::cardBufferUsed(0);
int messageBuffer::cardBuffer[BUFFERSIZE];
int messageBuffer::cardBufferTop = 0;
int messageBuffer::cardBufferBottom = 0;

QSemaphore messageBuffer::inforBufferReadLocker(0);
QSemaphore messageBuffer::inforBufferWriteLocker(1);
int messageBuffer::inforBuffer;

QSemaphore messageBuffer::batBufferReadLocker(0);
QSemaphore messageBuffer::batBufferWriteLocker(1);
BatInfor messageBuffer::batBuffer;

QSemaphore messageBuffer::msgBufferReadLocker(0);
QSemaphore messageBuffer::msgBufferWriteLocker(1);
QString messageBuffer::msg;
void messageBuffer::init()
{

}

BatInfor messageBuffer::readBatInfor()
{
    messageBuffer::batBufferReadLocker.acquire();;
    BatInfor ans = messageBuffer::batBuffer;
    messageBuffer::batBufferWriteLocker.release();;
    return ans;
}

int messageBuffer::readInfor()
{
    messageBuffer::inforBufferReadLocker.acquire();;
    int ans = messageBuffer::inforBuffer;
    messageBuffer::inforBufferWriteLocker.release();;
    return ans;
}

QList<CardEntity*> messageBuffer::readCardID(int sum)
{
    QList<CardEntity*> cards;
    messageBuffer::cardBufferUsed.acquire(sum);
    for(int i = 0;i < sum;i++)
    {
        cards << getCardByID(messageBuffer::cardBuffer[messageBuffer::cardBufferBottom++]);
        if(messageBuffer::cardBufferBottom >= BUFFERSIZE)
            messageBuffer::cardBufferBottom = 0;
    }
    messageBuffer::cardBufferFree.release(sum);
    return cards;
}
QString messageBuffer::readMsg()
{
    messageBuffer::msgBufferReadLocker.acquire();;
    QString ans = messageBuffer::msg;
    messageBuffer::msgBufferWriteLocker.release();;
    return ans;
}
void messageBuffer::writeMsg(QString msg)
{
    messageBuffer::msgBufferWriteLocker.acquire();
    messageBuffer::msg = msg;
    messageBuffer::msgBufferReadLocker.release();
}
void messageBuffer::writeBatInfor(BatInfor content)
{
    messageBuffer::batBufferWriteLocker.acquire();;
    messageBuffer::batBuffer = content;
    messageBuffer::batBufferReadLocker.release();;
}

void messageBuffer::writeInfor(int content)
{
    messageBuffer::inforBufferWriteLocker.acquire();
    messageBuffer::inforBuffer = content;
    messageBuffer::inforBufferReadLocker.release();
}

void messageBuffer::writeCardInfor(QList<int> cardID)
{
    int sum = cardID.length();
    messageBuffer::cardBufferFree.acquire(sum);
    for(int i = 0;i < sum;i++)
    {
        messageBuffer::cardBuffer[messageBuffer::cardBufferTop++] = cardID.at(i);
        if(messageBuffer::cardBufferTop >= BUFFERSIZE)
            messageBuffer::cardBufferTop = 0;
    }
    messageBuffer::cardBufferUsed.release(sum);
}

ClientSocket::ClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    this->id = -1;
}
void ClientSocket::readMessage()
 {
    QDataStream in(this);
    quint16 nextBlockSize;
    QString message;

        in.setVersion(QDataStream::Qt_4_7);
        while(1)
        {
        if(bytesAvailable() < (int)sizeof(quint16)) return;
        in >> nextBlockSize;

        if(bytesAvailable() < nextBlockSize) return;

        //如果没有得到全部的数据，则返回，继续接收数据

        in >> message;

        emit getMessage(id,message);
        }
        //将接收到的数据存放到变量中
 }

Server::Server(QObject *parent,bool isIPv6,int port, int roleStrategy) :
    QTcpServer(parent)
{
    this->clientSocketList.clear();
    this->ready.clear();
    if(!isIPv6)
        listen(QHostAddress::Any,port);
    else
        listen(QHostAddress::AnyIPv6,port);
    this->roleStrategy=roleStrategy;

}

void Server::incomingConnection ( int socketDescriptor )
{
    if(this->clientSocketList.size() >= playerSum)
        return;
    ClientSocket* newSocket=new ClientSocket;
    newSocket->setSocketDescriptor(socketDescriptor);

    clientSocketList.append(newSocket);
    ready.append(false);
    this->socketDescriptorList.append(socketDescriptor);
    newSocket->id=clientSocketList.count()-1;
    connect(newSocket,SIGNAL(disconnected()),newSocket,SLOT(onDisconnected()));
    connect(newSocket,SIGNAL(disconnectedSIN(ClientSocket*)),this,SLOT(onDisconnectedSIG(ClientSocket*)));
    connect(newSocket,SIGNAL(disconnected()),newSocket,SLOT(deleteLater()));
    connect(newSocket,SIGNAL(readyRead()),newSocket,SLOT(readMessage()));
    connect(newSocket,SIGNAL(getMessage(int,QString)),this,SLOT(decoder(int,QString)));

    QString temp = "1;";
    int howMany=clientSocketList.size();
    temp += QString::number(this->clientSocketList.size()-1);
    temp += ";";
    this->sendMessage(this->clientSocketList.size()-1,temp);

    if(howMany == playerSum)
        emit this->seatArrangeSIG();
    else
        sendMessage(-1,tr("现有")+QString::number(howMany)+tr("名玩家进入房间，请耐心等候"));
}

//num=-1表示广播
void Server::sendMessage(int num, QString message)
{
    QByteArray block; //用于暂存我们要发送的数据

    QDataStream out(&block,QIODevice::WriteOnly);

    //使用数据流写入数据

    out.setVersion(QDataStream::Qt_4_7);

    //设置数据流的版本，客户端和服务器端使用的版本要相同

    out<<(quint16) 0;

    out<<message;

    out.device()->seek(0);
    out<<(quint16)(block.size() - sizeof(quint16));


    QTcpSocket *clientConnection;
    if(num != -1)
    {
        clientConnection = clientSocketList.at(num);
        clientConnection->write(block);
    }
    else
    {
        for(int i = 0;i < this->clientSocketList.size();i++)
        {
            clientConnection = this->clientSocketList.at(i);
            clientConnection->write(block);
        }
    }
    emit this->toDisplay("send:");
    emit this->toDisplay(message);
    //qDebug()<<this->serverAddress().toString();

//    clientConnection->disconnectFromHost();

}

void Server::onDisconnectedSIG(ClientSocket* brokenSocket)
{
    clientSocketList.removeOne(brokenSocket);
    coder.notice("<font color=\'red\'>玩家"+QString::number(brokenSocket->getID())+"断线</font>");
}

void Server::decoder(int id, QString message)
{
    QStringList infor;
    emit this->toDisplay("receive:");
    emit this->toDisplay(message);
    if(message == NULL)
        return;
    infor = message.split(";");

    QList<int> cards;
    QStringList chosenCards;
    BatInfor ans;
    BatInfor action;
    BatInfor reply;
    BatInfor missileInfor;
    int i,howMany;

    switch(infor.at(0).toInt())
    {
    case 0:
        emit seatPrearrangeSIG(id,infor.at(1).toInt());
        break;
    case ACTIONCOMMAND:

        if(infor.at(1) == QString::number(ATTACK))
        {

            action.reply = infor.at(1).toInt();
            action.CardID = infor.at(2).toInt();
            if(infor.at(3) == "")
                action.dstID = 0;
            else
                action.dstID = infor.at(3).toInt();
            if(infor.at(4) == "")
                action.srcID = 0;
            else
                action.srcID = infor.at(4).toInt();

            messageBuffer::writeBatInfor(action);
        }
        else if(infor.at(1) == QString::number(SPECIAL))
        {
            action.reply = infor.at(1).toInt();
            action.CardID = infor.at(2).toInt();
            if(infor.at(3) == "")
                action.infor1 = 0;
            else
                action.infor1 = infor.at(3).toInt();
            if(infor.at(4) == "")
                action.infor2 = 0;
            else
                action.infor2 = infor.at(4).toInt();

            messageBuffer::writeBatInfor(action);
        }
        else if(infor.at(1).toInt() == MAGIC)
        {
            if(infor.at(2).toInt() == 0)
            {
                action.reply = MAGIC;
                action.infor1 = COMMONMAGIC;
                action.CardID = infor.at(3).toInt();
                action.dstID = infor.at(4).toInt();
                action.srcID = infor.at(5).toInt();

                messageBuffer::writeBatInfor(action);
            }
        }
        else if(infor.at(1).toInt() == FINISH)
        {
            action.reply = FINISH;
            messageBuffer::writeBatInfor(action);
        }
    break;

    case UNACTIONAL:
    action.reply = UNACTIONAL;
    messageBuffer::writeBatInfor(action);
    break;

    case REBATCOMMAND:

        reply.reply = infor.at(1).toInt();
        if(reply.reply != HIT)
        {
            reply.CardID = infor.at(2).toInt();
            reply.dstID = infor.at(3).toInt();
            reply.srcID = infor.at(4).toInt();
        }

        messageBuffer::writeBatInfor(reply);
    break;

    case DISCARDCOMMAND:

        chosenCards = infor.at(1).split(",");

        for(int i = 0;i < chosenCards.size();i++)
            cards << chosenCards.at(i).toInt();
        messageBuffer::writeCardInfor(cards);

    break;

    case READYBEGIN:
        ready[id] = true;
        for(int i = 0;i < ready.size();i++)
            if(!ready.at(i))
                return;
        for(int i = 0;i < ready.size();i++)
            ready[i]=false;
        emit roleStrategySIG();
    break;
    case 47:
        ready[id] = true;
        emit role3Pick1ReplySIG(id,infor.at(1).toInt());
        for(int i = 0;i < ready.size();i++)
            if(!ready.at(i))
                return;
        emit roleNoticeSIG();
        emit gameStartSIG();
        break;

    case WEAKCOMMAND:
        messageBuffer::writeInfor(infor[1].toInt());
    break;

    case MISSILECOMMAND:

        missileInfor.reply = infor.at(1).toInt();
        if(missileInfor.reply != 2)
            missileInfor.CardID = infor.at(2).toInt();
        if(missileInfor.reply == 0)
            missileInfor.dstID = infor.at(3).toInt();

        messageBuffer::writeBatInfor(missileInfor);
    break;

    case ANSFORCROSS:
    messageBuffer::writeInfor(infor[1].toInt());
    break;

    case ANSFORSKILL:
    messageBuffer::writeInfor(infor[1].toInt());
    break;
//连续技
    case 101:
        ans.infor1=101;
        ans.srcID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//剑影
    case 102:
        ans.infor1=102;
        ans.srcID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//疾风技额外行动
    case 103:
        ans.infor1=103;
        ans.srcID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case BOWLADY_ANSFORTRANSFIXTION:

    ans.reply = infor[1].toInt();
    if(ans.reply != 0)
        ans.CardID = infor[2].toInt();
    messageBuffer::writeBatInfor(ans);
    break;

    case BOWLADY_TRAP:

    ans.reply = MAGIC;
    ans.infor1 = BOWLADY_TRAP;
    ans.CardID = infor[1].toInt();
    ans.dstID = infor[2].toInt();
    ans.srcID = infor[3].toInt();
    messageBuffer::writeBatInfor(ans);
    break;

    case BOWLADY_SNIPE:

    ans.reply = MAGIC;
    ans.infor1 = BOWLADY_SNIPE;
    ans.infor2 = infor[1].toInt();
    ans.dstID = infor[2].toInt();
    ans.srcID = infor[3].toInt();
    messageBuffer::writeBatInfor(ans);
    break;
    case 304:
        ans.infor1=304;
        ans.srcID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//封印法术
    case 401:
        ans.reply = MAGIC;
        ans.infor1 = 401;
        ans.CardID = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//五系束缚
    case 402:
        ans.reply = MAGIC;
        ans.infor1 = 402;
        ans.infor2 = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//封印破碎
    case 403:
        ans.reply = MAGIC;
        ans.infor1 = 403;
        ans.infor2 = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.srcID=infor[3].toInt();
        ans.CardID = infor[4].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//法术激荡
    case 404:
        ans.srcID = infor.at(1).toInt();
        ans.infor1=404;
        messageBuffer::writeBatInfor(ans);
        break;
//水影
    case 502:
        messageBuffer::writeMsg(message.remove(0,4));
        break;
//潜行
    case 503:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//冰霜祷言
    case 601:
        ans.dstID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//治疗术
    case 602:
        ans.reply=MAGIC;
        ans.CardID=infor[1].toInt();
        ans.dstID=infor[2].toInt();
        ans.srcID=infor[3].toInt();
        ans.infor1=602;
        messageBuffer::writeBatInfor(ans);
        break;
//治愈之光
    case 603:
        ans.reply=MAGIC;
        ans.CardID=infor[1].toInt();
        ans.srcID=infor[2].toInt();
        ans.infor1=603;
        int n;
        n=infor[3].toInt();
        ans.infor2=n;
        ans.dstID=infor[4].toInt();
        if (n>1)
            ans.infor3=infor[5].toInt();
        if (n>2)
            ans.infor4=infor[6].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//怜悯
    case 604:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//圣疗
    case 605:
        ans.reply=MAGIC;
        ans.infor4=infor[1].toInt();
        ans.dstID=infor[2].toInt();
        ans.infor1=605;
        ans.infor2=infor[3].toInt();
        ans.infor3=infor[4].toInt();
        ans.srcID=infor[5].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//圣疗额外行动
    case 606:
        ans.srcID = infor.at(1).toInt();
        ans.infor1=606;
        messageBuffer::writeBatInfor(ans);
        break;
//风之洁净
    case 701:
        ans.reply=MAGIC;
        ans.infor1=701;
        ans.CardID = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.srcID= infor[3].toInt();
        ans.infor2=infor[4].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//天使祝福
    case 702:
        ans.reply = MAGIC;
        ans.infor1 = 702;
        ans.CardID = infor[1].toInt();
        ans.srcID= infor[2].toInt();
        ans.infor2= infor[3].toInt();
        ans.dstID = infor[4].toInt();
        if(ans.infor2==2)
            ans.infor3=infor[5].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//天使之墙
    case 703:
        ans.reply = MAGIC;
        ans.infor1 = 703;
        ans.CardID = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//天使之歌
    case 704:
        ans.reply=infor[1].toInt();
        if(ans.reply==1)
        {
            ans.infor1=infor[2].toInt();
            ans.dstID=infor[3].toInt();
            ans.CardID=infor[4].toInt();
        }
        messageBuffer::writeBatInfor(ans);
        break;
//神之庇护
    case 705:
        ans.reply=infor[1].toInt();
        if(ans.reply==1)
        {
            ans.infor1=infor[2].toInt();
            ans.infor2= infor[3].toInt();
        }
        messageBuffer::writeBatInfor(ans);
        break;
//天使羁绊
    case 707:
        ans.dstID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//天使祝福（给牌）
    case 751:
        howMany=infor[1].toInt();
        for(i=0;i<howMany;i++)
            cards<<infor[i+2].toInt();
        messageBuffer::writeCardInfor(cards);
        break;

//魔弹融合(回合内）
    case 801:
        ans.reply = MAGIC;
        ans.CardID = infor[1].toInt();
        ans.infor1 = 801;
        ans.dstID = infor[2].toInt();
        ans.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//魔弹融合（回合外）
    case 802:
        missileInfor.reply = 802;
        missileInfor.CardID = infor.at(1).toInt();
        missileInfor.dstID = infor.at(2).toInt();
        messageBuffer::writeBatInfor(missileInfor);
        break;
//魔爆冲击
    case 803:
        ans.reply = MAGIC;
        ans.infor1 = 803;
        ans.CardID = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.infor2 = infor[3].toInt();
        ans.srcID = infor[4].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//毁灭风暴
    case 804:
        ans.reply = MAGIC;
        ans.infor1 = 804;
        ans.dstID = infor[1].toInt();
        ans.infor2 = infor[2].toInt();
        ans.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//魔爆冲击（弃牌）
    case 851:
        ans.reply=infor[1].toInt();
        if(ans.reply)
        ans.CardID=infor[2].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//修罗连斩
    case 901:
        ans.srcID = infor.at(1).toInt();
        ans.infor1=901;
        messageBuffer::writeBatInfor(ans);
        break;
//暗影流星
    case 902:
        ans.reply = MAGIC;
        ans.infor1 = 902;
        ans.CardID = infor[1].toInt();
        ans.infor2 = infor[2].toInt();
        ans.dstID = infor[3].toInt();
        ans.srcID = infor[4].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//暗影凝聚
    case 903:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//辉耀
    case 1001:
        ans.reply = MAGIC;
        ans.infor1 = 1001;
        ans.CardID = infor[1].toInt();
        ans.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//惩戒
    case 1002:
        ans.reply = MAGIC;
        ans.infor1 = 1002;
        ans.CardID = infor[1].toInt();
        ans.dstID = infor[2].toInt();
        ans.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case 1003:
        ans.infor1 = 1003;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case 1004:
        ans.infor1 = 1004;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//地枪
    case 1005:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//圣光祈愈
    case 1006:
        ans.reply = MAGIC;
        ans.infor1 = 1006;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case 1007:
        ans.infor1 = 1007;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;

//元素法术
    case 1101:
        action.reply=MAGIC;
        action.infor1=1101;
        action.infor2=infor.at(1).toInt();
        action.dstID = infor.at(2).toInt();
        action.srcID = infor.at(3).toInt();
        action.infor3=infor.at(4).toInt();
        action.CardID=infor.at(5).toInt();
        if(action.infor3==2)
            action.infor4=infor.at(6).toInt();
        if(action.infor2==2)
            action.infor5=infor.at(5+action.infor3).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//元素点燃
    case 1102:
        action.reply=MAGIC;
        action.infor1=1102;
        action.dstID = infor.at(1).toInt();
        action.srcID = infor.at(2).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//月光
    case 1103:
        action.reply=MAGIC;
        action.infor1=1103;
        action.dstID = infor.at(1).toInt();
        action.srcID = infor.at(2).toInt();
        messageBuffer::writeBatInfor(action);
        break;
    case 1104:
        ans.infor1 = 1104;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case 1105:
        ans.infor1 = 1105;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case 1106:
        ans.infor1 = 1106;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//欺诈
    case 1201:
        action.reply=ATTACKSKILL;
        action.CardID = infor.at(1).toInt();
        action.dstID = infor.at(2).toInt();
        action.srcID = infor.at(3).toInt();
        action.infor1=1201;
        action.infor2=infor.at(4).toInt();
        action.infor3=infor.at(5).toInt();
        action.infor4=infor.at(6).toInt();
        if(action.infor2==3)
            action.infor5=infor.at(7).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//偷天换日
    case 1202:
        action.reply = MAGIC;
        action.infor1 = 1202;
        action.infor2 = infor[1].toInt();
        action.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//特殊加工
    case 1203:
        action.reply = MAGIC;
        action.infor1 = 1203;
        action.infor2 = infor[1].toInt();
        action.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//冒险者天堂
    case 1204:
        action.reply = SPECIAL;
        action.infor1 = 1204;
        action.dstID = infor[1].toInt();
        action.srcID = infor[2].toInt();
        action.infor2 = infor[3].toInt();
        action.infor3 = infor[4].toInt();
        messageBuffer::writeBatInfor(action);
        break;
    case 1205:
        ans.infor1 = 1205;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
    case 1206:
        ans.infor1 = 1206;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//死灵法师
//瘟疫
    case 1302:
        action.reply = MAGIC;
        action.infor1 = 1302;
        action.CardID = infor[1].toInt();
        action.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//死亡之触
    case 1303:
        action.reply = MAGIC;
        action.infor1 = 1303;
        action.dstID = infor[1].toInt();
        action.srcID = infor[2].toInt();
        action.infor2 = infor[3].toInt();
        action.infor3 = infor[4].toInt();
        action.inforstr = infor[5];
        messageBuffer::writeBatInfor(action);
        break;
//墓碑陨落
    case 1304:
        action.reply = MAGIC;
        action.infor1 = 1304;
        action.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//仪式中断
    case 1401:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//仲裁仪式
    case 1402:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//末日审判
    case 1403:
        action.reply=MAGIC;
        action.infor1=1403;
        action.dstID = infor.at(1).toInt();
        action.srcID = infor.at(2).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//判决天平
    case 1404:
        action.reply = MAGIC;
        action.infor1 = 1404;
        action.infor2 = infor[1].toInt();
        action.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//神官
//神圣祈福
    case 1502:
        action.reply = MAGIC;
        action.infor1 = 1502;
        action.CardID = infor[1].toInt();
        action.infor2 = infor[2].toInt();
        action.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(action);
        break;
 //水之神力
    case 1531:
        action.reply = MAGIC;
        action.infor1 = 1503;
        action.dstID = infor[1].toInt();
        action.CardID = infor[2].toInt();
        action.srcID = infor[3].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//水之神力给牌
    case 1532:
        cards<<infor[1].toInt();
        messageBuffer::writeCardInfor(cards);
        break;
//神圣契约
    case 1504:
        action.infor1 = 1504;
        action.infor2 = infor[1].toInt();
        action.dstID = infor[2].toInt();
        action.infor3 = infor[3].toInt();
        action.srcID = infor[4].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//神圣领域
    case 1505:
        action.reply = MAGIC;
        action.infor1 = 1505;
        action.infor2 = infor[1].toInt();
        action.dstID = infor[2].toInt();
        action.CardID = infor[3].toInt();
        action.infor3 = infor[4].toInt();
        action.srcID = infor[5].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//祈祷
    case 1601:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//威力赐福
    case 1602:
        action.reply=MAGIC;
        action.infor1=1602;
        action.CardID=infor.at(1).toInt();
        action.dstID = infor.at(2).toInt();
        action.srcID = infor.at(3).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//迅捷赐福
    case 1603:
        action.reply=MAGIC;
        action.infor1=1603;
        action.CardID=infor.at(1).toInt();
        action.dstID = infor.at(2).toInt();
        action.srcID = infor.at(3).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//光辉信仰
    case 1604:
        ans.reply = MAGIC;
        ans.infor1 = 1604;
        ans.dstID = infor[1].toInt();
        ans.srcID = infor[2].toInt();
        howMany=infor[3].toInt();
        if(howMany>0)
            ans.CardID = infor[4].toInt();
        if(howMany>1)
            ans.infor2 = infor[5].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//漆黑信仰
    case 1605:
        action.reply=MAGIC;
        action.infor1=1605;
        action.dstID = infor.at(1).toInt();
        action.srcID = infor.at(2).toInt();
        messageBuffer::writeBatInfor(action);
        break;
//法力潮汐
    case 1606:
        ans.infor1=1606;
        ans.srcID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//迅捷额外
    case 1607:
        ans.infor1=1607;
        ans.srcID=infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//挑衅
    case 2101:
        ans.reply = MAGIC;
        ans.infor1 = 2101;
        ans.dstID = infor[1].toInt();
        ans.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
//禁断之力
    case 2102:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
    case 2103:
        ans.infor1 = 2103;
        ans.srcID = infor[1].toInt();
        messageBuffer::writeBatInfor(ans);
        break;
        //法术反弹
            case 1701:
                messageBuffer::writeMsg(message);
                break;
        //魔道法典
            case 1702:
                action.reply = MAGIC;
                action.infor1 = 1702;
                action.dstID = infor[1].toInt();
                action.srcID = infor[2].toInt();
                action.infor2 = infor[3].toInt();
                action.inforstr = infor[4];
                messageBuffer::writeBatInfor(action);
                break;
        //圣洁法典
            case 1703:
                action.reply = MAGIC;
                action.infor1 = 1703;
                action.srcID = infor[1].toInt();
                action.infor2 = infor[2].toInt();
                action.infor3 = infor[3].toInt();
                action.inforstr = infor[4];
                action.inforstrp = infor[5];
                messageBuffer::writeBatInfor(action);
                break;
//苍炎蓄力
    case 2001:
        messageBuffer::writeMsg(message);
        break;
//念弹
    case 2002:
        messageBuffer::writeMsg(message);
        break;
//百式幻龙拳
    case 2003:
        messageBuffer::writeInfor(infor[1].toInt());
        break;
//斗神天驱
    case 2004:
        messageBuffer::writeMsg(message);
        break;
//巫女 23
//同生共死
    case 2301:
        action.reply = MAGIC;
        action.infor1 = 2301;
        action.dstID = infor[1].toInt();
        action.srcID = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//血之哀伤
    case 2302:
        action.infor1 = 2302;
        action.infor2 = infor[1].toInt();
        action.dstID = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//逆流
    case 2303:
        action.reply = MAGIC;
        action.infor1 = 2303;
        action.srcID = infor[1].toInt();
        action.CardID = infor[2].toInt();
        action.infor2 = infor[2].toInt();
        messageBuffer::writeBatInfor(action);
        break;
 //血之悲鸣
    case 2304:
        action.reply = MAGIC;
        action.infor1 = 2304;
        action.dstID = infor[1].toInt();
        action.CardID = infor[2].toInt();
        action.infor2 = infor[3].toInt();
        action.srcID = infor[4].toInt();
        messageBuffer::writeBatInfor(action);
        break;
//血之诅咒
    case 2305:
        action.reply = MAGIC;
        action.infor1 = 2305;
        action.dstID = infor[1].toInt();
        action.srcID = infor[2].toInt();
        action.infor2 = infor[3].toInt();
        action.inforstr = infor[4].toInt();
        messageBuffer::writeBatInfor(action);
        break;
    }
}
