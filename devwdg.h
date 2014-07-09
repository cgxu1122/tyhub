#ifndef DEVWDG_H
#define DEVWDG_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include "global.h"

namespace Ui {
class DevWdg;
}

class DevWdg : public QWidget
{
    Q_OBJECT
    
public:
    explicit DevWdg(QWidget *parent = 0);
    ~DevWdg();
    
public:
    void SetNum(QString strNum);
    void SetStatus(QString strStatus);
    void StartProcBar();
    void StopProcBar();
    void SetApkNum(int nIns, int nTotal);
    void StartPercLab();
    void StopPercLab();
    void DevWdgPrecess(USB_State* usbState);

private:
    Ui::DevWdg *ui;
    QString strNum;
    QString strStatus;
    int nTotalApk;
    int nInsApk;
    int nProgBarValue;
    QTimer* timProg;
    QTimer* timPerc;
    USB_State* usbState;

private slots:
    void progRun();
    void percRun();

};

#endif // DEVWDG_H
