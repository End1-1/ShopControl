#include "sc_storedoc.h"
#include "ui_sc_storedoc.h"
#include "sc_goodsqtyamount.h"
#include "common.h"
#include <QInputDialog>
#include <QMessageBox>

SC_StoreDoc::SC_StoreDoc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SC_StoreDoc)
{
    ui->setupUi(this);
    ui->tblData->setColumnCount(5);
    QList<int> w;
    w << 0 << 300 << 80 << 80 << 80;
    for (int i = 0; i < ui->tblData->columnCount(); i++)
        ui->tblData->setColumnWidth(i, w.at(i));
    QStringList h;
    h << "" << tr("Name") << tr("Qty") << tr("Price") << tr("Total");
    ui->tblData->setHorizontalHeaderLabels(h);
    ui->deDate->setDate(QDate::currentDate());
    m_id = 0;
    ui->cbDocType->addItem(tr("In"), 2);
    ui->cbDocType->addItem(tr("Out"), 3);
    ui->cbDocType->setCurrentIndex(0);

    ui->cbPayment->addItem(tr("Cash"));
    ui->cbPayment->addItem(tr("No cash"));

    db.openDB();
    db.execSQL("select id, llc from partners");
    while (db.next()) {
        ui->cbPartner->addItem(db.v_str(1), db.v_int(0));
    }
    db.closeDB();
}

SC_StoreDoc::~SC_StoreDoc()
{
    delete ui;
}

void SC_StoreDoc::load(int id)
{
    if (!db.openDB()) {
        return;
    }
    if (!db.prepare("select fdate, ftype, fpartner, fpaid, finvoice, fpayment from sth where id=:id")) {
        return;
    }
    db.bindValue(":id", id);
    if (!db.execSQL()) {
        return;
    }
    if (db.next()) {
        ui->deDate->setDate(db.v_date(0));
        ui->cbDocType->setCurrentIndex(ui->cbDocType->findData(db.v_int(1)));
        ui->cbPartner->setCurrentIndex(db.v_int(5) - 1);
        ui->cbPartner->setCurrentIndex(ui->cbPartner->findData(db.v_int(2)));
        ui->leInvoice->setText(db.v_str(4));
        ui->lePaid->setText(db.v_str(3));
    }
    if (!db.prepare("select t.id, f.fname, t.fqty, t.fprice, t.fgoods from stb t "
                    "left join goods f on f.id=t.fgoods "
                    "where t.fdoc=:fdoc")) {
        return;
    }
    db.bindValue(":fdoc", id);
    if (!db.execSQL()) {
        return;
    }
    while (db.next()) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row +1);
        ui->tblData->setItem(row, 0, new QTableWidgetItem(db.v_str(4)));
        ui->tblData->setItem(row, 1, new QTableWidgetItem(db.v_str(1)));
        ui->tblData->setItem(row, 2, new QTableWidgetItem(db.v_str(2)));
        ui->tblData->setItem(row, 3, new QTableWidgetItem(db.v_str(3)));
        ui->tblData->setItem(row, 4, new QTableWidgetItem(float_str(db.v_dbl(2) * db.v_dbl(3), 0)));
    }
    m_id = id;
    db.closeDB();
    countAmount();
}

QString SC_StoreDoc::title()
{
    return tr("Store document");
}

void SC_StoreDoc::on_actionBarcode_triggered()
{
    QString barcode = QInputDialog::getText(this, tr("Barcode"), tr("Barcode"));
    if (!barcode.length())
        return;
    if (!db.openDB())
        return;
    if (!db.prepare("select g.id, g.fname, g.fprice2 from goods g, barcode b where g.id=b.fgoods and b.fbarcode=:fbarcode"))
        return;
    db.bindValue(":fbarcode", barcode);
    if (!db.execSQL())
        return;
    if (db.next()) {
        int row = ui->tblData->rowCount();
        ui->tblData->setRowCount(row + 1);
        for (int i = 0; i < ui->tblData->columnCount(); i++)
            ui->tblData->setItem(row, i, new QTableWidgetItem());
        ui->tblData->item(row, 0)->setData(Qt::EditRole, db.v_int(0));
        ui->tblData->item(row, 1)->setData(Qt::EditRole, db.v_str(1));
        ui->tblData->item(row, 2)->setData(Qt::EditRole, 0);
        ui->tblData->item(row, 3)->setData(Qt::EditRole, db.v_dbl(2));
        ui->tblData->item(row, 4)->setData(Qt::EditRole, 0);
        ui->tblData->setCurrentCell(row, 0);
        setData(row);
    } else
        QMessageBox::critical(this, tr("Error"), tr("Barcode is not exists"));
    db.closeDB();
}

void SC_StoreDoc::setData(int row)
{
    double qty = ui->tblData->item(row, 2)->data(Qt::EditRole).toDouble();
    double price = ui->tblData->item(row, 3)->data(Qt::EditRole).toDouble();
    QString name = ui->tblData->item(row, 1)->data(Qt::EditRole).toString();
    if (!SC_GoodsQtyAmount::getData(name, qty, price, this))
        return;
    ui->tblData->item(row, 2)->setData(Qt::EditRole, qty);
    ui->tblData->item(row, 3)->setData(Qt::EditRole, price);
    ui->tblData->item(row, 4)->setData(Qt::EditRole, qty * price);
    countAmount();
}

void SC_StoreDoc::countAmount()
{
    double amount = 0.0;
    for (int i = 0; i < ui->tblData->rowCount(); i++)
        amount += ui->tblData->item(i, 4)->data(Qt::EditRole).toDouble();
    ui->leTotal->setText(QString::number(amount, 'f', 2));
}

void SC_StoreDoc::on_tblData_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    setData(index.row());
}

void SC_StoreDoc::on_actionSave_triggered()
{
    if (!db.openDB())
        return;
    if (!m_id) {
        if (!(m_id = db.genId("gen_sth_id")))
            return;
        if (!db.prepare("insert into sth (id) values (:id)"))
            return;
        db.bindValue(":id", m_id);
        if (!db.execSQL())
            return;
    }
    if (!db.prepare("update sth set fdate=:fdate, fpartner=:fpartner, ftimestamp=current_timestamp, famount=:famount, ftype=:ftype, "
                    "fpaid=:fpaid, finvoice=:finvoice, fpayment=:fpayment where id=:id"))
        return;
    db.bindValue(":fdate", ui->deDate->date());
    db.bindValue(":fpartner", ui->cbPartner->currentData());
    db.bindValue(":famount", ui->leTotal->text().toDouble());
    db.bindValue(":ftype", ui->cbDocType->currentData());
    db.bindValue(":fpaid", ui->lePaid->text().toDouble());
    db.bindValue(":fpayment", ui->cbPayment->currentIndex() + 1);
    db.bindValue(":finvoice", ui->leInvoice->text());
    db.bindValue(":id", m_id);
    if (!db.execSQL())
        return;
    if (!db.prepare("delete from stb where fdoc=:fdoc"))
        return;
    db.bindValue(":fdoc", m_id);
    if (!db.execSQL())
        return;
    if (!db.prepare("insert into stb (fdoc, fgoods, fqty, fprice) values (:fdoc, :fgoods, :fqty, :fprice)"))
        return;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        db.bindValue(":fdoc", m_id);
        db.bindValue(":fgoods", ui->tblData->item(i, 0)->data(Qt::EditRole));
        db.bindValue(":fqty", ui->tblData->item(i, 2)->data(Qt::EditRole));
        db.bindValue(":fprice", ui->tblData->item(i, 3)->data(Qt::EditRole));
        if (!db.execSQL())
            return;
    }
    db.closeDB();
    QMessageBox::information(this, tr("Information"), tr("Saved"));
}

void SC_StoreDoc::on_actionDelete_row_triggered()
{
    QModelIndexList l = ui->tblData->selectionModel()->selectedRows();
    if (!l.count())
        return;
    for (int i = l.count() - 1; i > -1; i--)
        ui->tblData->removeRow(l.at(i).row());
    countAmount();
}

