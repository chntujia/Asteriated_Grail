#ifndef MOGONG_H
#define MOGONG_H
#include "Role.h"

class MoGong : public Role
{
    Q_OBJECT
public:
    MoGong();
    void normal();
    void askForSkill(QString skill);
    void turnBegin();
    void attackAction();
    void additionalAction();
    void MoGuanChongJiHit();
    void DuoChongSheJi();
    void ChongNengMoYan1();
    void ChongNengMoYan2();
    void ChongNengGaiPai();
    void MoYanGaiPai();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void coverCardAnalyse();
    void MoGuanChongJi();
    void LeiGuangSanShe();
private:
    bool MoGuanChongJiUsed;
    bool DuoChongSheJiUsed;
    bool ChongNengUsed;
    int lastTarget;
    int startChoice;
    int chongnengNum;
};

#endif // MOGONG_H
