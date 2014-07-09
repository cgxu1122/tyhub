#include "gengxin.h"
#include "ui_gengxin.h"
#include <QtDebug>


Gengxin::Gengxin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gengxin)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("更新"));
    ui->labUp->setText(tr("系统正在登录中...\n请不要插/拔数据线进行安装工作或进行其他操作"));
    MvLoading = new QMovie(":/loading.gif");
    ui->labMv->setMovie(MvLoading);
    this->setWindowFlags(Qt::WindowCancelButtonHint);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);
    this->setWindowFlags(Qt::WindowMaximizeButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->ui->pushBtnTerm->setVisible(false);
}

Gengxin::~Gengxin()
{
    delete ui;
}

void Gengxin::SetBtnInvis()
{
    ui->ConfirmBtn->setVisible(false);
    ui->CancleBtn->setVisible(false);
    ui->pushBtnTerm->setVisible(true);
}
void Gengxin::OnOk()
{
    ui->ConfirmBtn->setVisible(false);
    ui->CancleBtn->setVisible(false);
    ui->pushBtnTerm->setVisible(true);
    StartUpdate();
    qDebug()<<"Updating...";
}
void Gengxin::OnTerm()
{
//    StopUpdate();
    UpState* State = new UpState;
    State->SetState(DataUpThread->GetDevState(),DataUpThread->GetPkgState(),DataUpThread->GetApkState());
    State->show();
    StopUpdate();

    this->close();
}
void Gengxin::OnCancle()
{
    this->close();
}
void Gengxin::UpDone()
{
    UpState* State = new UpState;
    State->SetState(DataUpThread->GetDevState(),DataUpThread->GetPkgState(),DataUpThread->GetApkState());
    State->show();
    DataUpThread->terminate();
    DataUpThread->deleteLater();
    this->close();
}
void Gengxin::StopUpdate()
{
    DataUpThread->terminate();
    DataUpThread->deleteLater();
}

void Gengxin::StartUpdate()
{
    //to do
    MvLoading->start();
    DataUpThread = new DataUpdate;
    DataUpThread->moveToThread(QApplication::instance()->thread());
    DataUpThread->start();
    //DataUpThread->wait();
    connect(DataUpThread,SIGNAL(CloseUp()),this,SLOT(UpDone()));
}

