#ifndef SHENGNV_H
#define SHENGNV_H
#include "Role.h"
class ShengNv : public Role
{
    Q_OBJECT
public:
    ShengNv();
    void BingShuangDaoYan();
    void LianMin();
    void normal();
    void attackOrMagic();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ZhiLiaoShu();
    void ZhiYuZhiGuang();
    void ShengLiao();
    void ShengLiao2();
    void ShengLiao3();    
};
#endif // SHENGNV_H
