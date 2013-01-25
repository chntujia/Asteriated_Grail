#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include "widget/GUI.h"
#include "data/DataInterface.h"
#include "Logic.h"

class Role : public QObject
{
    Q_OBJECT
public:
    explicit Role(QObject *parent = 0);    
    void attacked(QString element,int hitRate);
    void moDaned(int nextID);
    void cure(int cross,int harmPoint,int type);
    void drop(int howMany);
    void normal();
    void addtionalAction();
    void makeConnection();
    void attackAction();
    void magicAction();
    void attackOrMagic();
    void unactionalCheck();
    void TianShiZhuFu(int n);
    void MoBaoChongJi();
signals:
    void sendCommand(QString);

public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
    void exchangeCards();
    void resign();
    void buy();
    void synthetize();
    void extract();
protected:
    int myID;
    int state;
    int sourcePlayerID;
    int moDanNextID;
    int nextCounterClockwise;
    bool isMyTurn;
    bool onceUsed;
    int actionFlag;
    HandArea*handArea;
    PlayerArea*playerArea;
    ButtonArea*buttonArea;
    DecisionArea*decisionArea;
    TipArea*tipArea;
    TeamArea*teamArea;
    bool usedAttack,usedMagic,usedSpecial,start;
    QStringList actions;

};



#endif // ROLE_H
