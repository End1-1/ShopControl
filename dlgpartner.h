#ifndef DLGPARTNER_H
#define DLGPARTNER_H

#include "sc_dialog.h"

namespace Ui {
class DlgPartner;
}

class DlgPartner : public SC_Dialog
{
    Q_OBJECT

public:
    explicit DlgPartner(QWidget *parent = 0);
    ~DlgPartner();
    static void edit(int id);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgPartner *ui;
};

#endif // DLGPARTNER_H
