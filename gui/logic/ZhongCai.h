#ifndef ZHONGCAI_H
#define ZHONGCAI_H
#include "Role.h"
class ZhongCai : public Role
{
    Q_OBJECT
public:
    ZhongCai();
    void normal();
    void YiShiZhongDuan();
    void ZhongCaiYiShi();
public slots:
    void MoRiShenPan();
    void PanJueTianPing();
    void onOkClicked();
    void onCancelClicked();
    void decipher(QString command);
private:
};

#endif // ZHONGCAI_H
