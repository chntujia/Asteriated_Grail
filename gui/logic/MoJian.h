#ifndef MOJIAN_H
#define MOJIAN_H
#include "Role.h"
class MoJian : public Role
{
    Q_OBJECT
public:
    MoJian();
    void normal();
    void XiuLuoLianZhan();
    void AnYingNingJu();
    void HeiAnZhenChan();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void AnYingLiuXing();
};

#endif // MOJIAN_H
