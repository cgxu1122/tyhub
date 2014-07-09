#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void CreateHlayout();


private slots:
    void onShowGengxin();
    void onShowWangluo();
    void onShowWenjian();
    void onShowBaobiao();
    void onShowFuwuqi();
    void onShowShangchuan();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
