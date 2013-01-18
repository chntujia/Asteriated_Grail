/********************************************************************************
** Form generated from reading UI file 'ClientUI.ui'
**
** Created: Tue Aug 21 09:30:26 2012
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
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ClientUI
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *board;
    QHBoxLayout *horizontalLayout;
    QLabel *label1;
    QLineEdit *addr;
    QHBoxLayout *horizontalLayout_2;
    QLabel *laber2;
    QLineEdit *port;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *connectButton;
    QPushButton *startButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ClientUI)
    {
        if (ClientUI->objectName().isEmpty())
            ClientUI->setObjectName(QString::fromUtf8("ClientUI"));
        ClientUI->resize(257, 117);
        verticalLayout = new QVBoxLayout(ClientUI);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        board = new QLabel(ClientUI);
        board->setObjectName(QString::fromUtf8("board"));

        verticalLayout->addWidget(board);

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
        board->setText(QApplication::translate("ClientUI", "Please connect...", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("ClientUI", "Host Addr:", 0, QApplication::UnicodeUTF8));
        laber2->setText(QApplication::translate("ClientUI", "Host Port:", 0, QApplication::UnicodeUTF8));
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
