#ifndef TESTUIEX_H
#define TESTUIEX_H
#include "main.h"
#include <QWidget>
#include "ui_testuiex.h"

//这是客户端测试的UI,内容可以忽略
namespace Ui {
    class TestUIex;
}

class TestUIex : public QWidget
{
    Q_OBJECT

public:
    TestUIex(QWidget *parent):QWidget(parent),
    ui(new Ui::TestUIex)
{
    ui->setupUi(this);
    this->show();
}
    ~TestUIex(){
        delete ui;
    }
    void testStart();
public slots:
    void toDisplay(QString content);
/*
public slots:
    void actSLOT(QList<CardEntity*> hand,int currentID);
    void replySLOT(QList<CardEntity*> hand,QString element,int userID);
    void healSLOT(int cross,int harmPoint);
    void overLoadSLOT(QList<CardEntity*> hand,int overNum);
    void testSLOT(){qDebug()<<"test";}
    void toDisplay(QString content);
signals:
    //void startSIG();
    */

private:
    Ui::TestUIex *ui;
};

#endif // TESTUIEX_H
