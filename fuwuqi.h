#ifndef FUWUQI_H
#define FUWUQI_H
#define Unicom "192.168.0.1"
#define NET  " 192.168.100.1"

#include <QWidget>
#include <QString>

namespace Ui {
class fuwuqi;
}

class fuwuqi : public QWidget
{
    Q_OBJECT

public:
    explicit fuwuqi(QWidget *parent = 0);
   // fuwuqi(QString string);

    ~fuwuqi();
     QString  getIP(){return IP;}

private slots:
    void OnReturn();
    void OnChangeContent(QString str);
    void OnOK();

private:
    Ui::fuwuqi *ui;
    QString IP;  // to fwqi:genxin @param
};

#endif // FUWUQI_H
