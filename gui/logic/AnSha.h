#ifndef ANSHA_H
#define ANSHA_H
#include "Role.h"
class AnSha : public Role
{
    Q_OBJECT
public:
    AnSha();
    void ShuiYing();
    void QianXing();
public slots:
    void decipher(QString command);
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
};

#endif // ANSHA_H
