#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include "data/Card.h"
class Status
{
    friend class StatusItem;
    friend class Player;
public:
    Status(int type,Card*card){this->type=type;this->card=card;}
    Card* getCard(){return card;}
private:
    int type;
    Card*card;
};

class Player:public QObject
{
    Q_OBJECT
public:
    Player(int id,int color);
    void setHandCardsMax(int howMany);
    void setCrossNum(int howMany);
    void setGem(int howMany);
    void setCrystal(int howMany);
    void setEnergyMax(int howMany);
    void setRole(int roleID);
    void setTap(bool flag){tap=flag;}
    void setSpecial(int type,bool flag);
    void setToken(int id,int howMany){token[id]=howMany;}
    void changeHandCardNum(int increase){handCardsNum+=increase;}
    void addStatus(int type,Card* card);
    void removeStatus(Card* card);
    bool checkStatus(int type);
    bool checkStatus(QString status);
    bool hasStatus(){return !statusList.isEmpty();}
    void setName(QString name){this->name=name;}
    int getID();
    QString getName();
    QString getTokenName(int id){return tokenName[id];}
    int getHandCardMax();
    int getHandCardNum();
    int getCrossNum();
    int getCrossMax();
    int getGem();
    int getCrystal();
    int getEnergy();
    int getEnergyMax();
    int getColor();
    int getRoleID(){return roleID;}
    int getToken(int id){return token[id];}
    int getTokenMax(int id){return tokenMax[id];}
    bool getSpecial(int type){return specials[type];}
    bool getTap(){return tap;}
    QString getFaceSource(){return faceSource;}
    QString getTapSource(){return tapSource;}
    QList<Status*> getStatus(){return statusList;}
signals:
    void addStatusSIG(Status *status);
    void removeStatusSIG(Status *status);
protected:
    int id;
    QString name;
    int handCardsMax;
    int handCardsNum;
    int crossNum;
    int crossMax;
    int gem;
    int crystal;
    int energyMax;
    int color;
    int roleID;
    int tokenMax[3];
    int token[3];
    bool tap;
    bool specials[5];
    QList<Status*> statusList;
    QString faceSource;
    QString tapSource;
    QString tokenName[3];

};


#endif // PLAYER_H
