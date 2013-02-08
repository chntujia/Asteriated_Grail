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
    virtual void turnBegin();
    virtual void normal();
    virtual void makeConnection();
    virtual void attackAction();
    virtual void magicAction();
    virtual void attackOrMagic();
    virtual void additionalAction();
    virtual void attacked(QString element,int hitRate);
    virtual void moDaned(int nextID, int sourceID, int howMany);
    virtual void cure(int cross,int harmPoint,int type, int crossAvailable);
    virtual void askForSkill(QString skill);
    void drop(int howMany);
    void unactionalCheck();
    void TianShiZhuFu(int n);
    void MoBaoChongJi();
    void WeiLi();
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
    void setMyRole(Role* role){myRole=role;}
protected:
    int myID;
    int state;
    int sourceID;
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
    QString command;
    Role* myRole;
};



#endif // ROLE_H
