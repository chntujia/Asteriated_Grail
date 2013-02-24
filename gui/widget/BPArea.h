#ifndef BPAREA_H
#define BPAREA_H

#include <QGraphicsObject>
#include "RoleItem.h"

class BPArea : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    BPArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void BPStart(int num, QList<int> roles);
    void enableRoleItem(int roleID);
    void disableRoleItem(int roleID);
    void setMsg(QString msg);
    void setQuota(int howMany){least=most=howMany;}
    void setQuota(int least,int most){this->least=least;this->most=most;}
    QList<int> getSelectedRoles();
    void choose(int roleID);
    RoleItem *getRoleByID(int ID);
    void reset();
    bool checkOver();
    void remove(int roleID);
    void disablaAll();
    void enableAll();
public slots:
    void onRoleSelected(int id);
    void onRoleUnselected(int id);
private:
    int width;
    int height;
    int least;
    int most;
    int currentRed;
    int currentBlue;
    int currentColor;
    QPixmap background;
    QString msg;
    QList<int> selectedRoles;
    QList<RoleItem*> roleItems;
    QList<int> left;
signals:
    void roleReady();
    void roleUnready();
};

#endif // BPAREA_H
