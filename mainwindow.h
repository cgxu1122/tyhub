#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define UPLOAD_LOG_PATH "/tmp/log"

#include <QMainWindow>
#include "ClickedLabel.h"
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSpacerItem>
#include "devwdg.h"
#include <QTimer>
#include "global.h"
#include <QList>
#include <QNetworkInterface>
#include "data_sql.h"
#include "record.h"
#include <QNetworkConfigurationManager>
#include <QVector>
#include <fileupload.h>
#include "gengxin.h"
#include "sqlopt.h"

#define DEVCOUNT 12

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CreateLayout();
    void CreateStatusbar();
    void CreateToolbar();
//@Fzq
public:
    Data_Sql* Data_SqlOpt;
    FILE_CTL* File_Opt;
    FileUpload* File_UpLoad;
    QTimer* UsbDevTimer;
    int g_i;
//    DevWdg* UsbDevHandle;
//    DevWdg* UsbDevHandle3;
//    USB_State* UsbState;
    DevWdg* DevArray[DEVCOUNT];
    //USB_State* UsbState[DEVCOUNT];

private:
    QString strDevID;
    QNetworkConfigurationManager* netConf;
    int  GetAllFiles();


private slots:
    void OnGengxin();
    void OnWenjian();
    void OnBaobiao();
    void OnFuwuqi();
    void OnShangchuan();
    void OnHelp();
    void ScanUsbDev();
    void TimeNow();
    void ChangeDevStatus(int i);
    void onlineStateChange(bool bState);
    void setMvState(bool bState);   //change Mv State
    void SetUnUpCount();

signals:
    void setMvState();

private:
    Ui::MainWindow *ui;

    QToolBar *gexin;
    QToolBar *wejian;

    ClickedLabel *gxin;
    ClickedLabel *wluo;
    ClickedLabel *wjian;
    ClickedLabel *bbiao;
    ClickedLabel *fwuqi;
    QLabel *cangkuid;
    QLabel *id;
    QLabel *network;
    QLabel *neicun;
    QPushButton *shangchuan;
    QLabel *weishangchuan;
    QLabel *wscnum;
    QLabel* LabUpdateState;
    QMovie *MvUpdate;
    QSpacerItem *horizonSpacer1;

    QHBoxLayout *layout ;

    QGridLayout *gridlayout;

    QLabel  *label1;
    QLabel  *label2;
    QLabel  *label3;
    QLabel  *label4;
    QLabel  *label5;
    QLabel  *label6;
    QLabel  *label7;
    QLabel  *label8;
    QLabel  *label9;
    QLabel  *label10;
    QLabel  *label11;
    QLabel  *label12;


    QLabel *shebei;
    QLabel *sbid;
    QLabel *kjl;
    QLabel *time;
    QLabel *jiaoshi;
    ClickedLabel *help;
    QHBoxLayout *statusbox;
};

#endif // MAINWINDOW_H
