#include "KuangZhan.h"

KuangZhan::KuangZhan()
{
    makeConnection();
setMyRole(this);
}

void KuangZhan::SiLie()
{
    state=36;
    tipArea->setMsg(tr("是否发动撕裂？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void KuangZhan::XueXingPaoXiao()
{
    state=36;
    tipArea->setMsg(tr("是否发动血腥咆哮？"));
    decisionArea->enable(0);
    decisionArea->enable(1);

}

void KuangZhan::XueYingKuangDao()
{
    state=36;
    tipArea->setMsg(tr("是否发动血影狂刀？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void KuangZhan::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==tr("撕裂"))
        SiLie();
    else if(skill==tr("血腥咆哮"))
        XueXingPaoXiao();
    else if(skill==tr("血影狂刀"))
        XueYingKuangDao();
}
