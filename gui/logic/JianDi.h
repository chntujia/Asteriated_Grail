#ifndef JIANDI_H
#define JIANDI_H
#include "Role.h"
class JianDi : public Role
{
    Q_OBJECT
public:
    JianDi();
    void JianHunShouHu(int cardID);
    void JianQiZhan(int targetID);
    void JianQiZhan2();
    void TianShiZhiHun();
    void EMoZhiHun();
    void BuQuYiZhi();
    void askForSkill(QString skill);
    void additionalAction();
    void coverCardAnalyse();
    void playerAnalyse();
    void turnBegin();
public slots:
    void onOkClicked();
    void onCancelClicked();
private:
    bool buquUsed;
    int shouhuID;
    int jianqiID;
    QString jianqiPoint;
};

#endif // JIANDI_H
