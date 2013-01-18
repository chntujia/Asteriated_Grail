#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread:public QThread
{
protected:
    void run();
};

#endif // MYTHREAD_H
