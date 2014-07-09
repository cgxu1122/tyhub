#include "baobiao.h"
#include "ui_baobiao.h"
#include <QTableWidgetItem>

baobiao::baobiao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::baobiao)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0,"机型");
    ui->tabWidget->setTabText(1,"usb口");
    ui->JixingTab->setColumnWidth(0,150);
    ui->JixingTab->setColumnWidth(1,350);
    ui->JixingTab->setColumnWidth(2,210);
    ui->JixingTab->setRowCount(12);
    //ui->JixingTab->insertRow(0);

    ui->UsbTab->setColumnWidth(0,150);
    ui->UsbTab->setColumnWidth(1,350);
    ui->UsbTab->setColumnWidth(2,210);
    ui->UsbTab->setRowCount(12);
   // ui->UsbTab->insertRow(0);


    //set the date display policy
    ui->FromdateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->FromdateEdit->setCalendarPopup(true);
    ui->FromdateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->FromdateEdit->setDate(QDate::currentDate());
    ui->TodateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->TodateEdit->setCalendarPopup(true);
    ui->TodateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->TodateEdit->setDate(QDate::currentDate());

    ui->searchBtn->setAutoFillBackground(true);
    //ui->searchBtn->setStyleSheet("QPushButton{border:3px solid green;border-radius:8px}");

    this->setWindowTitle(tr("报表"));
    this->setFixedSize(800,480);

}

baobiao::~baobiao()
{
    delete ui;
}

void baobiao::OnReturn()
{
    this->close();
}
void baobiao::OnSearchData()
{
    ui->JixingTab->clear();
    ui->JixingTab->setHorizontalHeaderLabels(QStringList()<< tr("日期")<<tr("机型")<<tr("安装数量"));
    ui->UsbTab->clear();
    ui->UsbTab->setHorizontalHeaderLabels(QStringList()<<tr("日期")<<tr("USB口")<<tr("安装数量"));
    vector<Req_Data> res;
    res.clear();
    string dateFrom = ui->FromdateEdit->text().toStdString();
    string dateTo = ui->TodateEdit->text().toStdString();
    int nMod = 0;
    if( ui->tabWidget->currentIndex() == 0)
        nMod = 0;
    if( ui->tabWidget->currentIndex() == 1)
        nMod = 1;
    this->Data_SqlOpt->db_get(dateFrom, dateTo, nMod, &res);
    for (int nRow = 0; nRow < res.size(); nRow++)
    {
        for( int nCol = 0; nCol < 3; nCol++)
        {
            QTableWidgetItem* newItem = NULL;
            switch(nCol)
            {
            case 0:
                newItem = new QTableWidgetItem(QString::fromStdString(res[nRow].date.c_str()));
                break;
             case 1:
                newItem = new QTableWidgetItem(QString::fromStdString(res[nRow].id.c_str()));
                break;
             case 2:
                 QString ss = QString("%1").arg(res[nRow].num);
                 newItem = new QTableWidgetItem(ss);
                 break;
            }
            if( nMod == 0)
                ui->JixingTab->setItem(nRow,nCol,newItem);
            if( nMod == 1)
                ui->UsbTab->setItem(nRow,nCol,newItem);
          }
    }
    res.clear();
}
