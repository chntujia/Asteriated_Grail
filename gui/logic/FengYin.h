#ifndef FENGYIN_H
#define FENGYIN_H
#include "Role.h"
class FengYin : public Role
{
    Q_OBJECT
public:
    FengYin();
    void normal();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
    void FengYinFaShu();
    void WuXiShuFu();
    void FengYinPoSui();
    void onUnready();
private:
    bool usedGem;
    int flag;
};

#endif // FENGYIN_H
