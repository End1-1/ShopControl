#include "sc_editgoods.h"
#include "ui_sc_editgoods.h"
#include <QMenu>
#include <QMessageBox>
#include <QClipboard>

SC_EditGoods::SC_EditGoods(QWidget *parent) :
    SC_Dialog(parent),
    ui(new Ui::SC_EditGoods)
{
    ui->setupUi(this);
    ui->lePrice->setValidator(new QIntValidator());
    ui->lePrice2->setValidator(new QIntValidator());
}

SC_EditGoods::~SC_EditGoods()
{
    delete ui;
}

void SC_EditGoods::load(int id)
{
    if (!db.openDB())
        return;
    db.prepare("select fgroup, fname, fprice, fprice2, fstate from goods where id=:id");
    db.bindValue(":id", id);
    db.execSQL();
    if (db.next()) {
        ui->leCode->setInt(id);
        ui->cbGroup->setCurrentId(db.v_int(0));
        ui->leName->setText(db.v_str(1));
        ui->lePrice->setInt(db.v_int(2));
        ui->lePrice2->setInt(db.v_int(3));
        ui->cbState->setCurrentId(db.v_int(4));
        //ui->cbMeas->setCurrentId(v_int(5));
    }
    db.prepare("select fbarcode from barcode where fgoods=:fgoods");
    db.bindValue(":fgoods", id);
    db.execSQL();
    while (db.next()) {
        QListWidgetItem *item = new QListWidgetItem(ui->lstBarcode);
        item->setText(db.v_str(0));
        ui->lstBarcode->addItem(item);
    }
    db.prepare("select fqueue from quick_goods where fgoods=:fgoods");
    db.bindValue(":fgoods", id);
    db.execSQL();
    if (db.next())
        ui->leQuickList->setInt(db.v_int(0));
    db.closeDB();
}

void SC_EditGoods::on_pushButton_2_clicked()
{
    reject();
}

void SC_EditGoods::on_leNewBarcode_returnPressed()
{
    QString str = ui->leNewBarcode->text().trimmed();
    if (!str.length())
        return;
    if (ui->lstBarcode->findItems(str, Qt::MatchExactly).count()) {
        QMessageBox::warning(this, tr("Error"), tr("Already exists"));
        ui->leNewBarcode->clear();
        ui->leNewBarcode->setFocus();
        return;
    }
    ui->lstBarcode->addItem(str);
    ui->leNewBarcode->clear();
    ui->leNewBarcode->setFocus();
}

void SC_EditGoods::on_lstBarcode_customContextMenuRequested(const QPoint &pos)
{
    QMenu *m = new QMenu();
    m->addAction(ui->actionRemove_barcode);
    m->addAction(ui->actionCopy);
    m->exec(ui->lstBarcode->mapToGlobal(pos));
}

void SC_EditGoods::on_actionRemove_barcode_triggered()
{
    QList<QListWidgetItem*> rows = ui->lstBarcode->selectedItems();
    if (!db.openDB())
        return;
    db.prepare("delete from barcode where fbarcode=:fbarcode");
    for (QList<QListWidgetItem*>::iterator it = rows.begin(); it != rows.end(); it++) {
        db.bindValue(":fbarcode", (*it)->text());
        db.execSQL();
        delete *it;
    }
    db.closeDB();
}

void SC_EditGoods::on_btnSave_clicked()
{
    if (!db.openDB())
        return;
    if (!ui->leCode->toInt()) {
        ui->leCode->setInt(db.genId("GOODS_ID_GEN"));
        db.prepare("insert into goods (id) values (:id)");
        db.bindValue(":id", ui->leCode->toInt());
        db.execSQL();
    }
    db.prepare("update goods set fgroup=:fgroup, fname=:fname, fprice=:fprice, fprice2=:fprice2, fstate=:fstate where id=:id");
    db.bindValue(":fgroup", ui->cbGroup->currentId());
    db.bindValue(":fmeas", ui->cbMeas->currentId());
    db.bindValue(":fname", ui->leName->text());
    db.bindValue(":fprice", ui->lePrice->toInt());
    db.bindValue(":fprice2", ui->lePrice2->toInt());
    db.bindValue(":fstate", ui->cbState->currentId());
    db.bindValue(":id", ui->leCode->toInt());
    db.execSQL();
    db.prepare("delete from quick_goods where fgoods=:fgoods");
    db.bindValue(":fgoods", ui->leCode->toInt());
    db.execSQL();
    if (ui->leQuickList->toInt()) {
        db.prepare("insert into quick_goods values (null, :fgoods, :fqueue)");
        db.bindValue(":fgoods", ui->leCode->toInt());
        db.bindValue(":fqueue", ui->leQuickList->toInt());
        db.execSQL();
    }
    db.closeDB();
    if (!db.openDB())
        return;
    db.prepare("delete from barcode where fgoods=:fgoods");
    db.bindValue(":fgoods", ui->leCode->toInt());
    db.execSQL();
    for (int i = 0; i < ui->lstBarcode->count(); i++) {
        db.prepare("delete from barcode where fbarcode=:fbarcode");
        db.bindValue(":fbarcode", ui->lstBarcode->item(i)->text());
        db.execSQL();
        db.prepare("insert into barcode values (:fbarcode, :fgoods)");
        db.bindValue(":fbarcode", ui->lstBarcode->item(i)->text());
        db.bindValue(":fgoods", ui->leCode->toInt());
        if (!db.execSQL()) {
            db.closeDB();
            QMessageBox::critical(this, tr("Error"), tr("Barcode insert error: ") +  ui->lstBarcode->item(i)->text());
            return;
        }
    }
    db.closeDB();
    QMessageBox::information(this, tr("Information"), tr("Saved"));
}

void SC_EditGoods::on_actionCopy_triggered()
{
    QList<QListWidgetItem*> items = ui->lstBarcode->selectedItems();
    if (items.count() == 0) {
        return;
    }
    QClipboard *c = qApp->clipboard();
    c->setText(items.at(0)->text());
}
