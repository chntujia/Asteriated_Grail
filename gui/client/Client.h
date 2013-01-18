#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include "client/ClientUI.h"
class Client : public QTcpSocket
{
    Q_OBJECT
    friend class ClientUI;
public:
    explicit Client();
    void link(QString addr,int port);

signals:
    void readyToStart();
    void getMessage(QString message);
public slots:
    void readMessage();
    void sendMessage(QString message);
private:
};

#endif // CLIENT_H
