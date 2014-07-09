#include "fuwuqi.h"
#include "ui_fuwuqi.h"
#include <QImage>
#include <QPixmap>
#include "global.h"
#include <QProcess>
#include <QMessageBox>

fuwuqi::fuwuqi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fuwuqi)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("服务器"));
    ui->severcomboBox->addItem(tr("电信"));
    ui->severcomboBox->addItem(tr("联通"));
    connect(ui->severcomboBox,SIGNAL(activated(QString)),this,SLOT(OnChangeContent(QString)));
    this->setWindowTitle(tr("服务器"));
    this->setFixedSize(800,480);
}

fuwuqi::~fuwuqi()
{
    delete ui;
}

 void fuwuqi::OnReturn()
 {
     this->close();
 }


 void fuwuqi::OnChangeContent(QString str)
 {
     if(str.compare(tr("联通")) == 0)
          IP=Unicom;
     else
         IP=NET;
     ui->severshowLabel->setText(str);
     Global::g_IP = this->IP;
 }
 void fuwuqi::OnOK()
 {
     int exitCode;
     IP = "127.0.0.1";
     #ifdef Q_OS_WIN
        QString strArg = tr("ping ") + IP + tr(" -n 1 -i 2");
        exitCode = QProcess::execute(strArg);
     #else  //其他平台(Linux或Mac)
        QString strArg = tr("ping ") + IP + tr(" -c 1 -t 2");
        exitCode = QProcess::execute(strArg);
     #endif
     if( 0 == exitCode )
            QMessageBox::information(this, tr("网络状态"),
                                                tr("所选网络通信正常"),
                                                QMessageBox::NoButton);
     else
         QMessageBox::information(this, tr("网络状态"),
                                    tr("所选网络通信异常"),
                                    QMessageBox::NoButton);
     this->close();
 }
