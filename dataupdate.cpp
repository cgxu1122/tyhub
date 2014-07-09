#include  <QtScript>
#include "parser.h"
#include <QtDebug>
#include "global.h"
#include "dataupdate.h"

DataUpdate::DataUpdate(QObject *parent) :
    QThread(parent)
{
    this->bDevState = false;
    this->bApkState = false;
    this->bPkgState = false;
    sqlopt = new SqlOpt;
    sqlopt->sqlinit();
}
DataUpdate::~DataUpdate()
{
}
void DataUpdate::GetDeviceVer()
{
    NetMgrDev = new QNetworkAccessManager;
    QNetworkRequest request(QUrl(tr(URL_DEVVER)));
    QByteArray appArry("code=");
    this->Code = Global::g_DevID;
    appArry.append(QString::fromStdString(this->Code));
    appArry.append("&version=");
    this->DevVer = "1.1.0";//sqlopt->dev_get_dev();
    appArry.append(QString::fromStdString(this->DevVer));
     ReplyDev = NetMgrDev->post(request, appArry);
    QObject::connect(ReplyDev, SIGNAL(readyRead()),this,SLOT(DevReadyRead()), Qt::DirectConnection);
    QObject::connect(ReplyDev, SIGNAL(finished()), this, SLOT(DevFinish()),Qt::DirectConnection);
}

void DataUpdate::GetApkLibVer()
{
    NetMgrApk = new QNetworkAccessManager;
    QNetworkRequest request(QUrl(tr(URL_APKLIBVER)));
    QByteArray appArry("code=");
    this->Code = Global::g_DevID;
    appArry.append(QString::fromStdString(this->Code));
    appArry.append("&apkVersion=");
    this->ApkVer = sqlopt->dev_get_apk();
    appArry.append(QString::fromStdString(this->ApkVer));
    ReplyApk = NetMgrApk->post(request, appArry);
    QObject::connect(ReplyApk, SIGNAL(finished()), this, SLOT(ApkFinish()), Qt::DirectConnection);
}

void DataUpdate::GetPkgLibVer()
{
    NetMgrPkg = new QNetworkAccessManager;
    QNetworkRequest request(QUrl(tr(URL_PKGLIBVER)));
    QByteArray appArry("code=");
    this->Code = Global::g_DevID;
    appArry.append(QString::fromStdString(this->Code));
    appArry.append("&apkVersion=");
    this->ApkVer = sqlopt->dev_get_apk();
    appArry.append(QString::fromStdString(this->ApkVer));
    this->PkgVer = sqlopt->dev_get_pkg();
    appArry.append("&pkgVersion=");
    appArry.append(QString::fromStdString(this->PkgVer));
    ReplyPkg = NetMgrPkg->post(request, appArry);
    QObject::connect(ReplyPkg, SIGNAL(readyRead()),this,SLOT(PkgReadyRead()));
    QObject::connect(ReplyPkg, SIGNAL(finished()), this, SLOT(PkgFinish()));
}

bool DataUpdate::StartUpdate()
{
    return true;
}

void DataUpdate::DevReadyRead()
{

}
void DataUpdate::DevFinish()
{
#if 0
    QByteArray dev_rsp_byte = ReplyDev->readAll();
    QString dev_rsp_str = QString(dev_rsp_byte);
    qDebug()<<dev_rsp_str;
    bool ok;
    QJson::Parser parser;
    if (dev_rsp_str.size()==0){
        return;
    }
    QVariantMap dev_rsp_res = parser.parse(dev_rsp_str.toUtf8(), &ok).toMap();
    QString cid=dev_rsp_res["cid"].toString();
    QString org_cid = QString::fromStdString(sqlopt->dev_get_cid());
    if ( cid != org_cid){
        sqlopt->dev_update("apkversion", "0");
        sqlopt->dev_update("pkgversion", "0");
        sqlopt->dev_update("cid", cid.toStdString());

    }
    qint32  nDevVerState=dev_rsp_res["status"].toInt();
    if( nDevVerState == 0){
        bDevState = true;
        return;
    }else if(nDevVerState == 1){
        bDevState = false;
    }
    else{
        QString path=dev_rsp_res["cid"].toString();
        QString dev_md5=dev_rsp_res["md5"].toString();
        QString filename= SPIRIT_PATH;
        Download_File(path.toStdString(),filename.toStdString());
    }
#endif
}

void DataUpdate::ApkFinish()
{
#if 0
//    QByteArray apk_rsp_byte = ReplyApk->readAll();
//    QString apk_rsp_str = QString(apk_rsp_byte);
    QString apk_rsp_str = "{\"apkList\":[{\"apkId\":12,\"md5value\":\"27645ca17ac1c269e67862fcc0f1d2e3\",\"packagePath\":\"com.mapbar.android.accompany\",\"path\":\"http://download.redis.io/releases/redis-2.8.12.tar.gz\",\"type\":0},{\"apkId\":3,\"md5value\":\"d94e494566cb9d0b12c0d70aaec4543f\",\"packagePath\":\"air.com.wuba.bangbang\",\"path\":\"http://download.redis.io/releases/redis-2.8.12.tar.gz\",\"type\":0},{\"apkId\":5,\"md5value\":\"27645ca17ac1c269e67862fcc0f1d2e3\",\"packagePath\":\"com.mapbar.android.accompany\",\"path\":\"http://download.redis.io/releases/redis-2.8.12.tar.gz\",\"type\":0},{\"apkId\":7,\"md5value\":\"27645ca17ac1c269e67862fcc0f1d2e3\",\"packagePath\":\"com.qihoo360pp.paycentre\",\"path\":\"http://download.redis.io/releases/redis-2.8.12.tar.gz\",\"type\":0},{\"apkId\":8,\"md5value\":\"27645ca17ac1c269e67862fcc0f1d2e3\",\"packagePath\":\"com.qihoo360.antilostwatch\",\"path\":\"http://download.redis.io/releases/redis-2.8.12.tar.gz\",\"type\":0}],\"status\":2,\"version\":\"1390999000\"}";
    qDebug()<<apk_rsp_str;
    bool ok;
    QJson::Parser parser;
    bool  apk_flag = true;
    if (apk_rsp_str.size()==0){
        apk_flag = false;
        return;
    }
    QVariantMap apk_rsp_res = parser.parse(apk_rsp_str.toUtf8(), &ok).toMap();
    QString apkVersion=apk_rsp_res["version"].toString();
    qint32  nApkVerState=apk_rsp_res["status"].toInt();

    if( nApkVerState==0){
        apk_flag = true;
        return;
    }else if(nApkVerState == 1 ){
        apk_flag = false;
        return;
    }else{
        QVariantList apklist = apk_rsp_res["apkList"].toList();
        Apk_Update_Total = apklist.size();
        foreach( QVariant atom, apklist){

            ApkDownLoad = new QNetworkAccessManager();
            QVariantMap  apk_map = atom.toMap();
            QStr_apkid = apk_map["apkId"].toString();
            QString apk_file_name ;
            apk_file_name = TMP_PATH + QStr_apkid;
            setApkFile(QStr_apkid.toStdString());
            Premd5 = apk_map["md5value"].toString().toStdString();
            QString packagePath = apk_map["packagePath"].toString();
            sqlopt->apk_update_packagePath(QStr_apkid.toStdString(), packagePath.toStdString());

             QString path = apk_map["path"].toString();
             string ApkPath = path.toStdString();
             if( ApkPath.size() > 0){

                QNetworkRequest Down_Reqrequest(QUrl(tr(ApkPath.c_str())));
                loop = new QEventLoop ;
                QObject::connect(ApkDownLoad, SIGNAL(finished(QNetworkReply*)),this, SLOT(ApkFileWrite(QNetworkReply*)));
                this->ReplyApkDL = ApkDownLoad->get(Down_Reqrequest);
                //QObject::connect(ReplyApkDL,SIGNAL(finished()), &loop, SLOT(quit()));
                loop->exec();

//                QTimer* Time_Wait;
                //this->connect(ReplyApkDL, SIGNAL(finished()),Down_Loop, SLOT(quit()));
                //this->connect(Time_Wait, SIGNAL(timeout()), Down_Loop, SLOT(quit()));
                //Time_Wait->start(1000);
//                if(Time_Wait->isActive()){
//                     Time_Wait->stop();
//              this->ReplyApkDL  = new QNetworkReply();
                //to do
//                }
//                else{
//                //to do
//                    this->ReplyApkDL->abort();
//                }
             }else{  // path == NULL
                    apk_flag = false;
           }
            //QNetwork
        }
        if(  Apk_Update_finish == Apk_Update_finish){
              bApkState = true;
              sqlopt->dev_update("apkversion", apkVersion.toStdString());
              GetPkgLibVer();
         }else{
              bApkState= false;
        }
    }
    ReplyApk->deleteLater();
    NetMgrApk->deleteLater();
#endif
}

void DataUpdate::quit(){

}

void DataUpdate::PkgReadyRead()
{
}
void DataUpdate::PkgFinish()
{
#if 0
    QByteArray pkg_rsp_byte = ReplyPkg->readAll();
    QString pkg_rsp_str = QString(pkg_rsp_byte);
//   QString pkg_rsp_str = "{\"commonPkg\":{\"apkList\":[{\"apkId\":7,\"counter\":0,\"icon\":0,\"run\":1,\"sort\":0},{\"apkId\":8,\"counter\":0,\"icon\":0,\"run\":0,\"sort\":1},{\"apkId\":5,\"counter\":0,\"icon\":0,\"run\":0,\"sort\":2},{\"apkId\":12,\"counter\":0,\"icon\":0,\"run\":0,\"sort\":3},{\"apkId\":3,\"counter\":0,\"icon\":0,\"run\":0,\"sort\":4}],\"batchCode\":\"TY2\",\"modelList\":[],\"name\":\"common_pkg#TY2\",\"packageId\":14,\"type\":0},\"pkgList\":[],\"pkgVersion\":\"1404229444159\",\"status\":2}";
    qDebug()<<pkg_rsp_str;
    bool ok;
    QJson::Parser parser;
    bool  pkg_flag = true;
    if (pkg_rsp_str.size()==0){
        pkg_flag = false;
        return;
    }
    QVariantMap pkg_rsp_res = parser.parse(pkg_rsp_str.toUtf8(), &ok).toMap();
    QString pkgVersion=pkg_rsp_res["pkgVersion"].toString();
    qint32  status=pkg_rsp_res["status"].toInt();
    if(  status == 0 ){
        pkg_flag = true;
        //
        return;
    }else if(status == 1 ){
        pkg_flag = false;
        return;
    }else{
        QString md5;
        QString apk_path;
        QVariantList apkList;
        char  date[12];
        sqlopt->get_date(date, 12, 0);
        QString date_today = date;
        if( pkg_rsp_res["commonPkg"].isNull() ) {
        }else{
            QVariantMap commpkg = pkg_rsp_res["commonPkg"].toMap();
            apkList = commpkg["apkList"].toList();
            QString common_id = "common";
            QString batchCode = commpkg["batchCode"].toString();
           // QString modeList = "";
            QString name = commpkg["name"].toString();
            QString packageId = commpkg["packageId"].toString();
            sqlopt->mob_update( common_id.toStdString(), packageId.toStdString());
            qint8 type = commpkg["type"].toInt();
      //      QList<APK_INFO> apk_ins_list;
            QVariantList apkList = commpkg["apkList"].toList();
            int length=apkList.size();
            QString apk_sort;
            foreach (QVariant apkinfo, apkList) {
                QVariantMap apk_info = apkinfo.toMap();
                QString apkid = apk_info["apkId"].toString();
                qint32 counter = apk_info["counter"].toInt();
                qint32 icon = apk_info["icon"].toInt();
                qint32 run = apk_info["run"].toInt();
                qint32 sort = apk_info["sort"].toInt();
                apk_sort += apkid;
                apk_sort += ",";
                sqlopt->apk_update(apkid.toStdString(), name.toStdString(), counter, icon, run);
            }

            apk_sort.chop(1);
            //           pkg_update( const string pkg_id, const string pkg_name, const string apk_list, const int apk_sum, const string date, const string batchCode);
            sqlopt->pkg_update( packageId.toStdString(), name.toStdString(), apk_sort.toStdString(), batchCode.toStdString(),length, date_today.toStdString());
            //sqlite3  dev common;
            //sqlite3  pkg
        }
         if( ! pkg_rsp_res["pkgList"].isNull() ) {

            QString atom_batchcode;
            QString atom_name;
            QString atom_id;
            QString atom_packageid;
            QString atom_apksort;
            qint32  atom_type;
            QVariantList apk_list;
            QVariantList mob_list;
            QVariantList PkgList = pkg_rsp_res["pkgList"].toList();
            foreach (QVariant atom_list, PkgList) {
                QVariantMap Pkg_atom = atom_list.toMap();
                atom_batchcode = Pkg_atom["batchCode"].toString();
                atom_name = Pkg_atom["name"].toString();
                atom_type = Pkg_atom["type"].toInt();
                atom_id = Pkg_atom["packageId"].toString();
                apk_list = Pkg_atom["apkList"].toList();
                QString apksort;
                foreach( QVariant atom,  apk_list){
                    QVariantMap apkinfo = atom.toMap();
                    QString apkid = apkinfo["apkId"].toString();
                    qint32 counter = apkinfo["counter"].toInt();
                    qint32 icon = apkinfo["icon"].toInt();
                    qint32 run = apkinfo["run"].toInt();
                    qint32 sort = apkinfo["sort"].toInt();
                    sqlopt->apk_update(apkid.toStdString(), atom_name.toStdString(), counter, icon, run);

                    //QString md5;
                    //sqlite3 insert;
//
                    apksort += apkid;
                }
                apksort.chop(1);
                sqlopt->pkg_update(atom_id.toStdString(), atom_name.toStdString(), apksort.toStdString(), apk_list.size(), date_today.toStdString()   );
                mob_list = Pkg_atom["modelList"].toList();
                foreach( QVariant atom, mob_list){
                    QString mob = atom.toString();
                    sqlopt->mob_update(mob.toStdString(), atom_name.toStdString());
                    //insert
                }
            }
         }
     }
    bPkgState = true;
    sqlopt->dev_update("pkgversion", pkgVersion.toStdString());
    ReplyPkg->deleteLater();
    delete NetMgrPkg;
#endif
}

void DataUpdate::run()
{
//        for(int i =0; i<10; i++)
//        {
//            qDebug("%d",i);
//            msleep(1000);
//        }
//    GetDeviceVer();
//    GetPkgLibVer();
   for(int i=0; i<1; ++i)
        GetFile(QString::number(i,10));
//    GetApkLibVer();
//    Download_File("http://download.redis.io/releases/redis-2.8.12.tar.gz","Redis.data");
   emit CloseUp();
}

void DataUpdate::GetFile(QString strNo)
{
    QString str = strNo;
    str += "Redis.data";
    Download_File("http://download.redis.io/releases/redis-2.8.12.tar.gz",str.toStdString());
}
void DataUpdate::Download_File( string url, string file_path)
{
    NetMgrDown = new QNetworkAccessManager();
    QNetworkRequest request(QUrl(tr(url.c_str())));
    ReplyDown = NetMgrDown->get(request);
    setFilePath(file_path);

    string filename=getFilePath();
    if(QFile::exists(QString::fromStdString(filename))){
        QFile::remove( QString::fromStdString(filename));
    }
    dfile = new QFile( QString::fromStdString(filename));
    if( !dfile->open( QIODevice::WriteOnly)){
        dfile->close();
        dfile = 0;
    }
/*  QEventLoop**/ loop = new QEventLoop;
    QObject::connect(ReplyDown, SIGNAL(readyRead()),this,SLOT(DownloadRead()),Qt::DirectConnection);
    QObject::connect(ReplyDown, SIGNAL(finished()), this, SLOT(DownloadFinish()),Qt::DirectConnection);
    QObject::connect(ReplyDown, SIGNAL(finished()),loop,SLOT(quit()));
    loop->exec();
}

void DataUpdate::DownloadRead()
{
    if(dfile){
          dfile->write(ReplyDown->readAll());
    }
}
void DataUpdate::DownloadFinish()
{
   //loop->quit();
    dfile->flush();
    dfile->close();
//    string md5_pre = getPreMd5();
//    string filename = getFilePath();
//    if( MD5_Check(QString::fromStdString(filename), QString::fromStdString(md5_pre) )){
//     /*sqlite gengxin*/
//     /*shell*/
//     //   qDebug()<<"downloadok!";
//           bDevState = true;
//       }else{
//           bDevState = false;
//       }
     ReplyDown->deleteLater();
     NetMgrDown->deleteLater();
}

string DataUpdate::getFilePath(){
    return FilePath;
}
void DataUpdate::setFilePath( string filepath){
    FilePath = filepath;
}

void DataUpdate:: setPreMd5(string md5){
    Premd5 = md5;
}

string DataUpdate:: getPreMd5(){
    return Premd5;
}

void DataUpdate::setApkFile(string apkfile){
    ApkFile = apkfile;
}
string DataUpdate::getApkFile(){
    return ApkFile;
}

bool DataUpdate::MD5_Check(QString strFilePath, QString md5){
    QString md5_tmp = getFileMd5(strFilePath);
    qDebug()<< strFilePath << md5_tmp << strFilePath;
    QString md5_1 = md5_tmp.toLower();
    QString md5_2 = md5.toLower();
    if( md5_2.compare(md5_1) == 0){
            return true;
    }else{
            return false;
    }
}

void DataUpdate::ApkFileWrite(QNetworkReply* Reply_apk){
    qDebug() << QStr_apkid;
    //QString apk_id = QString::fromStdString(getApkFile());
    QString file_name;
    file_name = TMP_PATH + QStr_apkid;
    QFile fd(file_name);

    if(fd.open(QIODevice::WriteOnly)){
        fd.write(Reply_apk->readAll());               //////////budge////////
    }
    fd.flush();
    fd.close();
    string Pre_md5 = getPreMd5();
    if (MD5_Check( file_name ,  QString::fromStdString(Pre_md5) )){
           //sqllite3  string apk_info = sql_opt.get_all();
           QString apk_final_path;
           QString pkg_id;
           int a, b;
           pkg_id = QString::fromStdString( sqlopt->apk_get_path( QStr_apkid.toStdString(), &a, &b));
            apk_final_path = APK_PATH + pkg_id;

           apk_final_path +=  "/" + QStr_apkid + ".apk";
           mk_filedir(apk_final_path);
           if (copyFileToPath(file_name, apk_final_path, true )){
               //sqlite3 change data;
               sqlopt->apk_update(QStr_apkid.toStdString(), Pre_md5);
           }
            Apk_Update_finish += 1;
     }else{
           QFile::remove(file_name);
     }
    loop->quit();
    this->ReplyApkDL->deleteLater();
    ApkDownLoad->deleteLater();

}

bool DataUpdate::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    mkdir(toDir);
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

int DataUpdate::GetDevState()
{
    return this->bDevState;
}
int DataUpdate::GetApkState()
{
    return this->bApkState;
}
int DataUpdate::GetPkgState()
{
   return  this->bPkgState;
}
QString DataUpdate::getFileMd5(QString filePath)
 {
     QFile localFile(filePath);

     if (!localFile.open(QFile::ReadOnly))
     {
         qDebug() << "file open error.";
         return 0;
     }

     QCryptographicHash ch(QCryptographicHash::Md5);

     quint64 totalBytes = 0;
     quint64 bytesWritten = 0;
     quint64 bytesToWrite = 0;
     quint64 loadSize = 1024 * 4;
     QByteArray buf;

     totalBytes = localFile.size();
     bytesToWrite = totalBytes;

     while (1)
     {
         if(bytesToWrite > 0)
         {
             buf = localFile.read(qMin(bytesToWrite, loadSize));
             ch.addData(buf);
             bytesWritten += buf.length();
             bytesToWrite -= buf.length();
             buf.resize(0);
         }
         else
         {
            break;
         }

         if(bytesWritten == totalBytes)
         {
             break;
         }
     }

     localFile.close();
     QByteArray md5 = ch.result();
     QString qstrmd5;
      qstrmd5.append(md5.toHex());
     return qstrmd5;
 }


void DataUpdate::mkdir(QString  path){
    QDir *tmp = new QDir;
    QStringList  lpath = path.split("/");
    QString new_path;
  //      foreach( QVariant atom, apklist){
    foreach( QString  atom, lpath){
        new_path += "/";
        if ( atom != ""){
            new_path += atom;
            if( ! tmp->exists(new_path)){
                tmp->mkdir( new_path);
            }
        }
    }

}

void DataUpdate::mk_filedir(QString  path){
    QDir *tmp = new QDir;
    QStringList  lpath = path.split("/");
    QString new_path;
  //      foreach( QVariant atom, apklist){
    foreach( QString  atom, lpath){
        new_path += "/";
        if ( atom != ""){
            new_path += atom;
            if( ! tmp->exists(new_path)){
                tmp->mkdir( new_path);
            }
        }
    }
    tmp->rmdir(new_path);

}
