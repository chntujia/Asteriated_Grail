#ifndef MAOXIAN_H
#define MAOXIAN_H
#include "Role.h"
class MaoXian: public Role
{
    Q_OBJECT
public:
    MaoXian();
    void normal();
    void attackOrMagic();
    void attackAction();
    void MaoXianZheTianTang();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void onUnready();
    void cardAnalyse();
    void playerAnalyse();
    void buy();
    void extract();
    void QiZha();
    void TeShuJiaGong();
    void TouTianHuanRi();
private:
    bool onceUsed2;
    bool qizha;
};

#endif // MAOXIAN_H
