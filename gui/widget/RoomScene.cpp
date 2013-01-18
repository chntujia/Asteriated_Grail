#include "RoomScene.h"
#include <QPointF>
#include "widget/GUI.h"
#include <QPainter>
static const QPointF HandPos(165,505);
static const QPointF PlayerAreaPos(0,0);
static const QPointF ShowAreaPos(165,260);
static const QPointF TipAreaPos(210,230);
static const QPointF TeamAreaPos(2,2);
static const QPointF DecisionAreaPos(725,450);
static const QPointF ButtonAreaPos(165,650);
static const QPointF LogAreaPos(901,30);

RoomScene::RoomScene()
{
    setBackgroundBrush(QBrush(QPixmap("resource/background.png")));

    handArea = new HandArea;
    addItem(handArea);
    handArea->setPos(HandPos);
    gui->setHandArea(handArea);

    playerArea=new PlayerArea;
    addItem(playerArea);
    playerArea->setPos(PlayerAreaPos);
    gui->setPlayerArea(playerArea);

    showArea=new ShowArea;
    addItem(showArea);
    showArea->setPos(ShowAreaPos);
    gui->setShowArea(showArea);

    tipArea=new TipArea;
    addItem(tipArea);
    tipArea->setPos(TipAreaPos);
    gui->setTipArea(tipArea);

    decisionArea=new DecisionArea;
    addItem(decisionArea);
    decisionArea->setPos(DecisionAreaPos);
    gui->setDecisionArea(decisionArea);

    buttonArea=new ButtonArea;
    addItem(buttonArea);
    buttonArea->setPos(ButtonAreaPos);
    gui->setButtonArea(buttonArea);

    teamArea=new TeamArea();
    addItem(teamArea);
    teamArea->setPos(TeamAreaPos);
    gui->setTeamArea(teamArea);

    logArea=new LogArea;
    logAreaProxy=addWidget(logArea);
    logAreaProxy->setPos(LogAreaPos);
    gui->setLogArea(logArea);



}
