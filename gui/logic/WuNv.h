#ifndef WUNV_H
#define WUNV_H
#include "Role.h"

class WuNv : public Role
{
    Q_OBJECT
public:
    WuNv();
    void normal();
    void askForSkill(QString skill);
    void XueZhiAiShang();
public slots:
    void TongShengGongSi();
    void NiLiu();
    void XueZhiBeiMing();
    void XueZhiZuZhou();
private:
    int tongShengID;
};

#endif // WUNV_H
