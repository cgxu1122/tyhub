#ifndef WENJIAN_H
#define WENJIAN_H

#include <QWidget>
#include "data_sql.h"


namespace Ui {
class wenjian;
}

class wenjian : public QWidget
{
    Q_OBJECT

public:
    explicit wenjian(QWidget *parent = 0);
    ~wenjian();
public:
    Data_Sql* Data_SqlOpt;

public:
    QString GetFreeSpace();

private slots:
    void onreturn();
    void OnChangeContent(QString);
    void SetFreeSpace(int);

private:
    void UpdateContent();
    QString GetcardSpace();
    QString GetUpdatetime();
    int GetUpdatenum();

private:
    Ui::wenjian *ui;
};

#endif // WENJIAN_H
