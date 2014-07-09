#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include <QObject>
#include <QtNetwork>
#include <QThread>
#include "parser.h"
#include "data_sql.h"


class FileUpload : public QThread
{
    Q_OBJECT
public:
    explicit FileUpload(QObject *parent = 0);
    void  UpSingleFile(QString strFile);
    QStringList GetAllFiles(QString strPath);
    ~FileUpload();

private:
    QNetworkAccessManager* NetUp;
    QUrl UrlUp;
    qint8 nUpFaildRetryNum;
    qint8 nUpFaildNum;
    QString strFile;
    Data_Sql* Log_Sql;
    QEventLoop* loop;


private:
    void SetUrlUp(const QUrl Url_Up);
    void SetFileString(QString strFile);
    void UpOneBlock(QByteArray BlockData,QString FileID);
    void ResetFaildNum();
    void AddFaildNum();
    bool IncLog(std::string strLogDate);

signals:
    void SetUpState(bool bState);

public slots:
    void UpFinishSingleFile(QNetworkReply* Reply);
    void HandleError(QNetworkReply::NetworkError);

protected:
    void run();
};

#endif // FILEUPLOAD_H
