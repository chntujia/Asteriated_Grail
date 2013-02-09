#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("ipv4");
    ui->comboBox->addItem("ipv6");
    ui->comboBox_2->addItem(tr("Ëæ»ú"));

    ui->comboBox_2->addItem(tr("3Ñ¡1"));
    ui->lineEdit->setText("50000");

   connect(ui->buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked()), this, SLOT(onOK()));

}

void Dialog::onOK()
{
    server=new Server(0,ui->comboBox->currentIndex(),ui->lineEdit->text().toInt(),ui->comboBox_2->currentIndex());
}

Dialog::~Dialog()
{
    delete ui;
}
