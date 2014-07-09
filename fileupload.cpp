#include "fileupload.h"
#include <QtDebug>
#define URL_UPLOAD "http://192.168.1.103:8080/nzyw/api/processLog.do"
#define BLOCKSIZE 100
#define MAXLINELEN 512
#define LOGPATH "/home/zb/work/log"
#define PERIOD (60*60*2)

FileUpload::FileUpload(QObject *parent) :
    QThread(parent)
{
    this->nUpFaildRetryNum = 2;
    this->nUpFaildNum = 0;
  //  this->NetUp = new QNetworkAccessManager(this);
    this->Log_Sql = new Data_Sql;
    this->Log_Sql->sqlinit();
}
FileUpload::~FileUpload()
{
    this->NetUp->deleteLater();
    this->Log_Sql->sqlclose();
    if(Log_Sql != NULL)
    {
        delete Log_Sql;
        Log_Sql = NULL;
    }
}

void FileUpload::SetUrlUp(const QUrl Url_Up)
{
    this->UrlUp = Url_Up;
}
void FileUpload::SetFileString(QString strFile)
{
    this->strFile = strFile;
}
void FileUpload::ResetFaildNum()
{
    this->nUpFaildNum = 0;
}
void FileUpload::AddFaildNum()
{
    this->nUpFaildNum++;
}
bool FileUpload::IncLog(std::string strLogDate)
{
    return true;
}
void FileUpload::UpSingleFile(QString strFile)
{
    QFile Fd(strFile);
    QByteArray BlockData;
    BlockData.clear();
    QFileInfo FInfor(strFile);
    QString FileID = FInfor.baseName();
    Fd.open(QIODevice::ReadOnly | QIODevice::Text);
    while( !Fd.atEnd())
    {
        QByteArray OneLine = Fd.readLine(MAXLINELEN);
        char * crystr = OneLine.data();
        for( int i=0; i<OneLine.size(); i++){
            crystr[i] = crystr[i] ^ 0x12;
        }
        QByteArray byte( crystr);
        OneLine = byte;

        OneLine.insert(0,tr("\""));
        OneLine.replace(QByteArray(","),QByteArray("|"));
        OneLine.replace(QByteArray("\n"),QByteArray("\","));
        BlockData.append(OneLine);
    }
    BlockData.remove(BlockData.size()-1,1);
    BlockData.insert(0,tr("{["));
    BlockData.insert(BlockData.size(),tr("]}"));
    //qDebug()<<BlockData<<"\n";
    UpOneBlock(BlockData,FileID);
}
void FileUpload::UpOneBlock(QByteArray BlockData,QString FileID)
{
    loop = new QEventLoop;
    QObject::connect(NetUp,SIGNAL(finished(QNetworkReply*)), this, SLOT(UpFinishSingleFile(QNetworkReply*)));
    QNetworkRequest request(QUrl(tr(URL_UPLOAD)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    QByteArray SendData("id=");
    SendData.append(FileID);
    SendData.append("&data=");
    SendData.append(BlockData);
    qDebug()<<SendData<<"\n";
    NetUp->post(request, SendData);
    loop->exec();
   // UpFinishSingleFile();
}
void FileUpload::UpFinishSingleFile(QNetworkReply* Reply_Up)
{
#if 0
    QByteArray Res = Reply_Up->readAll();
    //Test
//    QByteArray Res("{\"status\":1,\"id\":2014062101}");
    QJson::Parser ResParser;
    bool bFlag;
    QVariantMap ResMap = ResParser.parse(Res,&bFlag).toMap();
    QString FopenedFilePath = LOGPATH;
    FopenedFilePath += tr("/");
    FopenedFilePath += ResMap["id"].toString();
    FopenedFilePath +=tr(".csv");
    QFile Fd(FopenedFilePath);
    QString strFileDate = ResMap["id"].toString().left(8);
    if(bFlag && Fd.exists())
    {
        if(ResMap["status"].toInt() == 1)
        {
            Fd.close();
            Fd.remove();
            ResetFaildNum();
            //写数据库
            this->Log_Sql->log_read_incr(strFileDate.toStdString());
           // disconnect(NetUp,SIGNAL(finished(QNetworkReply*)), this, SLOT(UpFinishSingleFile(QNetworkReply*)));
        }
        else if(ResMap["status"].toInt() == 0)
        {
            AddFaildNum();
            if(this->nUpFaildNum <= this->nUpFaildRetryNum)
            {
                UpSingleFile(this->strFile);
            }
            else
            {
                ResetFaildNum();
                Fd.close();
                //disconnect(NetUp,SIGNAL(finished(QNetworkReply*)), this, SLOT(UpFinishSingleFile(QNetworkReply*)));
            }
        }
        else
        {
            if(Fd.exists())
                Fd.close();
            //disconnect(NetUp,SIGNAL(finished(QNetworkReply*)), this, SLOT(UpFinishSingleFile(QNetworkReply*)));
            qDebug()<<"Error status\n";
        }
    }
    else
   {
        qDebug()<<"Error parser or File not exsist\n";
        //disconnect(NetUp,SIGNAL(finished(QNetworkReply*)), this, SLOT(UpFinishSingleFile(QNetworkReply*)));
   }
    loop->quit();
#endif
}
void FileUpload::HandleError(QNetworkReply::NetworkError NetError)
{

}

void FileUpload::run()
{
    NetUp = new QNetworkAccessManager();
    while(1)
    {
        emit SetUpState(true);
        QStringList LogLst = GetAllFiles(LOGPATH);
        QString strLog;
        foreach(strLog, LogLst)
        {
            UpSingleFile(strLog);
        }
        sleep(5);   //@zb
        emit SetUpState(false);
      //  sleep(3);   //@zb
    }
}
QStringList FileUpload::GetAllFiles(QString strPath)
{
    QDir dir(strPath);
    QFileInfoList FileInfoLst;
    QStringList strFileLst;
    strFileLst.clear();
    if( !dir.exists() )
    {
        return strFileLst;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);
    FileInfoLst = dir.entryInfoList();
    int nFileCount = FileInfoLst.count();
    if (nFileCount <= 0)
    {
        return strFileLst;
    }
    for(int i = 1; i < nFileCount; i++) //The latest log not include
        strFileLst.append(FileInfoLst.at(i).absoluteFilePath());
    return strFileLst;
}
