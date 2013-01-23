#include "client/ClientUI.h"
#include "client/ui_ClientUI.h"
#include "data/DataInterface.h"
ClientUI::ClientUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientUI)
{
    tcpSocket=new Client;
    ui->setupUi(this);
//    QRegExp regAddr("[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}");
//    ui->addr->setValidator(new QRegExpValidator(regAddr, this));
    QRegExp regPort("[0-9]{1,6}");
    ui->port->setValidator(new QRegExpValidator(regPort, this));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(link()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(tcpSocket,SIGNAL(readyToStart()),this,SLOT(startGame()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));

    ui->addr->setText("127.0.0.1");
    //ui->addr->setText("2001:5C0:1000:B::7C63");
    ui->port->setText("50000");
}

ClientUI::~ClientUI()
{
    delete ui;
}
void ClientUI::on_addr_textChanged()
{
    ui->port->setEnabled(ui->addr->hasAcceptableInput());
}
void ClientUI::on_port_textChanged()
{
    ui->connectButton->setEnabled(ui->port->hasAcceptableInput());
}
void ClientUI::startGame()
{
    ui->startButton->setEnabled(1);
}

void ClientUI::link()
{
    tcpSocket->link(ui->addr->text(),ui->port->text().toInt());
    ui->connectButton->setEnabled(0);
}

void ClientUI::displayError(QAbstractSocket::SocketError)
{
     qDebug() << tcpSocket->errorString(); //Êä³ö´íÎóÐÅÏ¢
}
