#ifndef GENGXIN_H
#define GENGXIN_H
#include <QWidget>
#include <QMovie>
#include "dataupdate.h"
#include "upstate.h"

namespace Ui {
class Gengxin;
}

class Gengxin : public QWidget
{
    Q_OBJECT

public:
    explicit Gengxin(QWidget *parent = 0);
    ~Gengxin();

public:
    void SetBtnInvis();
    void StartUpdate();
    void StopUpdate();

private:
    QMovie* MvLoading;

private:
    DataUpdate* DataUpThread;

private slots:
    void OnOk();
    void UpDone();
    void OnCancle();
    void OnTerm();
private:
    Ui::Gengxin *ui;
};

#endif // WIDGET_H
