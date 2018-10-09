#ifndef SC_EDITGOODS_H
#define SC_EDITGOODS_H

#include "sc_dialog.h"

namespace Ui {
class SC_EditGoods;
}

class SC_EditGoods : public SC_Dialog
{
    Q_OBJECT

public:
    explicit SC_EditGoods(QWidget *parent = 0);
    ~SC_EditGoods();
    void load(int id);

private slots:
    void on_pushButton_2_clicked();
    void on_leNewBarcode_returnPressed();
    void on_lstBarcode_customContextMenuRequested(const QPoint &pos);
    void on_actionRemove_barcode_triggered();
    void on_btnSave_clicked();

    void on_actionCopy_triggered();

private:
    Ui::SC_EditGoods *ui;
};

#endif // SC_EDITGOODS_H
