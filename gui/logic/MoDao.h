#ifndef MODAO_H
#define MODAO_H
#include "Role.h"
class MoDao : public Role
{
    Q_OBJECT
public:
    MoDao();
    void normal();
    void moDaned(int nextID);
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void MoDanRongHe();
    void MoBaoChongJi();
    void HuiMeiFengBao();
private:
    int nextClockwise;
    bool firstMoDan;
};

#endif // MODAO_H
