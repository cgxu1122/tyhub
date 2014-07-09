#ifndef SHANGCHUAN_H
#define SHANGCHUAN_H

#include <QWidget>
#include "data_sql.h"
#include "fileupload.h"

namespace Ui {
class Shangchuan;
}

class Shangchuan : public QWidget
{
    Q_OBJECT

public:
    explicit Shangchuan(QWidget *parent = 0);
    ~Shangchuan();
    void SetUpThread(FileUpload* File_Up);

private slots:
    void Return();
    void Search();
    void Upload();
private:
    Data_Sql* Sql_Log;
    FileUpload* File_Up;

private:
    Ui::Shangchuan *ui;
};

#endif // SHANGCHUAN_H
