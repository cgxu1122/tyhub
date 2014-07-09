#include "devprocess.h"
#include <QDebug>

DevProcess::DevProcess(QObject *parent) :
    QThread(parent)
{
}

void DevProcess::run()
{
    for(int i =0; i<20; i++)
    {
        qDebug("%d",i);
        emit DevStatus(i);
        msleep(1000);
    }
}

