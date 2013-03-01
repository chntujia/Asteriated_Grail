/********************************************************************************
** Form generated from reading UI file 'ClientUI.ui'
**
** Created: Wed Feb 27 23:30:30 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTUI_H
#define UI_CLIENTUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ClientUI
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label1;
    QLineEdit *addr;
    QHBoxLayout *horizontalLayout_2;
    QLabel *laber2;
    QLineEdit *port;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLineEdit *nickname;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QComboBox *comboBox;
    QTextBrowser *board;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *connectButton;
    QPushButton *startButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ClientUI)
    {
        if (ClientUI->objectName().isEmpty())
            ClientUI->setObjectName(QString::fromUtf8("ClientUI"));
        ClientUI->resize(257, 310);
        verticalLayout = new QVBoxLayout(ClientUI);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label1 = new QLabel(ClientUI);
        label1->setObjectName(QString::fromUtf8("label1"));

        horizontalLayout->addWidget(label1);

        addr = new QLineEdit(ClientUI);
        addr->setObjectName(QString::fromUtf8("addr"));

        horizontalLayout->addWidget(addr);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        laber2 = new QLabel(ClientUI);
        laber2->setObjectName(QString::fromUtf8("laber2"));

        horizontalLayout_2->addWidget(laber2);

        port = new QLineEdit(ClientUI);
        port->setObjectName(QString::fromUtf8("port"));

        horizontalLayout_2->addWidget(port);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_2 = new QLabel(ClientUI);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_5->addWidget(label_2);

        nickname = new QLineEdit(ClientUI);
        nickname->setObjectName(QString::fromUtf8("nickname"));

        horizontalLayout_5->addWidget(nickname);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(ClientUI);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        comboBox = new QComboBox(ClientUI);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_4->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout_4);

        board = new QTextBrowser(ClientUI);
        board->setObjectName(QString::fromUtf8("board"));

        verticalLayout->addWidget(board);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        connectButton = new QPushButton(ClientUI);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout_3->addWidget(connectButton);

        startButton = new QPushButton(ClientUI);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(false);

        horizontalLayout_3->addWidget(startButton);

        cancelButton = new QPushButton(ClientUI);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);

#ifndef QT_NO_SHORTCUT
        label1->setBuddy(addr);
        laber2->setBuddy(port);
        label_2->setBuddy(nickname);
        label->setBuddy(comboBox);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(addr, port);
        QWidget::setTabOrder(port, connectButton);
        QWidget::setTabOrder(connectButton, startButton);
        QWidget::setTabOrder(startButton, cancelButton);

        retranslateUi(ClientUI);

        QMetaObject::connectSlotsByName(ClientUI);
    } // setupUi

    void retranslateUi(QDialog *ClientUI)
    {
        ClientUI->setWindowTitle(QApplication::translate("ClientUI", "ClientUI", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("ClientUI", "Host Addr:", 0, QApplication::UnicodeUTF8));
        laber2->setText(QApplication::translate("ClientUI", "Host Port:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ClientUI", "\346\230\265\347\247\260:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ClientUI", "\346\212\242\351\230\237\357\274\210\344\270\215\344\270\200\345\256\232\346\210\220\345\212\237\357\274\211", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("ClientUI", "Connect", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("ClientUI", "Start", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ClientUI", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ClientUI: public Ui_ClientUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTUI_H
