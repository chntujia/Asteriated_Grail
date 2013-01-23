#ifndef SHENGQIANG_H
#define SHENGQIANG_H
#include "Role.h"

class ShengQiang : public Role
{
    Q_OBJECT
public:
    ShengQiang();
    void BingShuangDaoYan();
    void LianMin();
    void normal();
    void attackOrMagic();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void HuiYao();
    void ChengJie();
    void ShengGuangQiYu();
    void TianQiang();
    void DiQiang();

};

#endif // SHENGQIANG_H
