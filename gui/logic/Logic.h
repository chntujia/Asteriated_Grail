#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <client/Client.h>

class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = 0);
    void setClient(Client* socket){this->socket=socket;}
    void setMyRole(int roleID=0);
    void readyToStart();
    Client* getClient(){return socket;}
signals:
    void sendCommand(QString);
    void gameStart();
public slots:
    void getCommand(QString command);
private:
    Client* socket;
    int count;
};
extern Logic* logic;
#endif // LOGIC_H
