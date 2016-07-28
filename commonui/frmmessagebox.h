#ifndef FRMMESSAGEBOX_H
#define FRMMESSAGEBOX_H

#include "basedialog.h"

namespace Ui {
class frmMessageBox;
}

class frmMessageBox : public BaseDialog
{
    Q_OBJECT

public:
    explicit frmMessageBox(QWidget *parent = 0);
    ~frmMessageBox();

    void SetMessage(const QString &msg, int type);

private slots:
    void on_btnOk_clicked();

private:
    Ui::frmMessageBox *ui;
};

#endif // FRMMESSAGEBOX_H
