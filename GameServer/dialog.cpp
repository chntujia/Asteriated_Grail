#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("ipv4");
    ui->comboBox->addItem("ipv6");
    ui->lineEdit->setText("50000");
    connect(ui->buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked()), this, SLOT(onOK()));
}

void Dialog::onOK()
{
    if(ui->comboBox->currentIndex()==0)
        server=new Server(0,1,ui->lineEdit->text().toInt());
    else
        server=new Server(0,0,ui->lineEdit->text().toInt());
}

Dialog::~Dialog()
{
    delete ui;
}
