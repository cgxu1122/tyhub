#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class ClickedLabel : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
 //   void clicked(int x);

public:
    ClickedLabel(QWidget *parent=0):QLabel(parent)
    {
    }

    ~ClickedLabel(){}
protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        if(ev->button() == Qt::LeftButton )
        {
             emit clicked();
        }
    }
};

#endif // CLICKEDLABEL_H
