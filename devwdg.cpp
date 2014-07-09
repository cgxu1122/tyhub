#include "devwdg.h"
#include "ui_devwdg.h"
#include <qpalette.h>

DevWdg::DevWdg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevWdg)
{
    ui->setupUi(this);
    nProgBarValue = 0;
    this->setAutoFillBackground(true);
   QPalette Palette;
   Palette.setBrush(QPalette::Background, QBrush(QPixmap(":/bluebg.jpg")));
   this->setPalette(Palette);
   this->setFixedSize(100,190);
   Palette.setColor(QPalette::WindowText, Qt::white);
   ui->labNum->setText(tr(""));
   ui->labStatus->setText(tr(""));
   ui->labNum->setPalette(Palette);
   ui->labStatus->setPalette(Palette);
   ui->labPerc->setPalette(Palette);
   ui->progBar_Install->setTextVisible(false);
   ui->progBar_Install->setVisible(false);
   ui->labPerc->setVisible(false);
   timPerc = new QTimer;;
   timProg = new QTimer;
 /*  if(this->strStatus == tr("空闲"))
   {
       ui->progBar_Install->setVisible(false);
       ui->labPerc->setVisible(false);
   }
   else
   {
       ui->progBar_Install->setVisible(true);
       ui->labPerc->setVisible(true);
   }*/
}

void DevWdg::SetNum(QString strNum)
{
    this->strNum = strNum;
    ui->labNum->setText(strNum);
    ui->labNum->repaint();
}
void DevWdg::SetStatus(QString strStatus) //End Use
{
    this->strStatus = strStatus;
    ui->labStatus->setText(strStatus);
    ui->labStatus->repaint();
}
void DevWdg::SetApkNum(int nIns, int nTotal)
{
    this->nInsApk = nIns;
    this->nTotalApk = nTotal;
}
void DevWdg::StartPercLab()
{
    ui->labPerc->setVisible(true);
/*    QString strIns = QString::number(this->nInsApk,10);
    QString strTotal = QString::number(this->nTotalApk,10);
    QString strLab = strIns + tr("/") +strTotal;
    ui->labPerc->setText(strLab);*/
    connect(this->timPerc,SIGNAL(timeout()),this,SLOT(percRun()));
    timPerc->start(100);
}
void DevWdg::StopPercLab()
{
    if( this->strStatus == tr("安装中") && timPerc->isActive())
    {
        timPerc->stop();
        QString strTotal = QString::number(this->nTotalApk,10);
        QString strLab = strTotal + tr("/") + strTotal;
        ui->labPerc->setText(strLab);
    }
    else
    {
        ui->labPerc->setVisible(false);
    }
}
void DevWdg::StartProcBar()
{
        ui->progBar_Install->setVisible(true);
        ui->progBar_Install->setRange(0,99);
        connect(this->timProg,SIGNAL(timeout()),this,SLOT(progRun()));
        timProg->start(100);
}
void DevWdg::StopProcBar()
{
    if( this->strStatus == tr("安装中"))
    {
        if(timProg->isActive())
             timProg->stop();
        ui->progBar_Install->setValue(99);
        nProgBarValue = 0;
    }
    else
    {
        ui->progBar_Install->setVisible(false);
    }
}
void DevWdg::progRun()
{
        nProgBarValue++;
        if( nProgBarValue >=99)
            nProgBarValue = 0;
        ui->progBar_Install->setValue(nProgBarValue);
        ui->progBar_Install->repaint();
}
void DevWdg::percRun()
{
    this->SetApkNum((int)this->usbState->apk_num ,(int)usbState->apk_total);
    QString strIns = QString::number(this->nInsApk,10);
    QString strTotal = QString::number(this->nTotalApk,10);
    QString strLab = strIns + tr("/") +strTotal;
    ui->labPerc->setText(strLab);
    ui->labPerc->repaint();
}
void DevWdg::DevWdgPrecess(USB_State* usbState)
{
    this->usbState = usbState;
    if((this->strStatus == tr("空闲") && usbState->install_state ==1) || (this->strStatus == tr("中断") && usbState->install_state ==1))
    {
        //设置状态
        this->SetStatus(tr("安装中"));
        //启动进度条
        this->StartProcBar();
        //启动安装数
        this->StartPercLab();
    }
    if(usbState->install_state == 2)
    {
        this->SetStatus(tr("完成"));
        this->StopProcBar();
        this->StopPercLab();
    }
    if(usbState->install_state == 0)
    {
        this->SetStatus(tr("空闲"));
        ui->progBar_Install->setVisible(false);
        ui->labPerc->setVisible(false);
    }
    if(usbState->install_state == 3)
    {
        this->SetStatus(tr("中断"));
        this->StopProcBar();
        this->StopPercLab();
        ui->progBar_Install->setVisible(false);
        ui->labPerc->setVisible(false);
    }
    if(usbState->install_state == 4)
    {
        this->SetStatus(tr("失败"));
        this->StopProcBar();
        this->StopPercLab();
        ui->progBar_Install->setVisible(false);
        ui->labPerc->setVisible(false);
    }
}

DevWdg::~DevWdg()
{
    if(this->timPerc != NULL)
    {
        delete timPerc;
        timPerc = NULL;
    }
    if(this->timProg != NULL)
    {
        delete timProg;
        timProg = NULL;
    }
    delete ui;
}
