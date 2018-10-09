#ifndef DLGCHANGESALE_H
#define DLGCHANGESALE_H

#include "sc_dialog.h"

namespace Ui {
class DlgChangeSale;
}

class DlgChangeSale : public SC_Dialog
{
    Q_OBJECT

public:
    explicit DlgChangeSale(QWidget *parent = 0);
    ~DlgChangeSale();
    static void changeSale();
private slots:
    void on_leCode1_returnPressed();

    void on_leCode2_returnPressed();

    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::DlgChangeSale *ui;
    int fId1;
    int fId2;
};

#endif // DLGCHANGESALE_H
