#ifndef SC_GOODSQTYAMOUNT_H
#define SC_GOODSQTYAMOUNT_H

#include <QDialog>
#include <QEvent>

namespace Ui {
class SC_GoodsQtyAmount;
}

class SC_GoodsQtyAmount : public QDialog
{
    Q_OBJECT

public:
    explicit SC_GoodsQtyAmount(QWidget *parent = 0);
    ~SC_GoodsQtyAmount();
    static bool getData(const QString &goods, double &qty, double &price, QWidget *parent = 0);

private slots:
    void on_btnCancel_clicked();
    void on_btnOK_clicked();
    void on_leQty_textChanged(const QString &arg1);
    void on_leTotal_textChanged(const QString &arg1);
    void on_lePrice_textChanged(const QString &arg1);

private:
    Ui::SC_GoodsQtyAmount *ui;

protected:
    bool eventFilter(QObject *o, QEvent *e);
};

#endif // SC_GOODSQTYAMOUNT_H
