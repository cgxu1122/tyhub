#ifndef DATAUPDATE_H
#define DATAUPDATE_H

#include <QObject>
#include <QtNetwork>
#include <string.h>
 #include <QString>
 #include <QByteArray>
 #include <QCryptographicHash>
 #include <QFile>
 #include <QDebug>
#include "sqlopt.h"

using namespace std;

#define URL_DEVVER "http://192.168.1.103:8080/nzyw/api/getDeviceVersion.do"
#define URL_APKLIBVER "http://192.168.1.103:8080/nzyw/api/getApkLibVersion.do"
#define URL_PKGLIBVER "http://192.168.1.103:8080/nzyw/api/getPkgLibVersion.do"
#define  TMP_PATH  "/tmp/tmpapk/"
#define  APK_PATH  "/tmp/jing/data/"
#define  SPIRIT_PATH "/dev/jingling/jing.zip"

typedef struct APK_INFO{
    QString  apkId;
    qint32    counter;
    qint32   icon;
    qint32   run;
    qint32   sort;
    QString packagePath;
    QString md5;
}ApkInfo;

class DataUpdate : public QThread
{
    Q_OBJECT
public:
    explicit DataUpdate(QObject *parent = 0);
    ~DataUpdate();
    
private:
    QNetworkAccessManager* NetMgrDev;
    QNetworkAccessManager* NetMgrApk;
    QNetworkAccessManager* NetMgrPkg;
    QNetworkAccessManager* NetMgrDown;
    QNetworkAccessManager* ApkDownLoad;
    QNetworkReply* ReplyDev;
    QNetworkReply* ReplyApk;
    QNetworkReply* ReplyPkg;
    QNetworkReply* ReplyDown;
    QNetworkReply* ReplyApkDL;
    QUrl ApkUrl;
    qint8 ApkFailNum;
    qint8 ApkDownFlag;
    SqlOpt *sqlopt;

public:
    void GetDeviceVer(); //0 无更新 1 成功  2失败
    void GetApkLibVer(); //0 无更新 1 成功  2失败 3 不完全
    void GetPkgLibVer(); //0 无更新 1 成功  2失败
    bool StartUpdate();
    void mkdir(QString  path);
    void mk_filedir(QString  path);
    void setPreMd5(string md5);
    string getPreMd5();
    string getFilePath();
    void setApkFile(string apkfile);
    string getApkFile();
    void setAPKURL(string apkpath);
    string  getAPKURL();
    void setFilePath(string filepath);
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist) ;
    QString getFileMd5(QString filepath);

    void Download_File(string url, string filepath);
    bool MD5_Check(QString strFilePath, QString md5);
    QFile* dfile;
    QFile* apkfile;
    int GetDevState();
    int GetApkState();
    int GetPkgState();
private:
    int nDevVerState;
    int nApkLibState;
    int nPkgLibState;
    QString QStr_apkid;
    //QString QStr_apkpack;
    string FilePath;
    string Code;
    string DevVer;
    string ApkVer;
    string ApkLibVer;
    string PkgVer;
    string Premd5;
    string ApkFile;
    int  Apk_Update_Total;
    int  Apk_Update_finish;
    bool bDevState;
    bool bApkState;
    bool bPkgState;
    QEventLoop* loop;

private:


signals:
    void CloseUp();

public slots:
   void  DevReadyRead();
   void  DevFinish();
   void  ApkFinish();
   void  PkgReadyRead();
   void  PkgFinish();
   void  DownloadRead();
   void DownloadFinish();
   void quit();

   void  ApkFileWrite(QNetworkReply*);
//   void ApkDownReadyRead(/*QNetworkReply* */);
//   void ApkDownFish(/*QNetworkReply* */ );
//   void HandleError(QNetworkReply::NetworkError);

protected:
   void run();
public:
   void GetFile(QString str);
};

#endif // DATAUPDATE_H
