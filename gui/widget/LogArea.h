#ifndef LOGAREA_H
#define LOGAREA_H

#include <QTextEdit>

class LogArea : public QTextEdit
{
    Q_OBJECT
public:
    explicit LogArea(QWidget *parent = 0);
    void append(const QString &text);
    
};

#endif // LOGAREA_H
