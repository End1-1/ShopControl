#include "sc_goodsqtyamount.h"
#include "ui_sc_goodsqtyamount.h"
#include <QKeyEvent>

SC_GoodsQtyAmount::SC_GoodsQtyAmount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SC_GoodsQtyAmount)
{
    ui->setupUi(this);
    ui->leQty->setFocus();
    ui->leQty->selectAll();
    ui->leQty->installEventFilter(this);
    ui->lePrice->installEventFilter(this);
    ui->leTotal->installEventFilter(this);
}

SC_GoodsQtyAmount::~SC_GoodsQtyAmount()
{
    delete ui;
}

bool SC_GoodsQtyAmount::getData(const QString &goods, double &qty, double &price, QWidget *parent)
{
    SC_GoodsQtyAmount *s = new SC_GoodsQtyAmount(parent);
    s->ui->leName->setText(goods);
    s->ui->leQty->setText(QString::number(qty, 'f', 3));
    s->ui->lePrice->setText(QString::number(price, 'f', 2));
    s->ui->leTotal->setText(QString::number(qty * price, 'f', 2));
    bool result = false;
    if (s->exec() == QDialog::Accepted) {
        qty = s->ui->leQty->text().toDouble();
        price = s->ui->lePrice->text().toDouble();
        result = true;
    }
    delete s;
    return result;
}

void SC_GoodsQtyAmount::on_btnCancel_clicked()
{
    reject();
}

void SC_GoodsQtyAmount::on_btnOK_clicked()
{
    accept();
}

void SC_GoodsQtyAmount::on_leQty_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    disconnect(ui->leTotal, SIGNAL(textChanged(QString)), this, SLOT(on_leTotal_textChanged(QString)));
    ui->leTotal->setText(QString::number(ui->leQty->text().toDouble() * ui->lePrice->text().toDouble(), 'f', 2));
    connect(ui->leTotal, SIGNAL(textChanged(QString)), this, SLOT(on_leTotal_textChanged(QString)));
}


void SC_GoodsQtyAmount::on_leTotal_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    disconnect(ui->lePrice, SIGNAL(textChanged(QString)), this, SLOT(on_lePrice_textChanged(QString)));
    if (ui->leQty->text().toDouble() > 0.01)
        ui->lePrice->setText(QString::number(ui->leTotal->text().toDouble() / ui->leQty->text().toDouble(), 'f', 2));
    connect(ui->lePrice, SIGNAL(textChanged(QString)), this, SLOT(on_lePrice_textChanged(QString)));
}

void SC_GoodsQtyAmount::on_lePrice_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    disconnect(ui->leTotal, SIGNAL(textChanged(QString)), this, SLOT(on_leTotal_textChanged(QString)));
    ui->leTotal->setText(QString::number(ui->leQty->text().toDouble() * ui->lePrice->text().toDouble(), 'f', 2));
    connect(ui->leTotal, SIGNAL(textChanged(QString)), this, SLOT(on_leTotal_textChanged(QString)));
}

bool SC_GoodsQtyAmount::eventFilter(QObject *o, QEvent *e)
{
    bool f = false;
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent*>(e);
        switch (k->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (o == ui->leQty) {
                ui->lePrice->setFocus();
                ui->lePrice->selectAll();
                f = true;
            } else if (o == ui->lePrice) {
                ui->leTotal->setFocus();
                ui->leTotal->selectAll();
                f = true;
            } else if (o == ui->leTotal) {
                ui->btnOK->setFocus();
                f = true;
            }
            break;
        default:
            break;
        }
    }
    if (f)
        return true;
    else
        return QDialog::eventFilter(o, e);
}
