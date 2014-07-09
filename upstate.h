#ifndef UPSTATE_H
#define UPSTATE_H

#include <QWidget>

namespace Ui {
class UpState;
}

class UpState : public QWidget
{
    Q_OBJECT

public:
    explicit UpState(QWidget *parent = 0);
    ~UpState();

private:
    bool bDevState;
    bool bPkgState;
    bool bApkState;
public:
    void SetState(bool bDevState, bool bPkgState, bool bApkState);

private:
    Ui::UpState *ui;
private slots:
    void OnConfirm();
};

#endif // UPSTATE_H
