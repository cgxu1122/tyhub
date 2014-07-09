#include "wenjian.h"
#include "ui_wenjian.h"
#include <QDate>
#include <QPalette>
#include <QProcess>
#include <QtDebug>

wenjian::wenjian(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wenjian)
{
    ui->setupUi(this);
    //this->showMaximized();
    this->setWindowTitle(tr("文件"));
   // this->setFixedSize(640,500);
    ui->comboBox->addItem("1222");
    ui->comboBox->addItem("3333");
    connect(ui->comboBox,SIGNAL(activated(QString)),this,SLOT(OnChangeContent(QString)));
    //GetFreeSpace();
    QProcess* pDf = new QProcess;
    connect(pDf, SIGNAL(finished(int)), this, SLOT(SetFreeSpace(int)));
    //pDf->start(tr("df"),QStringList()<<"-h");
    pDf->start("df -h");
    UpdateContent();
}

wenjian::~wenjian()
{
    delete ui;
}

void wenjian::onreturn()
{
    this->close();
}

void wenjian::UpdateContent()
{

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::blue);

    ui->cardspaceLabel->setPalette(pe);
    ui->ldateLabel->setPalette(pe);
    ui->updatenumLabel->setPalette(pe);

    QString cdspace = GetcardSpace();
    QString datestr = GetUpdatetime();
    int updatenum = GetUpdatenum();
    QString num = QString("%1").arg(updatenum);


    ui->cardspaceLabel->setText(cdspace);
    ui->ldateLabel->setText(datestr);
    ui->updatenumLabel->setText(num);
}

QString wenjian::GetcardSpace()
{
    return QString(tr("15.69"));
}
QString wenjian::GetUpdatetime()
{
    QDate date = QDate::currentDate();
    QString rd = date.toString("yyyy-MM-dd");
    return rd;
}
int wenjian::GetUpdatenum()
{
    int num = 19;
    return num;
}

QString wenjian::GetFreeSpace()
{
    QProcess* pDf = new QProcess;
    connect(pDf, SIGNAL(finished(int)), this, SLOT(SetFreeSpace(int)));
    //pDf->start(tr("df"),QStringList()<<"-h");
    pDf->start("df -h");
//    if(NULL != pDf)
//    {
//        delete pDf;
//        pDf = NULL;
//    }
}
void wenjian::SetFreeSpace(int nCode)
{
    QProcess* pRes =( QProcess*)sender();
    QString strRes = pRes->readAll();
    qDebug()<<strRes;
    QStringList strLstLine = strRes.split("\n");
    qDebug()<<strLstLine;
    QStringList strLstMnt;
    strLstMnt = strLstLine.filter("run");/////////////@
    qDebug()<<strLstMnt;
    QList<QString> strLstOneSpace;
    double dFreeSpace = 0;
    for(int i = 0; i<strLstMnt.size(); i++)
    {
        strLstOneSpace.append(strLstMnt[i].simplified());
        qDebug()<<strLstOneSpace[i];
        QString strTmp = strLstMnt[i].simplified();
        QStringList strLstOneSplit = strTmp.split(" ");
        QString strNum = strLstOneSplit[3].left(strLstOneSplit[3].size()-1);
        QString strUnit = strLstOneSplit[3].right(1);
        if(strUnit == "G"){
              dFreeSpace += strNum.toDouble();
         }else{
               if(strUnit == "M")
                   dFreeSpace += strNum.toDouble()/1024;
               else
                    dFreeSpace += strNum.toDouble()/(1024*1024);
          }
    }
    QString strFreeSpace = QString::number(dFreeSpace, 'f', 4);
   ui->cardspaceLabel->setText(strFreeSpace);
}
void wenjian::OnChangeContent(QString str)
{}
//@Add Func1
//@Param (Vector<String> &)

//@Add Func2
//@Param (String, Data & , int &)
