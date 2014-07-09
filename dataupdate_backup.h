#ifndef DATAUPDATE_H
#define DATAUPDATE_H

#define URL_DEVVER "http://xxx.xxx.com/nzyw/api/getDeviceVersion.do"
#define URL_APKLIBVER "http://xxx.xxx.com/nzyw/api/getApkLibVersion.do"
#define URL_PKGLIBVER "http://xxx.xxx.com/nzyw/api/getPkgLibVersion.do''

#include <QObject>
#include <QtNetwork>

class DataUpdate : public QObject
{
    Q_OBJECT
public:
    explicit DataUpdate(QObject *parent = 0);
    //~DataUpdate();
    
private:
    QNetworkAccessManager* NetMgrDev;
    QNetworkAccessManager* NetMgrApk;
    QNetworkAccessManager* NetMgrPkg;

public:
    void GetDeviceVer(); //0 无更新 1 成功  2失败
    void GetApkLibVer(); //0 无更新 1 成功  2失败 3 不完全
    void GetPkgLibVer(); //0 无更新 1 成功  2失败
    bool StartUpdate();

private:
    int nDevVerState;
    int nApkLibState;
    int nPkgLibState;

signals:
    
public slots:
    
};

#endif // DATAUPDATE_H
