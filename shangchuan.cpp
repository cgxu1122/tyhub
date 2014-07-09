#include "shangchuan.h"
#include "ui_shangchuan.h"

Shangchuan::Shangchuan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Shangchuan)
{
    ui->setupUi(this);

    //set the date display policy
    ui->FromDataEdt->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->FromDataEdt->setCalendarPopup(true);
    ui->FromDataEdt->setDisplayFormat("yyyy-MM-dd");
    ui->FromDataEdt->setDate(QDate::currentDate());

    ui->ToDataEdt->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->ToDataEdt->setCalendarPopup(true);
    ui->ToDataEdt->setDisplayFormat("yyyy-MM-dd");
    ui->ToDataEdt->setDate(QDate::currentDate());

    ui->ResTabWid->setColumnWidth(0,150);
    ui->ResTabWid->setColumnWidth(1,350);
    ui->ResTabWid->setColumnWidth(2,210);
    ui->ResTabWid->setRowCount(12);
    this->setWindowTitle(tr("上传"));
    this->setFixedSize(800,480);
    this->Sql_Log = new Data_Sql;
    this->Sql_Log->sqlinit();
}

Shangchuan::~Shangchuan()
{
    delete ui;
    this->Sql_Log->sqlclose();
    if( Sql_Log != NULL)
    {
        delete Sql_Log;
        Sql_Log = NULL;
    }
}
void Shangchuan::Return()
{
    this->close();
}
void Shangchuan::Search()
{
    ui->ResTabWid->clear();
    ui->ResTabWid->setHorizontalHeaderLabels(QStringList()<< tr("日期")<<tr("上传文件")<<tr("上传结果"));
    vector<Req_Log> res;
    res.clear();
    string dateFrom = ui->FromDataEdt->text().toStdString();
    string dateTo = ui->ToDataEdt->text().toStdString();
    Sql_Log->db_log_get(dateFrom,dateTo,&res);
    for (int nRow = 0; nRow < res.size(); nRow++)
    {
        for( int nCol = 0; nCol < 3; nCol++)
        {
            QTableWidgetItem* newItem = NULL;
            switch(nCol)
            {
            case 0:
                newItem = new QTableWidgetItem(QString::fromStdString(res[nRow].id.c_str()));
                break;
             case 1:
                {
                    QString strTotal = QString("%1").arg(res[nRow].log_total);
                    newItem = new QTableWidgetItem(strTotal);
                    break;
                }
             case 2:
                {
                    QString strDone = QString("%1").arg(res[nRow].log_upload);
                    newItem = new QTableWidgetItem(strDone);
                    break;
                }
            }
           ui->ResTabWid->setItem(nRow,nCol,newItem);
          }
    }
    res.clear();
}
void Shangchuan::Upload()
{
    File_Up->terminate();
    File_Up->start();
}
void Shangchuan::SetUpThread(FileUpload* File_Up)
{
    this->File_Up = File_Up;
}
//@Func Search
//@Param (DataTime String, DataTime String, Vector<Struct> &)

//@Func Update
//@Param (int &Suc, int &Tol)
