#include "widget.h"
#include "ui_widget.h"
#include "gengxin.h"
#include "ClickedLabel.h"
#include <QPushButton>
#include "wenjian.h"
#include "fuwuqi.h"
#include"baobiao.h"
#include "QPicture"
#include <QPixmap>
#include <QBitmap>
#include<QtDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   // this->showMaximized();


   // QPixmap pixmap("bluebg.jpg");
    QImage image(":/bluebg.jpg");
    ui->label_1->setPixmap(QPixmap::fromImage(image));
    ui->label_1->setFixedSize(100,150);

    ui->label_2->setPixmap(QPixmap::fromImage(image));
    ui->label_2->setFixedSize(100,150);

    ui->label_3->setPixmap(QPixmap::fromImage(image));
    ui->label_3->setFixedSize(100,150);


    //ui->label_1->show();



   CreateHlayout();


}

Widget::~Widget()
{
    delete ui;
}

 void Widget::CreateHlayout()
 {
     /*
     ClickedLabel *gengxin= new ClickedLabel;
     gengxin->setText("<u>gengxin</u>");
     gengxin->setLineWidth(20);
     connect(gengxin,SIGNAL(clicked()),this,SLOT(onShowGengxin()));

     ClickedLabel *wangluo = new ClickedLabel;
     wangluo->setText("<u>wangluo</u>");
     wangluo->setLineWidth(20);
     //connect(wangluo,Clicked(),this,onShowWangluo());

     ClickedLabel *wenjian = new ClickedLabel;
     wenjian->setText("<u>wenjian</u>");
     wenjian->setLineWidth(20);
     connect(wenjian,SIGNAL(clicked()),this,SLOT(onShowWenjian()));

     ClickedLabel *baobiao = new ClickedLabel;
     baobiao->setText("<u>baobiao</u>");
     baobiao->setLineWidth(20);
     connect(baobiao,SIGNAL(clicked()),this,SLOT(onShowBaobiao()));

     ClickedLabel *fuwuqi = new ClickedLabel;
     fuwuqi->setText("<u>fuwuqi</u>");
     fuwuqi->setLineWidth(20);
     connect(fuwuqi,SIGNAL(clicked()),this,SLOT(onShowFuwuqi()));

     QLabel *cangkuid = new QLabel;
     cangkuid->setText("CnagkuID:");
     cangkuid->setLineWidth(20);


     QLabel *ci = new QLabel;
     ci->setText("334");

     QPushButton *shangchuan = new QPushButton;
     shangchuan->setText("shangchuan");
     connect(shangchuan,SIGNAL(clicked()),this,SLOT(onShowShangchuan()));


     QLabel *wsq = new QLabel;
     wsq->setText("weishangchuan:");

     QLabel *ws = new QLabel;
     ws->setText("1");


     ui->topHLayout->addWidget(gengxin);
     ui->topHLayout->addWidget(wangluo);
     ui->topHLayout->addWidget(wenjian);
     ui->topHLayout->addWidget(baobiao);
     ui->topHLayout->addWidget(fuwuqi);
     ui->topHLayout->addWidget(cangkuid);
     ui->topHLayout->addWidget(ci);
     ui->topHLayout->addWidget(shangchuan);
     ui->topHLayout->addWidget(wsq);
     ui->topHLayout->addWidget(ws);
     */

 }



void Widget::onShowGengxin()
{
    Gengxin *gx = new Gengxin;
    gx->show();
}
void Widget::onShowWangluo()
{

}
void Widget::onShowWenjian()
{
    wenjian *wj = new wenjian;
    wj->show();

}
void Widget::onShowBaobiao()
{
    baobiao *bb = new baobiao;
    bb->show();

}
void Widget::onShowFuwuqi()
{
    fuwuqi *fq = new fuwuqi;
    fq->show();
}
void Widget::onShowShangchuan()
{

}


