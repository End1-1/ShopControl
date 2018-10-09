#ifndef DLGCHANGEPASS_H
#define DLGCHANGEPASS_H

#include <QDialog>

namespace Ui {
class DlgChangePass;
}

class DlgChangePass : public QDialog
{
    Q_OBJECT

public:
    explicit DlgChangePass(QWidget *parent = 0);
    ~DlgChangePass();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgChangePass *ui;
};

#endif // DLGCHANGEPASS_H
