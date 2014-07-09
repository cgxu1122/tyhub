#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "wenjian.h"
#include "fuwuqi.h"
#include "baobiao.h"
#include <QMessageBox>
#include <QDateTime>
#include"shangchuan.h"
#include <QtDebug>
#include <QDateTimeEdit>
#include "devprocess.h"
#include "help.h"
#include "usb_enum.h"
#define PERIOD_SET_UNUPLOAD 30*60
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //network Information
    QList<QNetworkInterface> NetInterfaceList;
    NetInterfaceList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface net, NetInterfaceList)
    {
      //  qDebug()<<net;
        strDevID = net.hardwareAddress();
    }
    netConf = new QNetworkConfigurationManager;
    connect(netConf,SIGNAL(onlineStateChanged(bool)),this,SLOT(onlineStateChange(bool)));
    Global::g_DevID = strDevID.toStdString();

    //Update
//    DataUpdate* Up = new DataUpdate;
//    Up->GetFile();
    Gengxin* UpLoadFir = new Gengxin;
    //UpLoadFir->setWindowModality(Qt::ApplicationModal);
    UpLoadFir->show();
    UpLoadFir->SetBtnInvis();
    UpLoadFir->StartUpdate();

    //
    Data_SqlOpt = new Data_Sql;
    File_Opt = new FILE_CTL;
    Data_SqlOpt->sqlinit();

    CreateLayout();

   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout(layout);
   mainLayout->addLayout(gridlayout);
   ui->centralwidget->setLayout(mainLayout);
   ui->centralwidget->adjustSize();
   this->setFixedSize(800,480);

   CreateStatusbar();
   //@Fzq
   g_i = 1;
   UsbDevTimer = new QTimer;
   this->connect(UsbDevTimer,SIGNAL(timeout()),this,SLOT(ScanUsbDev()));
   UsbDevTimer->start(1000);
//   UsbState = new USB_State;
   /*DevProcess* DevPro = new DevProcess;
   DevPro->start();
   this->connect(DevPro,SIGNAL(DevStatus(int)),this,SLOT(ScanUsbDev(int))); 线程间信号和槽*/
   //上传

   QTimer* UnUpCount = new QTimer;
   this->connect(UnUpCount, SIGNAL(timeout()), this, SLOT(SetUnUpCount()));
   UnUpCount->start(PERIOD_SET_UNUPLOAD);

   File_UpLoad = new FileUpload;
   this->connect(File_UpLoad, SIGNAL(SetUpState(bool)), this, SLOT(setMvState(bool)));
    QTimer::singleShot(10*60*1000,File_UpLoad,SLOT(start()));
    call();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Data_SqlOpt;
    Data_SqlOpt->sqlclose();
    delete File_Opt;
    delete netConf;
}

void MainWindow::CreateLayout()
{
     layout = new QHBoxLayout;

     gxin = new ClickedLabel;
     gxin->setText("<u>更新</u>");
    // gxin->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     //gxin->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
     //gxin->setGeometry(QRect(1, 1, 20, 20));
     connect(gxin,SIGNAL(clicked()),this,SLOT(OnGengxin()));

     wluo = new ClickedLabel;
     wluo->setText("<u>网络</u>");
     wluo->setDisabled(true);


     wjian = new ClickedLabel;
     wjian->setText("<u>文件</u>");
     connect(wjian, SIGNAL(clicked()),this, SLOT(OnWenjian()));

     bbiao = new ClickedLabel;
     bbiao->setText("<u>报表</u>");
     connect(bbiao,SIGNAL(clicked()),this,SLOT(OnBaobiao()));

     fwuqi = new ClickedLabel;
     fwuqi->setText("<u>服务器</u>");
     connect(fwuqi,SIGNAL(clicked()),this,SLOT(OnFuwuqi()));

     cangkuid = new QLabel;
     cangkuid->setText("仓库ID:");

     id = new QLabel;
     SqlOpt* sql_opt = new SqlOpt;
     sql_opt->sqlinit();
     QString strStoreID = "zb";//QString::fromStdString(sql_opt->get_stroeid());//@
     sql_opt->sqlclose();
     id->setText(strStoreID);

     //更新图标
     LabUpdateState = new QLabel;
     MvUpdate = new QMovie(":/updating.gif");
     LabUpdateState->setMovie(MvUpdate);
     //connect(this,SIGNAL(setMvState()),MvUpdate,SLOT(ChangeMvState()));
     network = new QLabel;
     if(netConf->isOnline())
     {
          QImage image1(":/NetConn24.ico");
          network->setPixmap(QPixmap::fromImage(image1));
     }
     else
     {
        QImage image1(":/NetDisConn24.ico");
        network->setPixmap(QPixmap::fromImage(image1));
     }
     neicun = new QLabel;
     QImage CardImage(":/SdCard24.ico");
     neicun->setPixmap(QPixmap::fromImage(CardImage));

     shangchuan = new QPushButton;
     shangchuan->setText("上传");
     connect(shangchuan,SIGNAL(clicked()),this,SLOT(OnShangchuan()));

     weishangchuan = new QLabel;
     weishangchuan->setText("未上传:");

     wscnum = new QLabel;
     wscnum->setText("0");

     horizonSpacer1 = new QSpacerItem(800, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
     layout->addWidget(gxin);
     layout->addWidget(wluo);
     layout->addWidget(wjian);
     layout->addWidget(bbiao);
     layout->addWidget(fwuqi);
     layout->addSpacerItem(horizonSpacer1);
     layout->addWidget(cangkuid);
     layout->addWidget(id);
     layout->addSpacerItem(horizonSpacer1);
     layout->addWidget(network);
     layout->addWidget(neicun);
     layout->addWidget(shangchuan);
     layout->addWidget(weishangchuan);
     layout->addWidget(wscnum);
     layout->addWidget(LabUpdateState);


     gridlayout = new QGridLayout;

     for(int nDevCount = 0; nDevCount < DEVCOUNT; nDevCount++)
     {
         DevWdg* TmpDevWdg = new DevWdg;
         DevArray[nDevCount] = TmpDevWdg;
         //VecDev.append(TmpDevWdg);
         QString strNum = QString::number((nDevCount+1),10);
         DevArray[nDevCount]->SetNum(strNum);
         DevArray[nDevCount]->SetStatus(tr("空闲"));
         gridlayout->addWidget(DevArray[nDevCount],nDevCount/6,nDevCount%6);
     }
//     UsbDevHandle = DevArray[1];
//     UsbDevHandle3 = DevArray[2];
}
void MainWindow::CreateStatusbar()
{
    shebei = new QLabel;
    shebei->setText(tr("设备ID:"));
    sbid = new QLabel;
    sbid->setText(strDevID);
    kjl = new QLabel;
    kjl->setText(tr("      快精灵"));
    time = new QLabel;
    QDateTime im = QDateTime::currentDateTime();
    QString str = im.toString("yyyy-MM-dd hh:mm:ss ddd");
    time->setText(str);
    QTimer* timTm = new QTimer;
    connect(timTm,SIGNAL(timeout()),this,SLOT(TimeNow()));
    timTm->start(1000);
    jiaoshi = new ClickedLabel;
    jiaoshi->setText(tr("<u>校时</u>"));
    jiaoshi->setEnabled(false);
    help = new ClickedLabel;
    help->setText(tr("<u>帮助</u>"));
    connect(help, SIGNAL(clicked()), this, SLOT(OnHelp()));

    QStatusBar* statusBar = this->statusBar();
    statusBar->addWidget(shebei,1);
    statusBar->addWidget(sbid,2);
    statusBar->addWidget(kjl,3);
    statusBar->addWidget(time,4);
    statusBar->addWidget(jiaoshi,5);
    statusBar->addWidget(help,6);
}

void MainWindow::CreateToolbar()
{
    wejian = addToolBar("<u>Wenjian</u>");
}
void MainWindow::onlineStateChange(bool bState)
{
    if(bState)
    {
        QImage NetImage(":/NetConn24.ico");
        this->network->setPixmap(QPixmap::fromImage(NetImage));
    }
    else
    {
        QImage NetImage(":/NetDisConn24.ico");
        this->network->setPixmap(QPixmap::fromImage(NetImage));
    }
}
void MainWindow::OnGengxin()
{
    //@zb Func
    //@Param Ip  From fwqi
//    int ret = QMessageBox::information(this, tr("更新"),
//                                    tr("系统正在登录，请不要插/拔数据线进行安装工作"),
//                                    QMessageBox::NoButton);
    Gengxin* UpLoadManual = new Gengxin;
    UpLoadManual->setWindowModified(Qt::ApplicationModal);
    UpLoadManual->show();
}
void MainWindow::OnWenjian()
{
    wenjian *wj = new wenjian;
    wj->show();
    wj->Data_SqlOpt = Data_SqlOpt;
    wj->Data_SqlOpt->sqlinit();
}
void MainWindow::OnBaobiao()
{
    baobiao *bb = new baobiao;
    bb->show();
    bb->Data_SqlOpt = Data_SqlOpt;
   // MvUpdate->start();
}
void MainWindow::OnFuwuqi()
{
    fuwuqi *fq = new fuwuqi;
    //fuwuqi *f = new fuwuqi("dkalk");
 //   QStromg s = f->gepara();
    fq->show();
}
void MainWindow::OnShangchuan()
{
    //MvUpdate->start();
    Shangchuan *sh = new Shangchuan;
    sh->SetUpThread(this->File_UpLoad);
    sh->show();
}
void MainWindow::OnHelp()
{
//    NetInterfaceList =  QNetworkInterface::allInterfaces();
    Help* HelpDoc = new Help;
    HelpDoc->show();
}
//Scanning USB Dev On Timer 1s
void MainWindow::ScanUsbDev()
{
    //qDebug("ScanUsbDev %d",i++);
//    QString s = QString::number (g_i, 10);
//    this->UsbDevHandle->SetNum(s);
//    if(g_i%2)
//    {
//        this->UsbDevHandle->SetStatus(tr("占用"));
//        this->UsbDevHandle->StartProcBar();
//    }
//    if( g_i >= 50)
//        this->UsbDevHandle->StopProcBar();
//    qDebug()<<g_i<<"\n";
///*    else
//    {
//        this->UsbDevHandle->SetStatus(tr("空闲"));
//        this->UsbDevHandle->StopProcBar();
//    }*/
//    //this->UsbDevHandle->repaint();
//    //test
//    if( g_i== 0 )
//    {
//        UsbState->install_state = 0;
//    }
//    if( g_i >= 10 && g_i <=20)
//    {
//        UsbState->install_state = 1;
//        UsbState->apk_num = (g_i -10);
//        UsbState->apk_total = 10;
//    }
//    if(g_i > 20 && g_i < 25)
//    {
//        UsbState->install_state = 2;
//    }
//    if(g_i >=25 && g_i<30)
//    {
//        UsbState->install_state = 3;
//    }
//    if(g_i >= 30)
//    {
//        UsbState->install_state = 0;
//    }
//   //this->UsbDevHandle3->DevWdgPrecess(UsbState);
//    for(int nCount = 2; nCount < DEVCOUNT; nCount++)
//    {
//        DevArray[nCount]->DevWdgPrecess(UsbState);
//    }
//    g_i++;

    for(int i =0; i < DEVCOUNT; i++)
    {
     //   Global::usb_state[i].install_state = 1;
        DevArray[i]->DevWdgPrecess(&(Global::usb_state[i]));
        if(Global::usb_state[i].install_state == 0 )
            DevArray[i]->SetStatus(tr("空闲"));
        else if(Global::usb_state[i].install_state ==1)
        {
            DevArray[i]->SetStatus(tr("安装中"));
        }
        else if(Global::usb_state[i].install_state == 2)
        {
            DevArray[i]->SetStatus(tr("完成"));
        }
        else if(Global::usb_state[i].install_state == 3)
        {
            DevArray[i]->SetStatus(tr("中断"));
        }
        else if(Global::usb_state[i].install_state == 4)
        {
            DevArray[i]->SetStatus(tr("失败"));
        }
    }
}
void MainWindow::TimeNow()
{
    QDateTime im = QDateTime::currentDateTime();
    QString str = im.toString("yyyy-MM-dd hh:mm:ss ddd");
    this->time->setText(str);
}
void MainWindow::ChangeDevStatus(int i)
{

}
void MainWindow::setMvState(bool bState)
{
    if(bState)
        this->MvUpdate->start();
    else
        this->MvUpdate->stop();
}

int MainWindow::GetAllFiles()
{
    QString strPath = UPLOAD_LOG_PATH;
    QDir dir(strPath);
    QFileInfoList FileInfoLst;
    QStringList strFileLst;
    strFileLst.clear();
    if( !dir.exists() )
    {
        return 0;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);
    FileInfoLst = dir.entryInfoList();
    return FileInfoLst.count();
}
void MainWindow::SetUnUpCount()
{
    int nCount = GetAllFiles();
    this->wscnum->setText(QString::number(nCount, 10));
}
