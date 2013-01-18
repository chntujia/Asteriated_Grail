#ifndef PLAYERAREA_H
#define PLAYERAREA_H

#include <QGraphicsObject>
#include "PlayerItem.h"

class PlayerArea : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    PlayerArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void sortPlayerItems();
    void enablePlayerItem(int id);
    void enableAll();
    void disablePlayerItem(int id);
    void enableEnemy();
    void enableMate(bool includeYou=false);
    void disableAll();
    void reset();
    void setCurrentPlayerID(int id){currentPlayerID=id;}
    void setQuota(int howMany){least=most=howMany;}
    void setQuota(int least,int most){this->least=least;this->most=most;}
    int getCurrentPlayerID(){return currentPlayerID;}
    QList<Player*> getSelectedPlayers();
public slots:
    void onPlayerSelected(int id);
    void onPlayerUnselected(int id);
signals:
    void playerReady();
    void playerUnready();
private:
    QList<PlayerItem*> playerItems;
    QList<Player*> selectedPlayers;
    int least,most;
    int currentPlayerID;
    QList<int> enemy;
};

#endif // PLAYERAREA_H
