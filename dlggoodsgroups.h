#ifndef DLGGOODSGROUPS_H
#define DLGGOODSGROUPS_H

#include "sc_dialog.h"

namespace Ui {
class DlgGoodsGroups;
}

class DlgGoodsGroups : public SC_Dialog
{
    Q_OBJECT

public:
    explicit DlgGoodsGroups(QWidget *parent = 0);
    ~DlgGoodsGroups();
    static void group(int id = 0);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgGoodsGroups *ui;
    void load(int id);
};

#endif // DLGGOODSGROUPS_H
