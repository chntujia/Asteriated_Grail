#ifndef JIANSHENG_H
#define JIANSHENG_H
#include "Role.h"
class JianSheng : public Role
{
    Q_OBJECT
public:
    JianSheng();
    void LieFengJi();
    void LianXuJi();
    void JianYing();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
private:
    bool onceUsed2;
    int JiFengJi;
};

#endif // JIANSHENG_H
