#ifndef GUI_H
#define GUI_H

#include <QObject>
#include "HandArea.h"
#include "PlayerArea.h"
#include "ShowArea.h"
#include "TeamArea.h"
#include "TipArea.h"
#include "ButtonArea.h"
#include "LogArea.h"
#include "BPArea.h"

class GUI : public QObject
{
    Q_OBJECT
public:
    explicit GUI(QObject *parent = 0);

    void setHandArea(HandArea* handArea);
    void setPlayerArea(PlayerArea* playerArea);
    void setShowArea(ShowArea* showArea);
    void setTeamArea(TeamArea* teamArea);
    void setTipArea(TipArea* tipArea);
    void setDecisionArea(DecisionArea*decisionArea);
    void setButtonArea(ButtonArea* buttonArea);
    void setLogArea(LogArea* logArea);
    void setEnable(bool flag);
    void setCoverArea(HandArea* coverArea){this->coverArea = coverArea;}
    void setBPArea(BPArea* bpArea){this->bpArea = bpArea;}
    void reset();

    HandArea* getHandArea();
    PlayerArea* getPlayerArea();
    ShowArea* getShowArea();
    TeamArea* getTeamArea();
    TipArea* getTipArea();
    DecisionArea* getDecisionArea();
    ButtonArea* getButtonArea();
    void logAppend(QString msg);
    HandArea* getCoverArea(){return coverArea;}
    BPArea* getBPArea(){return bpArea;}
public slots:
    void showCoverArea(bool show);
    void showCoverArea();
    void closeCoverArea();
private:
    HandArea* handArea;
    PlayerArea* playerArea;
    ShowArea* showArea;
    TeamArea* teamArea;
    TipArea* tipArea;
    DecisionArea* decisionArea;
    ButtonArea* buttonArea;
    LogArea* logArea;

    HandArea* coverArea;
    BPArea* bpArea;
};
extern GUI* gui;

#endif // GUI_H
