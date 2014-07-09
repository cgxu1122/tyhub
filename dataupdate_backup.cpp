#include "dataupdate.h"

DataUpdate::DataUpdate(QObject *parent) :
    QObject(parent)
{
    NetMgrDev = new QNetworkAccessManager(this);
}

//DataUpdate::~DataUpdate()
//{
//    delete NetMgrDev;
//}
