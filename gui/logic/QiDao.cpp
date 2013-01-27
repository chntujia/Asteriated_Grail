#include "QiDao.h"

QiDao::QiDao()
{
    makeConnection();

    Button *weiLi,*xunJie,*guangHui,*qiHei;
    weiLi=new Button(3,tr("ÍþÁ¦´Í¸£"));
    buttonArea->addButton(weiLi);
    connect(weiLi,SIGNAL(buttonSelected(int)),this,SLOT(WeiLiCiFu()));

    xunJie=new Button(4,tr("Ñ¸½Ý´Í¸£"));
    buttonArea->addButton(xunJie);
    connect(xunJie,SIGNAL(buttonSelected(int)),this,SLOT(XunJieCiFu()));

    guangHui=new Button(5,tr("¹â»ÔÐÅÑö"));
    buttonArea->addButton(guangHui);
    connect(guangHui,SIGNAL(buttonSelected(int)),this,SLOT(GuangHuiXinYang()));

    qiHei=new Button(6,tr("ÆáºÚÐÅÑö"));
    buttonArea->addButton(qiHei);
    connect(qiHei,SIGNAL(buttonSelected(int)),this,SLOT(QiHeiXinYang()));
}

void QiDao::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(tr("ÍþÁ¦´Í¸£")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(tr("Ñ¸½Ý´Í¸£")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
    unactionalCheck();
}

void QiDao::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(tr("ÍþÁ¦´Í¸£")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(tr("Ñ¸½Ý´Í¸£")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
}

void QiDao::QiDong()
{
    state=1601;
    tipArea->setMsg(tr("ÊÇ·ñ·¢¶¯Æíµ»£¿"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
    }
    if(flag)
        decisionArea->enable(0);
}

void QiDao::WeiLiCiFu()
{
    state=1602;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(tr("ÍþÁ¦´Í¸£"));
}

void QiDao::XunJieCiFu()
{
    state=1603;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(tr("Ñ¸½Ý´Í¸£"));
}

void QiDao::GuangHuiXinYang()
{
    state=1604;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::QiHeiXinYang()
{
    state=1605;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::FaLiChaoXi()
{
    magicAction();
    Player*myself=dataInterface->getMyself();
    onceUsed=true;
    tipArea->setMsg(tr("ÇëÑ¡ÔñÊ¹ÓÃµÄÄÜÁ¿£º"));
    if(myself->getCrystal()>=1)
        tipArea->addBoxItem(tr("1.Ë®¾§"));
    if(myself->getGem()>=1)
        tipArea->addBoxItem(tr("2.±¦Ê¯"));

    tipArea->showBox();
}
