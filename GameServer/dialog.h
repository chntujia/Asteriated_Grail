#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<Server.h>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    Server* getServer(){return server;}
public slots:
    void onOK();
private:
    Server* server;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
