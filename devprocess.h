#ifndef DEVPROCESS_H
#define DEVPROCESS_H

#include <QThread>
#include "gengxin.h"
class DevProcess : public QThread
{
    Q_OBJECT
public:
    explicit DevProcess(QObject *parent = 0);

protected:
    void run();
    
signals:
    void DevStatus(int i);

public slots:
    
private:

};

#endif // DEVPROCESS_H
