#ifndef GONGNV_H
#define GONGNV_H
#include "Role.h"
class GongNv : public Role
{
    Q_OBJECT
public:
    GongNv();
    void GuanChuanSheJi();
    void JingZhunSheJi();
    void normal();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ShanGuangXianJing();
    void JuJi();
};

#endif // GONGNV_H
