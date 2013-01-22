#ifndef YONGZHE_H
#define YONGZHE_H
#include "Role.h"
class YongZhe : public Role
{
    Q_OBJECT
public:
    YongZhe();
    void normal();
    void NuHou();
    void MingJingZhiShui();
    void JinDuanZhiLi();
    void SiDou();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
 //   void cardAnalyse();
    void TiaoXin();
private:
    int jinDuanZhiLi;
};


#endif // YONGZHE_H
