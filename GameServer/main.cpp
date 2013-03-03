#include <QtCore/QCoreApplication>

#include "BackgroundEngine.h"
#include "testuiex.h"
#include "Server.h"
#include "dialog.h"
Q_DECLARE_METATYPE(QList<CardEntity*>)

QList<int> shieldCardID;
QList<int> weakCardID;
QList<int> poisonCardID;
QList<int> missileCardID;

QList<CardEntity*> cardList;
TeamArea teamArea;
Coder coder;
int playerSum = 6;
//以下为编码器函数,具体可参见通讯协议
void Coder::askForAction(int playerID, int actionTypeAllowed,bool acted)
{
    emit this->sendMessage(-1,combMessage(TOQSTR(ASKFORACTION),TOQSTR(playerID),TOQSTR(actionTypeAllowed),acted?"1":"0"));
}

void Coder::useCardNotice(QList<CardEntity *> cards, int dstID, int srcID,int realCard)
{
    QString cardsID;
    for(int i = 0;i < cards.size();i++)
    {
        cardsID += TOQSTR(cards.at(i)->getID());
        if(i < cards.size() - 1)
            cardsID += ",";
    }
    emit this->sendMessage(-1,combMessage("28",cardsID,TOQSTR(dstID),TOQSTR(srcID),TOQSTR(realCard)));
}

void Coder::drawNotice(int ID,int sum,QList<CardEntity*> cards)
{
    QString message = combMessage("9",TOQSTR(ID),TOQSTR(sum));
    for(int i = 0;i < cards.size();i++)
    {
        message += TOQSTR(cards.at(i)->getID());
        if(i != cards.size() - 1)
            message += ",";
        else
            message += ";";
    }
    emit this->sendMessage(-1,message);
}
void Coder::getCardNotice(int sum, QList<CardEntity *> cards, int dstID, bool show)
{
    QString cardsID;
    for(int i = 0;i < cards.size();i++)
    {
        cardsID += TOQSTR(cards.at(i)->getID());
        if(i < cards.size() - 1)
            cardsID += ",";
    }
    emit this->sendMessage(-1,combMessage("41",TOQSTR(dstID),TOQSTR(show),TOQSTR(sum),cardsID));
}
void Coder::discardNotice(int ID,int sum,QString show,QList<CardEntity*> cards)
{
    QString message = combMessage("13",TOQSTR(ID),TOQSTR(sum),show);
    for(int i = 0;i < cards.size();i++)
    {
        message += TOQSTR(cards.at(i)->getID());
        if(i != cards.size() - 1)
            message += ",";
        else
            message += ";";
    }
    emit this->sendMessage(-1,message);
}

void Coder::moveCardNotice(int sum,QList<CardEntity*> cards,int srcID,int srcArea,int dstID,int dstArea)
{
    QString message = combMessage("19",TOQSTR(sum));
    int n;
    for(int i = 0;i < cards.size();i++)
    {
        n=cards.size();
        message += TOQSTR(cards.at(i)->getID());
        if(i != cards.size() - 1)
            message += ",";
        else
            message += ";";
    }
    message += combMessage(TOQSTR(srcID),TOQSTR(srcArea),TOQSTR(dstID),TOQSTR(dstArea));
    emit this->sendMessage(-1,message);
}

void Coder::askForRolePick(int ID,int howMany,QList<int>*roles)
{
    QString message="46;"+QString::number(howMany)+";";
    for(int i=0;i<howMany;i++)
        message+=QString::number(roles->takeFirst())+";";
    emit sendMessage(ID,message);
}

void Coder::coverCardNotice(int playerID, int howMany, QList<CardEntity *> cards, bool remove, bool show)
{
    QString message = "48;" + TOQSTR(playerID) + ";" + TOQSTR(howMany) + ";";
    for(int i = 0;i < howMany;i++)
    {
        message += TOQSTR(cards[i]->getID());
        if(i != howMany - 1)
            message += ",";
    }
    message += ";" + TOQSTR((int)remove) + ";" + TOQSTR((int)show) + ";";
    emit sendMessage(-1,message);
}

void Coder::optionalRoleNotice(int num, QList<int> *roles)
{
    QString message="51;"+QString::number(num)+";";
    for(int i=0;i<num;i++)
        message+=QString::number(roles->takeFirst())+";";
    emit sendMessage(-1,message);
}

void Coder::askForBan(int ID)
{
    QString message="52;"+QString::number(ID)+";";
    emit sendMessage(ID, message);
}

void Coder::banNotice(int ID, int role)
{
    QString message = "54;"+QString::number(ID)+";"+QString::number(role)+";";
    emit sendMessage(-1, message);
}

void Coder::askForPick(int ID)
{
    QString message="55;"+QString::number(ID)+";";
    emit sendMessage(ID, message);
}

void Coder::pickNotice(int ID, int role)
{
    QString message = "57;"+QString::number(ID)+";"+QString::number(role)+";";
    emit sendMessage(-1, message);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    messageBuffer::init();
//    QTextCodec *codec = QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);
//    QTextCodec::setCodecForTr(codec);
    /*
    int initialArray[28] = {0,1,2,6,7,8,23,24,29,30,3,9,25,26,34,35,4,10,31,36,37,38,5,22,27,28,32,33};
    for(int i = 0;i < 28;i++)
    {
        if(i < 10)
            shieldCardID << initialArray[i];
        else if(i < 16)
            weakCardID << initialArray[i];
        else if(i < 22)
            poisonCardID << initialArray[i];
        else if(i < 28)
            missileCardID << initialArray[i];
    }
    */

    qRegisterMetaType<CardEntity*>("CardEntity*");
    qRegisterMetaType<QList<CardEntity*> >("QList<CardEntity*>");
    qRegisterMetaType<QList<CardEntity*> >("QList<CardEntity*>&");

    Dialog dia;
    if(dia.exec()==QDialog::Accepted)
    {
        MyRoom *test = new MyRoom(dia.getServer());

    //test->show();
        return a.exec();
    }
    else return 0;
}

MyRoom::MyRoom(Server *server):QObject()
{
    backEngine = new BackgroundEngine();
    testUI = new TestUIex(NULL);
    this->serverModule = server;

    engineThr = new MyThread;
    backEngine->moveToThread(engineThr);
    engineThr->start();

    //connect(backEngine,SIGNAL(askForActSIG(QList<CardEntity*>,int)),testUI,SLOT(actSLOT(QList<CardEntity*>,int)));
    //connect(backEngine,SIGNAL(askForReply(QList<CardEntity*>,QString,int)),testUI,SLOT(replySLOT(QList<CardEntity*>,QString,int)));
    //connect(backEngine,SIGNAL(askForHeal(int,int)),testUI,SLOT(healSLOT(int,int)));
    connect(backEngine,SIGNAL(toInforDisplay(QString)),testUI,SLOT(toDisplay(QString)));

    connect(serverModule,SIGNAL(seatArrangeSIG()),backEngine,SLOT(seatArrange()));
    connect(serverModule,SIGNAL(seatPrearrangeSIG(int,bool)),backEngine,SLOT(seatPrearrange(int,bool)));
    connect(this->serverModule,SIGNAL(toDisplay(QString)),this->testUI,SLOT(toDisplay(QString)));
    connect(backEngine,SIGNAL(sendMessageSIG(int,QString)),this->serverModule,SLOT(sendMessage(int,QString)));
    connect(&coder,SIGNAL(sendMessage(int,QString)),this->serverModule,SLOT(sendMessage(int,QString)));

    switch(serverModule->getRoleStrategy())
    {
    case 0:
        connect(this->serverModule,SIGNAL(roleStrategySIG()),backEngine,SLOT(roleRandom()));
        break;
    case 1:
        connect(this->serverModule,SIGNAL(roleStrategySIG()),backEngine,SLOT(role3Pick1()));
        connect(serverModule,SIGNAL(role3Pick1ReplySIG(int,int)),backEngine,SLOT(role3Pick1Reply(int,int)));
        connect(serverModule,SIGNAL(gameStartSIG()),this->backEngine,SLOT(gameStart()));
        connect(this->serverModule,SIGNAL(roleNoticeSIG()),backEngine,SLOT(seatPostarrange()));
        break;
    case 2:
        connect(this->serverModule,SIGNAL(roleStrategySIG()),backEngine, SLOT(BP()));
    }

//    emit this->serverModule->toDisplayPublic("server IP:" + this->serverModule->serverAddress().toString());
    emit this->serverModule->toDisplayPublic("server port:" + QString::number(this->serverModule->serverPort()));
}

QString combMessage(QString item1, QString item2, QString item3, QString item4, QString item5,QString item6,QString item7)
{
    QString message;
    message = item1;
    message += ";";
    if(item2 != (QChar)0)
    {
        message += item2;
        message += ";";
    }
    if(item3 != (QChar)0)
    {
        message += item3;
        message += ";";
    }
    if(item4 != (QChar)0)
    {
        message += item4;
        message += ";";
    }
    if(item5 != (QChar)0)
    {
        message += item5;
        message += ";";
    }
    if(item6 != (QChar)0)
    {
        message += item6;
        message += ";";
    }
    if(item7 != (QChar)0)
    {
        message += item7;
        message += ";";
    }

    return message;
}
void MyRoom::makeConnect(int playerNum)
{
    for(int i = 0;i < playerNum;i++)
    {
        //connect(backEngine->getPlayerByID(i),SIGNAL(askForOverLoadSIG(QList<CardEntity*>,int)),testUI,SLOT(overLoadSLOT(QList<CardEntity*>,int)));
    }
}

CardEntity* getCardByID(int ID)
{
    for(int i = 0;i < cardList.size();i++)
    {
        if(ID == cardList.at(i)->getID())
            return cardList.at(i);
    }
    return NULL;
}


