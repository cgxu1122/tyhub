#ifndef BAOBIAO_H
#define BAOBIAO_H

#include <QWidget>
#include "global.h"
#include <QVector>
#include "data_sql.h"

namespace Ui {
class baobiao;
}

class baobiao : public QWidget
{
    Q_OBJECT

public:
    explicit baobiao(QWidget *parent = 0);
    ~baobiao();
public:
    Data_Sql* Data_SqlOpt;

private slots:
    void OnReturn();
    void OnSearchData();


private:
    Ui::baobiao *ui;
};

#endif // BAOBIAO_H
