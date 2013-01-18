#ifndef KUANGZHAN_H
#define KUANGZHAN_H
#include "Role.h"
class KuangZhan : public Role
{
    Q_OBJECT
public:
    KuangZhan();
    void SiLie();
    void XueXingPaoXiao();
    void XueYingKuangDao();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
};

#endif // KUANGZHAN_H
