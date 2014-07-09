#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("帮助"));
    this->setFixedSize(800,480);
    this->connect(ui->retBtn,SIGNAL(clicked()),this,SLOT(OnBack()));
}

void Help::OnBack()
{
    this->close();
}
Help::~Help()
{
    delete ui;
}
