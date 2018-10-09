#include "dlgchangesale.h"
#include "ui_dlgchangesale.h"
#include "common.h"

DlgChangeSale::DlgChangeSale(QWidget *parent) :
    SC_Dialog(parent),
    ui(new Ui::DlgChangeSale)
{
    ui->setupUi(this);
}

DlgChangeSale::~DlgChangeSale()
{
    delete ui;
}

void DlgChangeSale::changeSale()
{
    DlgChangeSale *d = new DlgChangeSale(fMainWindow);
    d->exec();
    delete d;
}

void DlgChangeSale::on_leCode1_returnPressed()
{
    db.prepare("select fgoods from barcode where fbarcode=:fbarcode");
    db.bindValue(":fbarcode", ui->leCode1->text());
    db.execSQL();
    if (!db.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Code was not found"));
        ui->leCode1->clear();
        return;
    }
    fId1 = db.v_int(0);
    db.prepare("select fname from goods where id=:id");
    db.bindValue(":id", fId1);
    db.execSQL();
    if (!db.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Id was not found"));
        ui->leCode1->clear();
        return;
    }
    ui->leName1->setText(db.v_str(0));
}

void DlgChangeSale::on_leCode2_returnPressed()
{
    db.prepare("select fgoods from barcode where fbarcode=:fbarcode");
    db.bindValue(":fbarcode", ui->leCode2->text());
    db.execSQL();
    if (!db.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Code was not found"));
        ui->leCode2->clear();
        return;
    }
    fId2 = db.v_int(0);
    db.prepare("select fname from goods where id=:id");
    db.bindValue(":id", fId2);
    db.execSQL();
    if (!db.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Id was not found"));
        ui->leCode2->clear();
        return;
    }
    ui->leName2->setText(db.v_str(0));
}

void DlgChangeSale::on_btnCancel_clicked()
{
    reject();
}

void DlgChangeSale::on_btnOK_clicked()
{
    if (fId1 == 0 || fId2 == 0) {
        QMessageBox::critical(this, tr("Error"), tr("One of the goods was not selected"));
        return;
    }
    db.prepare("update stb set fgoods=:fgoods1 where fgoods=:fgoods2");
    db.bindValue(":fgoods1", fId2);
    db.bindValue(":fgoods2", fId1);
    db.execSQL();
    db.closeDB();
    QMessageBox::information(this, tr("Information"), tr("Done"));
    accept();
}
