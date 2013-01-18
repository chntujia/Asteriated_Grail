#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <QDialog>
#include <QtNetwork>
#include "client/Client.h"
namespace Ui {
class ClientUI;
}
class Client;
class ClientUI : public QDialog
{
    Q_OBJECT

public:
    explicit ClientUI(QWidget *parent = 0);
    ~ClientUI();


private slots:
    void link();
    void on_addr_textChanged();
    void on_port_textChanged();
    void startGame();
    void displayError(QAbstractSocket::SocketError);
private:
    Client* tcpSocket;
    Ui::ClientUI *ui;

};

#endif // CLIENTUI_H
