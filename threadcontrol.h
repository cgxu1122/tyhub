#ifndef THREADCONTROL_H
#define THREADCONTROL_H
#include "dataupdate.h"

#include <QThread>

class ThreadControl : public QThread
{
    Q_OBJECT
public:
    explicit ThreadControl(QObject *parent = 0);

signals:

public slots:

protected:
    void run();
};

#endif // THREADCONTROL_H
